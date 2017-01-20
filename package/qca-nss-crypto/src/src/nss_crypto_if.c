/* Copyright (c) 2013-2015, The Linux Foundation. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
 * OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 *
 */
#include <nss_crypto_hlos.h>
#include <nss_api_if.h>
#include <nss_crypto.h>
#include <nss_crypto_if.h>
#include <nss_crypto_hw.h>
#include <nss_crypto_ctrl.h>
#include <nss_crypto_dbg.h>
#include <nss_crypto_debugfs.h>

#define NSS_CRYPTO_DEBUGFS_PERM_RO 0444
#define NSS_CRYPTO_DEBUGFS_PERM_RW 0666
#define NSS_CRYPTO_MSG_LEN (sizeof(struct nss_crypto_msg) - sizeof(struct nss_cmn_msg))

#define NSS_CRYPTO_ZONE_NAME_LEN	64
#define NSS_CRYPTO_ZONE_DEFAULT_NAME	"crypto_buf-"

/*
 * global control component
 */
extern struct nss_crypto_ctrl gbl_crypto_ctrl;

struct nss_ctx_instance *nss_drv_hdl;

struct nss_crypto_drv_ctx gbl_ctx = {0};

/*
 * internal structure for a buffer node
 */
struct nss_crypto_buf_node {
	struct llist_node node;			/* lockless node */
	struct nss_crypto_buf buf;		/* crypto buffer */
};

/*
 * users of crypto driver
 */
struct nss_crypto_user {
	struct list_head  node;			/* user list */
	struct llist_head pool_head;	/* buffer pool lockless list */

	nss_crypto_user_ctx_t ctx;		/* user specific context*/

	nss_crypto_attach_t attach;		/* attach function*/
	nss_crypto_detach_t detach;		/* detach function*/

	struct kmem_cache *zone;
	uint8_t zone_name[NSS_CRYPTO_ZONE_NAME_LEN];
};

LIST_HEAD(user_head);

/*
 * XXX: its expected that this should be sufficient for 4 pipes
 */
static uint32_t pool_seed = 1024;

/*
 * nss_crypto_buf_init
 * Initialize the allocated crypto buffer
 */
static inline void nss_crypto_buf_init(struct nss_crypto_buf *buf)
{
	buf->origin = NSS_CRYPTO_BUF_ORIGIN_HOST;
}

/*
 * nss_crypto_register_user()
 * 	register a new user of the crypto driver
 */
void nss_crypto_register_user(nss_crypto_attach_t attach, nss_crypto_detach_t detach, uint8_t *user_name)
{
	struct nss_crypto_user *user;
	struct nss_crypto_buf_node *entry;
	int i;

	user = vmalloc(sizeof(struct nss_crypto_user));
	nss_crypto_assert(user);

	memset(user, 0, sizeof(struct nss_crypto_user));

	user->attach = attach;
	user->ctx = user->attach(user);
	user->detach = detach;
	strlcpy(user->zone_name, NSS_CRYPTO_ZONE_DEFAULT_NAME, NSS_CRYPTO_ZONE_NAME_LEN);

	/*
	 * initialize the lockless list
	 */
	init_llist_head(&user->pool_head);

	/*
	 * Allocated the kmem_cache pool of crypto_bufs
	 * XXX: we can use the constructor
	 */
	strlcat(user->zone_name, user_name, NSS_CRYPTO_ZONE_NAME_LEN);
	user->zone = kmem_cache_create(user->zone_name, sizeof(struct nss_crypto_buf_node), 0, SLAB_HWCACHE_ALIGN, NULL);

	for (i = 0; i < pool_seed; i++) {
		entry = kmem_cache_alloc(user->zone, GFP_KERNEL);
		llist_add(&entry->node, &user->pool_head);
		nss_crypto_buf_init(&entry->buf);
	}

	list_add_tail(&user->node, &user_head);
}
EXPORT_SYMBOL(nss_crypto_register_user);

/*
 * nss_crypto_unregister_user()
 * 	unregister a user from the crypto driver
 */
void nss_crypto_unregister_user(nss_crypto_handle_t crypto)
{
	struct nss_crypto_user *user;
	struct nss_crypto_buf_node *entry;
	struct llist_node *node;
	uint32_t buf_count;

	user = (struct nss_crypto_user *)crypto;
	buf_count = 0;

	/*
	 * XXX: need to handle the case when there are packets in flight
	 * for the user
	 */
	if (user->detach) {
		user->detach(user->ctx);
	}

	while (!llist_empty(&user->pool_head)) {
		buf_count++;

		node = llist_del_first(&user->pool_head);
		entry = container_of(node, struct nss_crypto_buf_node, node);

		kmem_cache_free(user->zone, entry);
	}

	/*
	 * it will assert for now if some buffers where in flight while the deregister
	 * happened
	 */
	nss_crypto_assert(buf_count >= pool_seed);

	kmem_cache_destroy(user->zone);

	list_del(&user->node);

	vfree(user);
}
EXPORT_SYMBOL(nss_crypto_unregister_user);

/*
 * nss_crypto_buf_alloc()
 * 	allocate a crypto buffer for its user
 *
 * the allocation happens from its user pool. If, a user runs out its pool
 * then it will only be affected. Also, this function is lockless
 */
struct nss_crypto_buf *nss_crypto_buf_alloc(nss_crypto_handle_t hdl)
{
	struct nss_crypto_user *user;
	struct nss_crypto_buf_node *entry;
	struct llist_node *node;

	user = (struct nss_crypto_user *)hdl;
	node = llist_del_first(&user->pool_head);

	if (node) {
		entry = container_of(node, struct nss_crypto_buf_node, node);
		return &entry->buf;
	}

	/*
	 * Note: this condition is hit when there are more than 'seed' worth
	 * of crypto buffers outstanding with the system. Instead of failing
	 * allocation attempt allocating buffers so that pool grows itself
	 * to the right amount needed to sustain the traffic without the need
	 * for dynamic allocation in future requests
	 */
	entry = kmem_cache_alloc(user->zone, GFP_KERNEL);
	if (entry == NULL) {
		goto fail;
	}

	nss_crypto_buf_init(&entry->buf);
	return &entry->buf;

fail:
	nss_crypto_err("Unable to allocate crypto buffer from cache \n");
	return NULL;
}
EXPORT_SYMBOL(nss_crypto_buf_alloc);

/*
 * nss_crypto_buf_free()
 * 	free the crypto buffer back to the user buf pool
 */
void nss_crypto_buf_free(nss_crypto_handle_t hdl, struct nss_crypto_buf *buf)
{
	struct nss_crypto_user *user;
	struct nss_crypto_buf_node *entry;

	user = (struct nss_crypto_user *)hdl;

	entry = container_of(buf, struct nss_crypto_buf_node, buf);

	llist_add(&entry->node, &user->pool_head);

}
EXPORT_SYMBOL(nss_crypto_buf_free);

/*
 * nss_crypto_transform_done()
 * 	completion callback for NSS HLOS driver when it receives a crypto buffer
 *
 * this function assumes packets arriving from host are transform buffers that
 * have been completed by the NSS crypto. It needs to have a switch case for
 * detecting control packets also
 */
void nss_crypto_transform_done(void *app_data __attribute((unused)), void *buffer, uint32_t paddr, uint16_t len)
{
	struct nss_crypto_buf *buf = (struct nss_crypto_buf *)buffer;

	dma_unmap_single(NULL, paddr, sizeof(struct nss_crypto_buf), DMA_FROM_DEVICE);
	dma_unmap_single(NULL, buf->data_paddr, buf->data_len, DMA_FROM_DEVICE);

	buf->cb_fn(buf);
}

/*
 * nss_crypto_copy_stats()
 * 	copy stats from msg to local copy.
 */
static void nss_crypto_copy_stats(void *dst, void *src)
{
	memcpy(dst, src, sizeof(struct nss_crypto_stats));
}

/*
 * nss_crypto_process_sync()
 *	callback function for sync messages.
 */
void nss_crypto_process_event(void *app_data, struct nss_crypto_msg *nim)
{
	struct nss_crypto_ctrl *ctrl = &gbl_crypto_ctrl;
	struct nss_crypto_ctrl_eng *e_ctrl;
	struct nss_crypto_idx_info *idx;
	struct nss_crypto_sync_stats *stats;
	struct nss_crypto_config_eng *open;
	struct nss_crypto_config_session *session;
	int i;

	switch (nim->cm.type) {
	case NSS_CRYPTO_MSG_TYPE_STATS:

		stats = &nim->msg.stats;

		for (i = 0; i < ctrl->num_eng; i++) {
			e_ctrl = &ctrl->eng[i];
			nss_crypto_copy_stats(&e_ctrl->stats, &stats->eng_stats[i]);
		}

		for (i = 0; i < NSS_CRYPTO_MAX_IDXS; i++) {
			idx = &ctrl->idx_info[i];

			/*
			 * Copy statistics only if session is active
			 */
			if (nss_crypto_chk_idx_isfree(idx) == true) {
				continue;
			}

			nss_crypto_copy_stats(&idx->stats, &stats->idx_stats[i]);
		}

		nss_crypto_copy_stats(&ctrl->total_stats, &stats->total);

		break;

	case NSS_CRYPTO_MSG_TYPE_OPEN_ENG:
		open = &nim->msg.eng;

		if (nim->cm.response != NSS_CMN_RESPONSE_ACK) {
			nss_crypto_err("unable to Open the engine: resp code (%d), error code (%d)\n",
					nim->cm.response, nim->cm.error);
			return;
		}

		nss_crypto_info("engine(%d) opened successfully\n", open->eng_id);

		break;

	case NSS_CRYPTO_MSG_TYPE_RESET_SESSION:
		session = &nim->msg.session;

		if (nim->cm.response != NSS_CMN_RESPONSE_ACK) {
			nss_crypto_err("unable to reset session: resp code (%d), error code (%d)\n",
					nim->cm.response, nim->cm.error);

			return;
		}

		nss_crypto_info("session(%d) reset successfully\n", session->idx);

		nss_crypto_assert(session->idx < NSS_CRYPTO_MAX_IDXS);

		/*
		 * If NSS state has changed to free, start the delayed free
		 * timer for de-allocating session resources
		 */
		if (session->state == NSS_CRYPTO_SESSION_STATE_FREE) {
			nss_crypto_start_idx_free(session->idx);
		}

		break;

	default:
		nss_crypto_err("unsupported sync type %d\n", nim->cm.type);
		return;
	}
}

/*
 * nss_crypto_transform_payload()
 *	submit a transform for crypto operation to NSS
 */
nss_crypto_status_t nss_crypto_transform_payload(nss_crypto_handle_t crypto, struct nss_crypto_buf *buf)
{
	nss_tx_status_t nss_status;
	uint32_t paddr;

	buf->data_paddr = dma_map_single(NULL, buf->data, buf->data_len, DMA_TO_DEVICE);
	paddr = dma_map_single(NULL, buf, sizeof(struct nss_crypto_buf), DMA_TO_DEVICE);

	nss_status = nss_crypto_tx_buf(nss_drv_hdl, buf, paddr, sizeof(struct nss_crypto_buf));
	if (nss_status != NSS_TX_SUCCESS) {
		nss_crypto_dbg("Not able to send crypto buf to NSS\n");
		return NSS_CRYPTO_STATUS_FAIL;
	}

	return NSS_CRYPTO_STATUS_OK;
}
EXPORT_SYMBOL(nss_crypto_transform_payload);

/*
 * nss_crypto_init()
 * 	initialize the crypto driver
 *
 * this will do the following
 * - Bring Power management perf level to TURBO
 * - register itself to the NSS HLOS driver
 * - wait for the NSS to be ready
 * - initialize the control component
 */
void nss_crypto_init(void)
{
	nss_crypto_ctrl_init();

	gbl_ctx.pm_hdl = nss_pm_client_register(NSS_PM_CLIENT_CRYPTO);

	/*
	 * Initialize debugfs entries
	 */
	nss_crypto_debugfs_init(&gbl_crypto_ctrl);

	nss_drv_hdl = nss_crypto_notify_register(nss_crypto_process_event, &user_head);
	nss_drv_hdl = nss_crypto_data_register(nss_crypto_transform_done, &user_head);

}

/*
 * nss_crypto_engine_init()
 * 	initialize the crypto interface for each engine
 *
 * this will do the following
 * - prepare the open message for the engine
 * - initialize the control component for all pipes in that engine
 * - send the open message to the NSS crypto
 */
void nss_crypto_engine_init(uint32_t eng_num)
{
	struct nss_crypto_msg nim;
	struct nss_cmn_msg *ncm = &nim.cm;
	struct nss_crypto_config_eng *open = &nim.msg.eng;
	struct nss_crypto_ctrl_eng *e_ctrl;
	struct nss_crypto_ctrl *ctrl = &gbl_crypto_ctrl;
	int i;

	e_ctrl = &ctrl->eng[eng_num];

	memset(&nim, 0, sizeof(struct nss_crypto_msg));

	nss_cmn_msg_init(ncm,
			NSS_CRYPTO_INTERFACE,
			NSS_CRYPTO_MSG_TYPE_OPEN_ENG,
			NSS_CRYPTO_MSG_LEN,
			nss_crypto_process_event,
			(void *)eng_num);

	/*
	 * prepare the open config message
	 */
	open->eng_id = eng_num;
	open->bam_pbase = e_ctrl->bam_pbase;

	for (i = 0; i < NSS_CRYPTO_BAM_PP; i++) {
		nss_crypto_pipe_init(e_ctrl, i, &open->desc_paddr[i], &e_ctrl->hw_desc[i]);
	}

	if (nss_crypto_idx_init(e_ctrl, open->idx) != NSS_CRYPTO_STATUS_OK) {
		nss_crypto_err("failed to initiallize\n");
		return;
	}

	/*
	 * send open config message to NSS crypto
	 */
	if (nss_crypto_tx_msg(nss_drv_hdl, &nim) != NSS_TX_SUCCESS) {
		nss_crypto_err("Failed to send the message to NSS\n");
		return;
	}
}

/*
 * nss_crypto_reset_session()
 * 	reset session specific state (alloc or free)
 */
void nss_crypto_reset_session(uint32_t session_idx, enum nss_crypto_session_state state)
{
	struct nss_crypto_msg nim;
	struct nss_cmn_msg *ncm = &nim.cm;
	struct nss_crypto_config_session *session = &nim.msg.session;
	struct nss_crypto_ctrl *ctrl = &gbl_crypto_ctrl;

	switch (state) {
	case NSS_CRYPTO_SESSION_STATE_ACTIVE:
		nss_crypto_debugfs_add_session(ctrl, session_idx);

		break;

	case NSS_CRYPTO_SESSION_STATE_FREE:
		nss_crypto_debugfs_del_session(ctrl, session_idx);

		break;

	default:
		nss_crypto_err("incorrect session state = %d\n", state);

		return;
	}

	memset(&nim, 0, sizeof(struct nss_crypto_msg));
	nss_cmn_msg_init(ncm,
			NSS_CRYPTO_INTERFACE,
			NSS_CRYPTO_MSG_TYPE_RESET_SESSION,
			NSS_CRYPTO_MSG_LEN,
			nss_crypto_process_event,
			(void *)session_idx);

	session->idx = session_idx;
	session->state = state;

	/*
	 * send reset stats config message to NSS crypto
	 */
	nss_crypto_tx_msg(nss_drv_hdl, &nim);
}
