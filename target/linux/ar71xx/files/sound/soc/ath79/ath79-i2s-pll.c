/*
 * ath79-i2s-pll.c -- ALSA DAI PLL management for QCA AR71xx/AR9xxx designs
 *
 * Copyright (c) 2012 The Linux Foundation. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <linux/module.h>
#include <linux/clk.h>
#include <linux/spinlock.h>
#include <linux/delay.h>
#include <sound/core.h>
#include <sound/soc.h>
#include <sound/pcm_params.h>

#include <asm/mach-ath79/ar71xx_regs.h>
#include <asm/mach-ath79/ath79.h>

#include "ath79-i2s.h"
#include "ath79-i2s-pll.h"

static DEFINE_SPINLOCK(ath79_pll_lock);

static const struct ath79_pll_config pll_cfg_25MHz[] = {
	/* Freq		divint	divfrac		ppllpwd	bypass	extdiv	refdiv	PS	ki	kd	shift */
	/* 		-----------------------PLL----------------------------	STEREO	--------DPLL--------- */
	{ 22050,	0x15,	0x2B442,	0x3,	0,	0x6,	0x1,	3,	0x4,	0x3d,	0x6 },
	{ 32000,	0x17,	0x24F76,	0x3,	0,	0x6,	0x1,	3,	0x4,	0x3d,	0x6 },
	{ 44100,	0x15,	0x2B442,	0x3,	0,	0x6,	0x1,	2,	0x4,	0x3d,	0x6 },
	{ 48000,	0x17,	0x24F76,	0x3,	0,	0x6,	0x1,	2,	0x4,	0x3d,	0x6 },
	{ 88200,	0x15,	0x2B442,	0x3,	0,	0x6,	0x1,	1,	0x4,	0x3d,	0x6 },
	{ 96000,	0x17,	0x24F76,	0x3,	0,	0x6,	0x1,	1,	0x4,	0x3d,	0x6 },
	{ 0,		0,	0,		0,	0,	0,	0,	0,	0,	0,	0   },
};

static const struct ath79_pll_config pll_cfg_40MHz[] = {
	{ 22050,	0x1b,	0x6152,		0x3,	0,	0x6,	0x2,	3,	0x4,	0x32,	0x6 },
	{ 32000,	0x1d,	0x1F6FD,	0x3,	0,	0x6,	0x2,	3,	0x4,	0x32,	0x6 },
	{ 44100,	0x1b,	0x6152,		0x3,	0,	0x6,	0x2,	2,	0x4,	0x32,	0x6 },
	{ 48000,	0x1d,	0x1F6FD,	0x3,	0,	0x6,	0x2,	2,	0x4,	0x32,	0x6 },
	{ 88200,	0x1b,	0x6152,		0x3,	0,	0x6,	0x2,	1,	0x4,	0x32,	0x6 },
	{ 96000,	0x1d,	0x1F6FD,	0x3,	0,	0x6,	0x2,	1,	0x4,	0x32,	0x6 },
	{ 0,		0,	0,		0,	0,	0,	0,	0,	0,	0,	0   },
};

static void ath79_pll_set_target_div(u32 div_int, u32 div_frac)
{
	u32 t;

	spin_lock(&ath79_pll_lock);

	t = ath79_pll_rr(AR934X_PLL_AUDIO_MOD_REG);
	t &= ~(AR934X_PLL_AUDIO_MOD_TGT_DIV_INT_MASK
		<< AR934X_PLL_AUDIO_MOD_TGT_DIV_INT_SHIFT);
	t &= ~(AR934X_PLL_AUDIO_MOD_TGT_DIV_FRAC_MASK
		<< AR934X_PLL_AUDIO_MOD_TGT_DIV_FRAC_SHIFT);
	t |= (div_int & AR934X_PLL_AUDIO_MOD_TGT_DIV_INT_MASK)
		<< AR934X_PLL_AUDIO_MOD_TGT_DIV_INT_SHIFT;
	t |= (div_frac & AR934X_PLL_AUDIO_MOD_TGT_DIV_FRAC_MASK)
		<< AR934X_PLL_AUDIO_MOD_TGT_DIV_FRAC_SHIFT;
	ath79_pll_wr(AR934X_PLL_AUDIO_MOD_REG, t);

	spin_unlock(&ath79_pll_lock);
}

static void ath79_pll_set_refdiv(u32 refdiv)
{
	u32 t;

	spin_lock(&ath79_pll_lock);

	t = ath79_pll_rr(AR934X_PLL_AUDIO_CONFIG_REG);
	t &= ~(AR934X_PLL_AUDIO_CONFIG_REFDIV_MASK
		<< AR934X_PLL_AUDIO_CONFIG_REFDIV_SHIFT);
	t |= (refdiv & AR934X_PLL_AUDIO_CONFIG_REFDIV_MASK)
		<< AR934X_PLL_AUDIO_CONFIG_REFDIV_SHIFT;
	ath79_pll_wr(AR934X_PLL_AUDIO_CONFIG_REG, t);

	spin_unlock(&ath79_pll_lock);
}

static void ath79_pll_set_ext_div(u32 ext_div)
{
	u32 t;

	spin_lock(&ath79_pll_lock);

	t = ath79_pll_rr(AR934X_PLL_AUDIO_CONFIG_REG);
	t &= ~(AR934X_PLL_AUDIO_CONFIG_EXT_DIV_MASK
		<< AR934X_PLL_AUDIO_CONFIG_EXT_DIV_SHIFT);
	t |= (ext_div & AR934X_PLL_AUDIO_CONFIG_EXT_DIV_MASK)
		<< AR934X_PLL_AUDIO_CONFIG_EXT_DIV_SHIFT;
	ath79_pll_wr(AR934X_PLL_AUDIO_CONFIG_REG, t);

	spin_unlock(&ath79_pll_lock);
}

static void ath79_pll_set_postpllpwd(u32 postpllpwd)
{
	u32 t;

	spin_lock(&ath79_pll_lock);

	t = ath79_pll_rr(AR934X_PLL_AUDIO_CONFIG_REG);
	t &= ~(AR934X_PLL_AUDIO_CONFIG_POSTPLLPWD_MASK
		<< AR934X_PLL_AUDIO_CONFIG_POSTPLLPWD_SHIFT);
	t |= (postpllpwd & AR934X_PLL_AUDIO_CONFIG_POSTPLLPWD_MASK)
		<< AR934X_PLL_AUDIO_CONFIG_POSTPLLPWD_SHIFT;
	ath79_pll_wr(AR934X_PLL_AUDIO_CONFIG_REG, t);

	spin_unlock(&ath79_pll_lock);
}

static void ath79_pll_bypass(bool val)
{
	u32 t;

	spin_lock(&ath79_pll_lock);

	t = ath79_pll_rr(AR934X_PLL_AUDIO_CONFIG_REG);
	if(val)
		t |= AR934X_PLL_AUDIO_CONFIG_BYPASS;
	else
		t &= ~(AR934X_PLL_AUDIO_CONFIG_BYPASS);
	ath79_pll_wr(AR934X_PLL_AUDIO_CONFIG_REG, t);

	spin_unlock(&ath79_pll_lock);
}

static bool ath79_pll_ispowered(void)
{
	u32 status;

	status = ath79_pll_rr(AR934X_PLL_AUDIO_CONFIG_REG)
			& AR934X_PLL_AUDIO_CONFIG_PLLPWD;
	return ( !status ? true : false);
}

static void ath79_audiodpll_set_gains(u32 kd, u32 ki)
{
	u32 t;

	spin_lock(&ath79_pll_lock);

	if(ath79_pll_ispowered())
		BUG();

	t = ath79_audio_dpll_rr(AR934X_DPLL_REG_2);
	t &= ~(AR934X_DPLL_2_KD_MASK << AR934X_DPLL_2_KD_SHIFT);
	t &= ~(AR934X_DPLL_2_KI_MASK << AR934X_DPLL_2_KI_SHIFT);
	t |= (kd & AR934X_DPLL_2_KD_MASK) << AR934X_DPLL_2_KD_SHIFT;
	t |= (ki & AR934X_DPLL_2_KI_MASK) << AR934X_DPLL_2_KI_SHIFT;
	ath79_audio_dpll_wr(AR934X_DPLL_REG_2, t);

	spin_unlock(&ath79_pll_lock);
}

static void ath79_audiodpll_phase_shift_set(u32 phase)
{
	u32 t;

	spin_lock(&ath79_pll_lock);

	if(ath79_pll_ispowered())
		BUG();

	t = ath79_audio_dpll_rr(AR934X_DPLL_REG_3);
	t &= ~(AR934X_DPLL_3_PHASESH_MASK << AR934X_DPLL_3_PHASESH_SHIFT);
	t |= (phase & AR934X_DPLL_3_PHASESH_MASK)
		<< AR934X_DPLL_3_PHASESH_SHIFT;
	ath79_audio_dpll_wr(AR934X_DPLL_REG_3, t);

	spin_unlock(&ath79_pll_lock);
}

static void ath79_audiodpll_range_set(void)
{
	u32 t;

	spin_lock(&ath79_pll_lock);

	t = ath79_audio_dpll_rr(AR934X_DPLL_REG_2);
	t &= ~(AR934X_DPLL_2_RANGE);
	ath79_audio_dpll_wr(AR934X_DPLL_REG_2, t);
	t = ath79_audio_dpll_rr(AR934X_DPLL_REG_2);
	t |= AR934X_DPLL_2_RANGE;
	ath79_audio_dpll_wr(AR934X_DPLL_REG_2, t);

	spin_unlock(&ath79_pll_lock);
}

static u32 ath79_audiodpll_sqsum_dvc_get(void)
{
	u32 t;

	t = ath79_audio_dpll_rr(AR934X_DPLL_REG_3) >> AR934X_DPLL_3_SQSUM_DVC_SHIFT;
	t &= AR934X_DPLL_3_SQSUM_DVC_MASK;
	return t;
}

static void ath79_stereo_set_posedge(u32 posedge)
{
	u32 t;

	spin_lock(&ath79_stereo_lock);

	t = ath79_stereo_rr(AR934X_STEREO_REG_CONFIG);
	t &= ~(AR934X_STEREO_CONFIG_POSEDGE_MASK
		<< AR934X_STEREO_CONFIG_POSEDGE_SHIFT);
	t |= (posedge & AR934X_STEREO_CONFIG_POSEDGE_MASK)
		<< AR934X_STEREO_CONFIG_POSEDGE_SHIFT;
	ath79_stereo_wr(AR934X_STEREO_REG_CONFIG, t);

	spin_unlock(&ath79_stereo_lock);
}

static void ath79_pll_powerup(void)
{
	u32 t;

	spin_lock(&ath79_pll_lock);

	t = ath79_pll_rr(AR934X_PLL_AUDIO_CONFIG_REG);
	t &= ~AR934X_PLL_AUDIO_CONFIG_PLLPWD;
	ath79_pll_wr(AR934X_PLL_AUDIO_CONFIG_REG, t);

	spin_unlock(&ath79_pll_lock);
}

static void ath79_pll_powerdown(void)
{
	u32 t;

	spin_lock(&ath79_pll_lock);

	t = ath79_pll_rr(AR934X_PLL_AUDIO_CONFIG_REG);
	t |= AR934X_PLL_AUDIO_CONFIG_PLLPWD;
	ath79_pll_wr(AR934X_PLL_AUDIO_CONFIG_REG, t);

	spin_unlock(&ath79_pll_lock);
}

static void ath79_audiodpll_do_meas_set(void)
{
	u32 t;

	spin_lock(&ath79_pll_lock);

	t = ath79_audio_dpll_rr(AR934X_DPLL_REG_3);
	t |= AR934X_DPLL_3_DO_MEAS;
	ath79_audio_dpll_wr(AR934X_DPLL_REG_3, t);

	spin_unlock(&ath79_pll_lock);
}

static void ath79_audiodpll_do_meas_clear(void)
{
	u32 t;

	spin_lock(&ath79_pll_lock);

	t = ath79_audio_dpll_rr(AR934X_DPLL_REG_3);
	t &= ~(AR934X_DPLL_3_DO_MEAS);
	ath79_audio_dpll_wr(AR934X_DPLL_REG_3, t);

	spin_unlock(&ath79_pll_lock);
}

static bool ath79_audiodpll_meas_done_is_set(void)
{
	u32 status;

	status = ath79_audio_dpll_rr(AR934X_DPLL_REG_4) & AR934X_DPLL_4_MEAS_DONE;
	return ( status ? true : false);
}

static void ath79_load_pll_regs(const struct ath79_pll_config *cfg)
{
	/* Set PLL regs */
	ath79_pll_set_postpllpwd(cfg->postpllpwd);
	ath79_pll_bypass(cfg->bypass);
	ath79_pll_set_ext_div(cfg->extdiv);
	ath79_pll_set_refdiv(cfg->refdiv);
	ath79_pll_set_target_div(cfg->divint, cfg->divfrac);
	/* Set DPLL regs */
	ath79_audiodpll_range_set();
	ath79_audiodpll_phase_shift_set(cfg->shift);
	ath79_audiodpll_set_gains(cfg->kd, cfg->ki);
	/* Set Stereo regs */
	ath79_stereo_set_posedge(cfg->posedge);
	return;
}

int ath79_audio_set_freq(int freq)
{
	struct clk *clk;
	const struct ath79_pll_config *cfg;

	clk = clk_get(NULL, "ref");

	/* PLL settings can have 2 different values depending
	 * on the clock rate */
	switch(clk_get_rate(clk)) {
	case 25*1000*1000:
		cfg = &pll_cfg_25MHz[0];
		break;
	case 40*1000*1000:
		cfg = &pll_cfg_40MHz[0];
		break;
	default:
		printk(KERN_ERR "%s: Clk speed %lu.%03lu not supported\n", __FUNCTION__,
			clk_get_rate(clk)/1000000,(clk_get_rate(clk)/1000) % 1000);
		return -EIO;
	}

	/* Search the frequency in the pll table */
	do {
		if(cfg->rate == freq)
			break;
		cfg++;
	} while(cfg->rate != 0);
	if (cfg->rate == 0) {
		printk(KERN_ERR "%s: Freq %d not supported\n",
			__FUNCTION__, freq);
		return -ENOTSUPP;
	}

	/* Loop until we converged to an acceptable value */
	do {
		ath79_audiodpll_do_meas_clear();
		ath79_pll_powerdown();
		udelay(100);

		ath79_load_pll_regs(cfg);

		ath79_pll_powerup();
		ath79_audiodpll_do_meas_clear();
		ath79_audiodpll_do_meas_set();

		while ( ! ath79_audiodpll_meas_done_is_set()) {
			udelay(10);
		}

	} while (ath79_audiodpll_sqsum_dvc_get() >= 0x40000);

	return 0;
}

