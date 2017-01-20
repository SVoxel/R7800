/*
 * Atheros AR71xx Audio registers management functions
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

struct ath79_pll_config {
	int rate;		/* Stream frequency */
	int divint;		/* AUDIO_PLL_MODULATION		06:01 */
	int divfrac;		/* AUDIO_PLL_MODULATION		28:11 */
	int postpllpwd;		/* AUDIO_PLL_CONFIG		09:07 */
	int bypass;		/* AUDIO_PLL_CONFIG		04    */
	int extdiv;		/* AUDIO_PLL_CONFIG		14:12 */
	int refdiv;		/* AUDIO_PLL_CONFIG		03:00 */
	int posedge;		/* STEREO_CONFIG		07:00 */
	int ki;			/* AUDIO_DPLL2			29:26 */
	int kd;			/* AUDIO_DPLL2			25:19 */
	int shift;		/* AUDIO_DPLL3			29:23 */
};

int ath79_audio_set_freq(int);
