/* This is a generated file */
#ifndef __hdb_private_h__
#define __hdb_private_h__

#include <stdarg.h>

hdb_master_key
_hdb_find_master_key (
	uint32_t */*mkvno*/,
	hdb_master_key /*mkey*/);

krb5_error_code
_hdb_keytab2hdb_entry (
	krb5_context /*context*/,
	const krb5_keytab_entry */*ktentry*/,
	hdb_entry_ex */*entry*/);

int
_hdb_mkey_decrypt (
	krb5_context /*context*/,
	hdb_master_key /*key*/,
	krb5_key_usage /*usage*/,
	void */*ptr*/,
	size_t /*size*/,
	krb5_data */*res*/);

int
_hdb_mkey_encrypt (
	krb5_context /*context*/,
	hdb_master_key /*key*/,
	krb5_key_usage /*usage*/,
	const void */*ptr*/,
	size_t /*size*/,
	krb5_data */*res*/);

int
_hdb_mkey_version (hdb_master_key /*mkey*/);

#endif /* __hdb_private_h__ */
