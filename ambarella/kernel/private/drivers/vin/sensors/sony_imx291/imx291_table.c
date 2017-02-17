/*
 * kernel/private/drivers/ambarella/vin/sensors/sony_imx291/imx291_table.c
 *
 * History:
 *    2014/12/05 - [Long Zhao] Create
 *
 * Copyright (C) 2004-2014, Ambarella, Inc.
 *
 * All rights reserved. No Part of this file may be reproduced, stored
 * in a retrieval system, or transmitted, in any form, or by any means,
 * electronic, mechanical, photocopying, recording, or otherwise,
 * without the prior consent of Ambarella, Inc.
 *
 */
static struct vin_video_pll imx291_plls[] = {
	{0, 37125000, 148500000}, /* for 1080p */
	{0, 37125000, 148500000}, /* for 720p */
};

static struct vin_reg_16_8 imx291_pll_regs[][6] = {
	{	/* for 1080p */
		{IMX291_INCKSEL1, 0x18},
		{IMX291_INCKSEL2, 0x00},
		{IMX291_INCKSEL3, 0x20},
		{IMX291_INCKSEL4, 0x01},
		{IMX291_INCKSEL5, 0x1A},
		{IMX291_INCKSEL6, 0x1A},
	},
	{	/* for 720p */
		{IMX291_INCKSEL1, 0x20},
		{IMX291_INCKSEL2, 0x00},
		{IMX291_INCKSEL3, 0x20},
		{IMX291_INCKSEL4, 0x01},
		{IMX291_INCKSEL5, 0x1A},
		{IMX291_INCKSEL6, 0x1A},
	},
};

static struct vin_reg_16_8 imx291_mode_regs[][11] = {
	{	/* 1080p120 10bits */
		{IMX291_ADBIT,			0x00},
		{IMX291_WINMODE,		0x00},
		{IMX291_FRSEL,			0x00},
		{IMX291_VMAX_LSB,		0x65},
		{IMX291_VMAX_MSB,		0x04},
		{IMX291_VMAX_HSB,		0x00},
		{IMX291_HMAX_LSB,		0x4C},
		{IMX291_HMAX_MSB,		0x04},
		{IMX291_ODBIT,			0xF0},
		{IMX291_BLKLEVEL_LSB,	0x3C},
		{IMX291_BLKLEVEL_MSB,	0x00},
	},
	{	/* 720p120 10bits */
		{IMX291_ADBIT,			0x00},
		{IMX291_WINMODE,		0x10},
		{IMX291_FRSEL,			0x00},
		{IMX291_VMAX_LSB,		0xEE},
		{IMX291_VMAX_MSB,		0x02},
		{IMX291_VMAX_HSB,		0x00},
		{IMX291_HMAX_LSB,		0x72},
		{IMX291_HMAX_MSB,		0x06},
		{IMX291_ODBIT,			0xE0},
		{IMX291_BLKLEVEL_LSB,	0x3C},
		{IMX291_BLKLEVEL_MSB,	0x00},
	}
};

static struct vin_reg_16_8 imx291_share_regs[] = {
	/* chip ID: 02h */
	{0x300F, 0x00},
	{0x3010, 0x21},
	{0x3012, 0x64},
	{0x3016, 0x08}, /* set to 0x08 to fix flare issue for switching HCG/LCG mode */
	{0x3070, 0x02},
	{0x3071, 0x11},
	{0x30A6, 0x20},
	{0x30A8, 0x20},
	{0x30AA, 0x20},
	{0x30AC, 0x20},

	/* chip ID: 03h */
	{0x310B, 0x01}, /* according to sony fae's suggestion, set to 1 to fix lowlight issue for 1080p */
	{0x3119, 0x9E},
	{0x311E, 0x08},
	{0x3128, 0x05},
	{0x3134, 0x0F},
	{0x313B, 0x50},
	{0x313D, 0x83},
	{0x317C, 0x00},
	{0x317F, 0x00},

	/* chip ID: 04h */
	{0x32B8, 0x50},
	{0x32B9, 0x10},
	{0x32BA, 0x00},
	{0x32BB, 0x04},
	{0x32C8, 0x50},
	{0x32C9, 0x10},
	{0x32CA, 0x00},
	{0x32CB, 0x04},

	/* chip ID: 05h */
	{0x332C, 0xD3},
	{0x332D, 0x10},
	{0x332E, 0x0D},
	{0x3358, 0x06},
	{0x3359, 0xE1},
	{0x335A, 0x11},
	{0x3360, 0x1E},
	{0x3361, 0x61},
	{0x3362, 0x10},
	{0x33B0, 0x08},
	{0x33B1, 0x30},
	{0x33B3, 0x04},
};

#ifdef CONFIG_PM
static struct vin_reg_16_8 pm_regs[] = {
	{IMX291_VMAX_HSB, 0x00},
	{IMX291_VMAX_MSB, 0x00},
	{IMX291_VMAX_LSB, 0x00},
	{IMX291_SHS1_HSB, 0x00},
	{IMX291_SHS1_MSB, 0x00},
	{IMX291_SHS1_LSB, 0x00},
	{IMX291_GAIN, 0x00},
};
#endif

static struct vin_video_format imx291_formats[] = {
	{
		.video_mode	= AMBA_VIDEO_MODE_1080P,
		.def_start_x	= 4+8,
		.def_start_y	= 1+2+10+8,
		.def_width	= 1920,
		.def_height	= 1080,
		/* sensor mode */
		.device_mode	= 0,
		.pll_idx	= 0,
		.width		= 1920,
		.height		= 1080,
		.format		= AMBA_VIDEO_FORMAT_PROGRESSIVE,
		.type		= AMBA_VIDEO_TYPE_RGB_RAW,
		.bits		= AMBA_VIDEO_BITS_10,
		.ratio		= AMBA_VIDEO_RATIO_16_9,
		.max_fps	= AMBA_VIDEO_FPS_120,
		.default_fps	= AMBA_VIDEO_FPS_29_97,
		.default_agc	= 0,
		.default_shutter_time	= AMBA_VIDEO_FPS_60,
		.default_bayer_pattern	= VINDEV_BAYER_PATTERN_RG,
	},
	{
		.video_mode	= AMBA_VIDEO_MODE_720P,
		.def_start_x	= 4+8,
		.def_start_y	= 1+2+4+4,
		.def_width	= 1280,
		.def_height	= 720,
		/* sensor mode */
		.device_mode	= 1,
		.pll_idx	= 1,
		.width		= 1280,
		.height		= 720,
		.format		= AMBA_VIDEO_FORMAT_PROGRESSIVE,
		.type		= AMBA_VIDEO_TYPE_RGB_RAW,
		.bits		= AMBA_VIDEO_BITS_10,
		.ratio		= AMBA_VIDEO_RATIO_16_9,
		.max_fps	= AMBA_VIDEO_FPS_120,
		.default_fps	= AMBA_VIDEO_FPS_29_97,
		.default_agc	= 0,
		.default_shutter_time	= AMBA_VIDEO_FPS_60,
		.default_bayer_pattern	= VINDEV_BAYER_PATTERN_RG,
	},
};

#define IMX291_GAIN_MAX_DB  210 /* 63dB */
#define IMX291_GAIN_ROWS 211
