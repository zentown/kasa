#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include <sched.h>
#include <signal.h>
#include <getopt.h>

#include "img_abs_filter.h"
#include "img_adv_struct_arch.h"
#include "iav_common.h"

#include "ov9718_liso_adj_param.c"
#include "ov9718_aliso_adj_param.c"
#include "ov9718_mliso_adj_param.c"
#include "ov9718_aeb_param.c"

#include "ov9715_liso_adj_param.c"
#include "ov9715_aliso_adj_param.c"
#include "ov9715_mliso_adj_param.c"
#include "ov9715_aeb_param.c"

#include "ov9750_liso_adj_param.c"
#include "ov9750_aliso_adj_param.c"
#include "ov9750_mliso_adj_param.c"
#include "ov9750_aeb_param.c"

#include "ov4689_liso_adj_param.c"
#include "ov4689_mliso_adj_param.c"
#include "ov4689_mliso_adj_param_hdr.c"
#include "ov4689_aliso_adj_param.c"
#include "ov4689_aeb_param.c"

#include "ov5658_liso_adj_param.c"
#include "ov5658_mliso_adj_param.c"
#include "ov5658_aliso_adj_param.c"
#include "ov5658_aeb_param.c"

#include "mn34220pl_liso_adj_param.c"
#include "mn34220pl_mliso_adj_param.c"
#include "mn34220pl_aliso_adj_param.c"
#include "mn34220pl_bliso_adj_param.c"
#include "mn34220pl_aeb_param.c"

#include "imx123_liso_adj_param.c"
//#include "imx123_liso_adj_param_hdr.c"
#include "imx123_mliso_adj_param.c"
#include "imx123_mliso_adj_param_hdr.c"
#include "imx123_aliso_adj_param.c"
#include "imx123_aeb_param.c"

#include "imx124_liso_adj_param.c"
#include "imx124_mliso_adj_param.c"
#include "imx124_aliso_adj_param.c"
#include "imx124_aeb_param.c"

#include "imx104_mliso_adj_param.c"
#include "imx104_aeb_param.c"

#include "imx123dcg_liso_adj_param.c"
#include "imx123dcg_mliso_adj_param.c"
#include "imx123dcg_aliso_adj_param.c"
#include "imx123dcg_aeb_param.c"

#include "imx178_liso_adj_param.c"
#include "imx178_mliso_adj_param.c"
#include "imx178_aliso_adj_param.c"
#include "imx178_aeb_param.c"

#include "imx224_liso_adj_param.c"
#include "imx224_mliso_adj_param.c"
#include "imx224_aliso_adj_param.c"
#include "imx224_aeb_param.c"

#include "imx291_liso_adj_param.c"
#include "imx291_mliso_adj_param.c"
#include "imx291_aliso_adj_param.c"
#include "imx291_aeb_param.c"

#include "imx290_liso_adj_param.c"
#include "imx290_mliso_adj_param.c"
#include "imx290_aliso_adj_param.c"
#include "imx290_bliso_adj_param.c"
#include "imx290_aeb_param.c"

#include "imx322_liso_adj_param.c"
#include "imx322_mliso_adj_param.c"
#include "imx322_aliso_adj_param.c"
#include "imx322_aeb_param.c"

#include "ar0130_liso_adj_param.c"
#include "ar0130_aliso_adj_param.c"
#include "ar0130_mliso_adj_param.c"
#include "ar0130_aeb_param.c"

#include "mt9t002_liso_adj_param.c"
#include "mt9t002_aliso_adj_param.c"
#include "mt9t002_mliso_adj_param.c"
#include "mt9t002_aeb_param.c"

#include "ar0141_liso_adj_param.c"
#include "ar0141_aliso_adj_param.c"
#include "ar0141_mliso_adj_param.c"
#include "ar0141_aeb_param.c"

#include "ar0230_liso_adj_param.c"
#include "ar0230_mliso_adj_param.c"
#include "ar0230_aliso_adj_param.c"
#include "ar0230_bliso_adj_param.c"
#include "ar0230_aeb_param.c"

#include "ar0237_liso_adj_param.c"
#include "ar0237_mliso_adj_param.c"
#include "ar0237_aliso_adj_param.c"
#include "ar0237_bliso_adj_param.c"
#include "ar0237_aeb_param.c"

#include "m13vp288ir_piris_param.c"
#include "mz128bp2810icr_piris_param.c"

//#include "ambas_vin.h"
#include "AmbaDSP_ImgFilter.h"
#include "img_api_adv_arch.h"

#define	IMGPROC_PARAM_PATH	"/etc/idsp"
static u8 reg[AMBA_DSP_IMG_CC_REG_SIZE];
static u8 matrix[AMBA_DSP_IMG_CC_3D_SIZE];
static lens_ID lens_mount_id = LENS_CMOUNT_ID;

int load_cc_bin(char * sensor_name)
{
	int file, count;
	char filename[128];
	u8 *matrix[4];
	u8 i;

	for (i = 0; i < 4; i++) {
		matrix[i] = malloc(AMBA_DSP_IMG_CC_3D_SIZE);
		sprintf(filename,"%s/sensors/%s_0%d_3D.bin", IMGPROC_PARAM_PATH, sensor_name, (i+1));
		if ((file = open(filename, O_RDONLY, 0)) < 0) {
			printf("%s cannot be opened\n", filename);
			return -1;
		}
		if((count = read(file, matrix[i], AMBA_DSP_IMG_CC_3D_SIZE)) != AMBA_DSP_IMG_CC_3D_SIZE) {
			printf("read %s error\n",filename);
			return -1;
		}
		close(file);
	}
	cc_binary_addr_t cc_addr;
	cc_addr.cc_0 = matrix[0];
	cc_addr.cc_1 = matrix[1];
	cc_addr.cc_2 = matrix[2];
	cc_addr.cc_3 = matrix[3];
	img_adj_load_cc_binary(&cc_addr);

	for(i=0; i<4; i++)
		free(matrix[i]);
	return 0;
}

int enable_cc(int fd_iav, char* sensor_name, amba_img_dsp_mode_cfg_t* ik_mode)
{
	amba_img_dsp_color_correction_reg_t cc_reg;
	amba_img_dsp_color_correction_t cc_3d;

	char filename[128];
	int file, count;
	sprintf(filename,"%s/reg.bin", IMGPROC_PARAM_PATH);
	if ((file = open(filename, O_RDONLY, 0)) < 0) {
		printf("%s cannot be opened\n", filename);
		return -1;
	}
	if((count = read(file, reg, AMBA_DSP_IMG_CC_REG_SIZE)) != AMBA_DSP_IMG_CC_REG_SIZE) {
		printf("read %s error\n", filename);
		return -1;
	}
	close(file);

	sprintf(filename,"%s/sensors/%s_0%d_3D.bin", IMGPROC_PARAM_PATH, sensor_name, 2);
	if((file = open(filename, O_RDONLY, 0)) < 0) {
		printf("%s cannot be opened\n", filename);
		return -1;
	}
	if((count = read(file, matrix, AMBA_DSP_IMG_CC_3D_SIZE)) != AMBA_DSP_IMG_CC_3D_SIZE) {
		printf("read %s error\n", filename);
		return -1;
	}
	close(file);

	cc_reg.RegSettingAddr = (u32)reg;
	amba_img_dsp_set_color_correction_reg(ik_mode, &cc_reg);

	cc_3d.MatrixThreeDTableAddr = (u32)matrix;
	amba_img_dsp_set_color_correction(fd_iav, ik_mode,&cc_3d);
	return 0;
}

int load_containers(u32 sensor_id,char* sensor_name, img_config_info_t *p_img_cfg)
{
	fc_collection_t fcc;
	hdr_pipeline_t hdr_pipeline = p_img_cfg->hdr_config.pipeline;
	isp_pipeline_t isp_pipeline = p_img_cfg->isp_pipeline;
	hdr_method_t hdr_method = p_img_cfg->hdr_config.method;
	u8 expo_num = p_img_cfg->hdr_config.expo_num;
	memset(&fcc, 0, sizeof(fcc));
	switch(sensor_id){
		case SENSOR_MN34220PL:
			printf("mn34220pl\n");
			if(isp_pipeline ==ISP_PIPELINE_LISO){
				fcc.fc_ae_target = &mn34220pl_liso_fc_ae_target;
				fcc.fc_wb_ratio = &mn34220pl_liso_fc_wb_ratio;
				fcc.fc_blc = &mn34220pl_liso_fc_blc;
				fcc.fc_antialiasing = &mn34220pl_liso_fc_antialiasing;
				fcc.fc_grgbmismatch = &mn34220pl_liso_fc_grgbmismatch;
				fcc.fc_dpc = &mn34220pl_liso_fc_dpc;
				fcc.fc_cfanf_low = &mn34220pl_liso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = (hdr_pipeline == HDR_PIPELINE_OFF)? NULL : \
					((expo_num == HDR_2X)? &mn34220pl_liso_fc_2x_hdr_le : &mn34220pl_liso_fc_3x_hdr_le);
				fcc.fc_demosaic = &mn34220pl_liso_fc_demosaic;
				fcc.fc_cc = &mn34220pl_liso_fc_cc;
				fcc.fc_tone = (hdr_pipeline == HDR_PIPELINE_OFF)? &mn34220pl_liso_fc_tone : \
					((expo_num == HDR_2X)? &mn34220pl_liso_fc_2x_hdr_tone : &mn34220pl_liso_fc_3x_hdr_tone);
				fcc.fc_rgb2yuv = &mn34220pl_liso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &mn34220pl_liso_fc_chroma_scale;
				fcc.fc_chroma_median = &mn34220pl_liso_fc_chroma_median;
				fcc.fc_chroma_nf = &mn34220pl_liso_fc_chroma_nf;
				fcc.fc_cdnr = &mn34220pl_liso_fc_cdnr;
				fcc.fc_1stmode_sel = &mn34220pl_liso_fc_1stmode_sel;
				fcc.fc_asf = &mn34220pl_liso_fc_asf;
				fcc.fc_1st_shpboth = &mn34220pl_liso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &mn34220pl_liso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &mn34220pl_liso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &mn34220pl_liso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &mn34220pl_liso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &mn34220pl_liso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &mn34220pl_liso_fc_1st_shpcoring_scale_coring;
				fcc.fc_video_mctf = &mn34220pl_liso_fc_video_mctf;
				fcc.fc_hdr_alpha =  \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &mn34220pl_liso_fc_hdr_alpha;
				fcc.fc_hdr_threshold = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &mn34220pl_liso_fc_hdr_threshold;
				fcc.fc_hdr_ce = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &mn34220pl_liso_fc_hdr_ce;
			}
			else if(isp_pipeline ==ISP_PIPELINE_ADV_LISO){
				fcc.fc_ae_target = &mn34220pl_aliso_fc_ae_target;
				fcc.fc_wb_ratio = &mn34220pl_aliso_fc_wb_ratio;
				fcc.fc_blc = &mn34220pl_aliso_fc_blc;
				fcc.fc_antialiasing = &mn34220pl_aliso_fc_antialiasing;
				fcc.fc_grgbmismatch = &mn34220pl_aliso_fc_grgbmismatch;
				fcc.fc_dpc = &mn34220pl_aliso_fc_dpc;
				fcc.fc_cfanf_low = &mn34220pl_aliso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = (hdr_pipeline == HDR_PIPELINE_OFF)? NULL : \
					((expo_num == HDR_2X)? &mn34220pl_aliso_fc_2x_hdr_le : &mn34220pl_aliso_fc_3x_hdr_le);
				fcc.fc_demosaic = &mn34220pl_aliso_fc_demosaic;
				fcc.fc_cc = &mn34220pl_aliso_fc_cc;
				fcc.fc_tone = (hdr_pipeline == HDR_PIPELINE_OFF)? &mn34220pl_aliso_fc_tone : \
					((expo_num = HDR_2X)? &mn34220pl_aliso_fc_2x_hdr_tone : &mn34220pl_aliso_fc_3x_hdr_tone);
				fcc.fc_rgb2yuv = &mn34220pl_aliso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &mn34220pl_aliso_fc_chroma_scale;
				fcc.fc_chroma_median = &mn34220pl_aliso_fc_chroma_median;
				fcc.fc_chroma_nf = &mn34220pl_aliso_fc_chroma_nf;
				fcc.fc_cdnr = &mn34220pl_aliso_fc_cdnr;
				fcc.fc_1stmode_sel = &mn34220pl_aliso_fc_1stmode_sel;
				fcc.fc_asf = &mn34220pl_aliso_fc_asf;
				fcc.fc_1st_shpboth = &mn34220pl_aliso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &mn34220pl_aliso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &mn34220pl_aliso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &mn34220pl_aliso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &mn34220pl_aliso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &mn34220pl_aliso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &mn34220pl_aliso_fc_1st_shpcoring_scale_coring;
				fcc.fc_final_shpboth = &mn34220pl_aliso_fc_final_shpboth;
				fcc.fc_final_shpnoise = &mn34220pl_aliso_fc_final_shpnoise;
				fcc.fc_final_shpfir = &mn34220pl_aliso_fc_final_shpfir;
				fcc.fc_final_shpcoring = &mn34220pl_aliso_fc_final_shpcoring;
				fcc.fc_final_shpcoring_idx_scale = &mn34220pl_aliso_fc_final_shpcoring_idx_scale;
				fcc.fc_final_shpcoring_min = &mn34220pl_aliso_fc_final_shpcoring_min;
				fcc.fc_final_shpcoring_scale_coring = &mn34220pl_aliso_fc_final_shpcoring_scale_coring;
				fcc.fc_wide_chroma_filter =&mn34220pl_aliso_fc_wide_chroma_filter;
				fcc.fc_wide_chroma_filter_combine =&mn34220pl_aliso_fc_wide_chroma_filter_combine;
				fcc.fc_video_mctf = &mn34220pl_aliso_fc_video_mctf;
				fcc.fc_video_mctf_temporal_adjust =&mn34220pl_aliso_fc_video_mctf_temporal_adjust;
				fcc.fc_hdr_alpha = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &mn34220pl_aliso_fc_hdr_alpha;
				fcc.fc_hdr_threshold = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &mn34220pl_aliso_fc_hdr_threshold;
				fcc.fc_hdr_ce = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &mn34220pl_aliso_fc_hdr_ce;
				}
			else if(isp_pipeline ==ISP_PIPELINE_MID_LISO){
				fcc.fc_ae_target = &mn34220pl_mliso_fc_ae_target;
				fcc.fc_wb_ratio = &mn34220pl_mliso_fc_wb_ratio;
				fcc.fc_blc = &mn34220pl_mliso_fc_blc;
				fcc.fc_antialiasing = &mn34220pl_mliso_fc_antialiasing;
				fcc.fc_grgbmismatch = &mn34220pl_mliso_fc_grgbmismatch;
				fcc.fc_dpc = &mn34220pl_mliso_fc_dpc;
				fcc.fc_cfanf_low = &mn34220pl_mliso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = (hdr_pipeline == HDR_PIPELINE_OFF)? NULL : \
					((expo_num == HDR_2X)? &mn34220pl_mliso_fc_2x_hdr_le : &mn34220pl_mliso_fc_3x_hdr_le);
				fcc.fc_demosaic = &mn34220pl_mliso_fc_demosaic;
				fcc.fc_cc = &mn34220pl_mliso_fc_cc;
				fcc.fc_tone = (hdr_pipeline == HDR_PIPELINE_OFF)? &mn34220pl_mliso_fc_tone : \
					((expo_num == HDR_2X)? &mn34220pl_mliso_fc_2x_hdr_tone : &mn34220pl_mliso_fc_3x_hdr_tone);
				fcc.fc_rgb2yuv = &mn34220pl_mliso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &mn34220pl_mliso_fc_chroma_scale;
				fcc.fc_chroma_median = &mn34220pl_mliso_fc_chroma_median;
				fcc.fc_chroma_nf = &mn34220pl_mliso_fc_chroma_nf;
				fcc.fc_cdnr = &mn34220pl_mliso_fc_cdnr;
				fcc.fc_1stmode_sel = &mn34220pl_mliso_fc_1stmode_sel;
				fcc.fc_asf = &mn34220pl_mliso_fc_asf;
				fcc.fc_1st_shpboth = &mn34220pl_mliso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &mn34220pl_mliso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &mn34220pl_mliso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &mn34220pl_mliso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &mn34220pl_mliso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &mn34220pl_mliso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &mn34220pl_mliso_fc_1st_shpcoring_scale_coring;
				fcc.fc_video_mctf = &mn34220pl_mliso_fc_video_mctf;
				fcc.fc_video_mctf_temporal_adjust =&mn34220pl_mliso_fc_video_mctf_temporal_adjust;
				fcc.fc_hdr_alpha = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &mn34220pl_mliso_fc_hdr_alpha;
				fcc.fc_hdr_threshold = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &mn34220pl_mliso_fc_hdr_threshold;
				fcc.fc_hdr_ce = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &mn34220pl_mliso_fc_hdr_ce;
				}
				else if(isp_pipeline ==ISP_PIPELINE_B_LISO){
				fcc.fc_ae_target = &mn34220pl_bliso_fc_ae_target;
				fcc.fc_wb_ratio = &mn34220pl_bliso_fc_wb_ratio;
				fcc.fc_blc = &mn34220pl_bliso_fc_blc;
				fcc.fc_antialiasing = &mn34220pl_bliso_fc_antialiasing;
				fcc.fc_grgbmismatch = &mn34220pl_bliso_fc_grgbmismatch;
				fcc.fc_dpc = &mn34220pl_bliso_fc_dpc;
				fcc.fc_cfanf_low = &mn34220pl_bliso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = &mn34220pl_bliso_le;
				fcc.fc_demosaic = &mn34220pl_bliso_fc_demosaic;
				fcc.fc_cc = &mn34220pl_bliso_fc_cc;
				fcc.fc_tone = (hdr_pipeline == HDR_PIPELINE_OFF)? &mn34220pl_bliso_fc_tone : \
					((expo_num = HDR_2X)? &mn34220pl_bliso_fc_2x_hdr_tone : &mn34220pl_bliso_fc_3x_hdr_tone);
				fcc.fc_rgb2yuv = &mn34220pl_bliso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &mn34220pl_bliso_fc_chroma_scale;
				fcc.fc_chroma_median = &mn34220pl_bliso_fc_chroma_median;
				fcc.fc_chroma_nf = &mn34220pl_bliso_fc_chroma_nf;
				fcc.fc_1stmode_sel = &mn34220pl_bliso_fc_1stmode_sel;
				fcc.fc_asf = &mn34220pl_bliso_fc_asf;
				fcc.fc_1st_shpboth = &mn34220pl_bliso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &mn34220pl_bliso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &mn34220pl_bliso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &mn34220pl_bliso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &mn34220pl_bliso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &mn34220pl_bliso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &mn34220pl_bliso_fc_1st_shpcoring_scale_coring;

				fcc.fc_video_mctf = &mn34220pl_bliso_fc_video_mctf;
				fcc.fc_video_mctf_temporal_adjust =&mn34220pl_bliso_fc_video_mctf_temporal_adjust;
				fcc.fc_hdr_alpha = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &mn34220pl_bliso_fc_hdr_alpha;
				fcc.fc_hdr_threshold = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &mn34220pl_bliso_fc_hdr_threshold;
				fcc.fc_hdr_ce = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &mn34220pl_bliso_fc_hdr_ce;
				fcc.fc_mo_antialiasing = &mn34220pl_bliso_fc_mo_antialiasing;
				fcc.fc_mo_grgbmismatch = &mn34220pl_bliso_fc_mo_grgbmismatch;
				fcc.fc_mo_dpc = &mn34220pl_bliso_fc_mo_dpc;
				fcc.fc_mo_cfanf = &mn34220pl_bliso_fc_mo_cfanf;
				fcc.fc_mo_demosaic = &mn34220pl_bliso_fc_mo_demosaic;
				fcc.fc_mo_chroma_median = &mn34220pl_bliso_fc_mo_chroma_median;
				fcc.fc_mo_chroma_nf = &mn34220pl_bliso_fc_mo_chroma_nf;
				fcc.fc_mo_1stmode_sel = &mn34220pl_bliso_fc_mo_1stmode_sel;
				fcc.fc_mo_asf = &mn34220pl_bliso_fc_mo_asf;
				fcc.fc_mo_1st_shpboth = &mn34220pl_bliso_fc_mo_1st_shpboth;
				fcc.fc_mo_1st_shpnoise = &mn34220pl_bliso_fc_mo_1st_shpnoise;
				fcc.fc_mo_1st_shpfir = &mn34220pl_bliso_fc_mo_1st_shpfir;
				fcc.fc_mo_1st_shpcoring = &mn34220pl_bliso_fc_mo_1st_shpcoring;
				fcc.fc_mo_1st_shpcoring_idx_scale = &mn34220pl_bliso_fc_mo_1st_shpcoring_idx_scale;
				fcc.fc_mo_1st_shpcoring_min = &mn34220pl_bliso_fc_mo_1st_shpcoring_min;
				fcc.fc_mo_1st_shpcoring_scale_coring = &mn34220pl_bliso_fc_mo_1st_shpcoring_scale_coring;
				}
			sprintf(sensor_name, "mn34220pl");
			break;
		case SENSOR_OV9718:
			printf("ov9718\n");
			if(isp_pipeline ==ISP_PIPELINE_LISO){
				fcc.fc_ae_target = &ov9718_liso_fc_ae_target;
				fcc.fc_wb_ratio = &ov9718_liso_fc_wb_ratio;
				fcc.fc_blc = &ov9718_liso_fc_blc;
				fcc.fc_antialiasing = &ov9718_liso_fc_antialiasing;
				fcc.fc_grgbmismatch = &ov9718_liso_fc_grgbmismatch;
				fcc.fc_dpc = &ov9718_liso_fc_dpc;
				fcc.fc_cfanf_low = &ov9718_liso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = NULL;
				fcc.fc_demosaic = &ov9718_liso_fc_demosaic;
				fcc.fc_cc = &ov9718_liso_fc_cc;
				fcc.fc_tone = &ov9718_liso_fc_tone;
				fcc.fc_rgb2yuv = &ov9718_liso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &ov9718_liso_fc_chroma_scale;
				fcc.fc_chroma_median = &ov9718_liso_fc_chroma_median;
				fcc.fc_chroma_nf = &ov9718_liso_fc_chroma_nf;
				fcc.fc_cdnr = &ov9718_liso_fc_cdnr;
				fcc.fc_1stmode_sel = &ov9718_liso_fc_1stmode_sel;
				fcc.fc_asf = &ov9718_liso_fc_asf;
				fcc.fc_1st_shpboth = &ov9718_liso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &ov9718_liso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &ov9718_liso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &ov9718_liso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &ov9718_liso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &ov9718_liso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &ov9718_liso_fc_1st_shpcoring_scale_coring;
				fcc.fc_video_mctf = &ov9718_liso_fc_video_mctf;
				}
			else if(isp_pipeline ==ISP_PIPELINE_ADV_LISO){
				fcc.fc_ae_target = &ov9718_aliso_fc_ae_target;
				fcc.fc_wb_ratio = &ov9718_aliso_fc_wb_ratio;
				fcc.fc_blc = &ov9718_aliso_fc_blc;
				fcc.fc_antialiasing = &ov9718_aliso_fc_antialiasing;
				fcc.fc_grgbmismatch = &ov9718_aliso_fc_grgbmismatch;
				fcc.fc_dpc = &ov9718_aliso_fc_dpc;
				fcc.fc_cfanf_low = &ov9718_aliso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = NULL;
				fcc.fc_demosaic = &ov9718_aliso_fc_demosaic;
				fcc.fc_cc = &ov9718_aliso_fc_cc;
				fcc.fc_tone = &ov9718_aliso_fc_tone;
				fcc.fc_rgb2yuv = &ov9718_aliso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &ov9718_aliso_fc_chroma_scale;
				fcc.fc_chroma_median = &ov9718_aliso_fc_chroma_median;
				fcc.fc_chroma_nf = &ov9718_aliso_fc_chroma_nf;
				fcc.fc_cdnr = &ov9718_aliso_fc_cdnr;
				fcc.fc_1stmode_sel = &ov9718_aliso_fc_1stmode_sel;
				fcc.fc_asf = &ov9718_aliso_fc_asf;
				fcc.fc_1st_shpboth = &ov9718_aliso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &ov9718_aliso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &ov9718_aliso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &ov9718_aliso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &ov9718_aliso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &ov9718_aliso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &ov9718_aliso_fc_1st_shpcoring_scale_coring;
				fcc.fc_final_shpboth = &ov9718_aliso_fc_final_shpboth;
				fcc.fc_final_shpnoise = &ov9718_aliso_fc_final_shpnoise;
				fcc.fc_final_shpfir = &ov9718_aliso_fc_final_shpfir;
				fcc.fc_final_shpcoring = &ov9718_aliso_fc_final_shpcoring;
				fcc.fc_final_shpcoring_idx_scale = &ov9718_aliso_fc_final_shpcoring_idx_scale;
				fcc.fc_final_shpcoring_min = &ov9718_aliso_fc_final_shpcoring_min;
				fcc.fc_final_shpcoring_scale_coring = &ov9718_aliso_fc_final_shpcoring_scale_coring;
				fcc.fc_wide_chroma_filter =&ov9718_aliso_fc_wide_chroma_filter;
				fcc.fc_wide_chroma_filter_combine =&ov9718_aliso_fc_wide_chroma_filter_combine;
				fcc.fc_video_mctf = &ov9718_aliso_fc_video_mctf;
				fcc.fc_video_mctf_temporal_adjust =&ov9718_aliso_fc_video_mctf_temporal_adjust;
				}
			else if(isp_pipeline ==ISP_PIPELINE_MID_LISO){
				fcc.fc_ae_target = &ov9718_mliso_fc_ae_target;
				fcc.fc_wb_ratio = &ov9718_mliso_fc_wb_ratio;
				fcc.fc_blc = &ov9718_mliso_fc_blc;
				fcc.fc_antialiasing = &ov9718_mliso_fc_antialiasing;
				fcc.fc_grgbmismatch = &ov9718_mliso_fc_grgbmismatch;
				fcc.fc_dpc = &ov9718_mliso_fc_dpc;
				fcc.fc_cfanf_low = &ov9718_mliso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = NULL;
				fcc.fc_demosaic = &ov9718_mliso_fc_demosaic;
				fcc.fc_cc = &ov9718_mliso_fc_cc;
				fcc.fc_tone =&ov9718_mliso_fc_tone;
				fcc.fc_rgb2yuv = &ov9718_mliso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &ov9718_mliso_fc_chroma_scale;
				fcc.fc_chroma_median = &ov9718_mliso_fc_chroma_median;
				fcc.fc_chroma_nf = &ov9718_mliso_fc_chroma_nf;
				fcc.fc_cdnr = &ov9718_mliso_fc_cdnr;
				fcc.fc_1stmode_sel = &ov9718_mliso_fc_1stmode_sel;
				fcc.fc_asf = &ov9718_mliso_fc_asf;
				fcc.fc_1st_shpboth = &ov9718_mliso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &ov9718_mliso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &ov9718_mliso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &ov9718_mliso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &ov9718_mliso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &ov9718_mliso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &ov9718_mliso_fc_1st_shpcoring_scale_coring;
				fcc.fc_video_mctf = &ov9718_mliso_fc_video_mctf;
				fcc.fc_video_mctf_temporal_adjust =&ov9718_mliso_fc_video_mctf_temporal_adjust;
				}
			sprintf(sensor_name, "ov9718");
			break;
		case SENSOR_OV9710:
			printf("ov9715\n");
			if(isp_pipeline ==ISP_PIPELINE_LISO){
				fcc.fc_ae_target = &ov9715_liso_fc_ae_target;
				fcc.fc_wb_ratio = &ov9715_liso_fc_wb_ratio;
				fcc.fc_blc = &ov9715_liso_fc_blc;
				fcc.fc_antialiasing = &ov9715_liso_fc_antialiasing;
				fcc.fc_grgbmismatch = &ov9715_liso_fc_grgbmismatch;
				fcc.fc_dpc = &ov9715_liso_fc_dpc;
				fcc.fc_cfanf_low = &ov9715_liso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = NULL;
				fcc.fc_demosaic = &ov9715_liso_fc_demosaic;
				fcc.fc_cc = &ov9715_liso_fc_cc;
				fcc.fc_tone =&ov9715_liso_fc_tone;
				fcc.fc_rgb2yuv = &ov9715_liso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &ov9715_liso_fc_chroma_scale;
				fcc.fc_chroma_median = &ov9715_liso_fc_chroma_median;
				fcc.fc_chroma_nf = &ov9715_liso_fc_chroma_nf;
				fcc.fc_cdnr = &ov9715_liso_fc_cdnr;
				fcc.fc_1stmode_sel = &ov9715_liso_fc_1stmode_sel;
				fcc.fc_asf = &ov9715_liso_fc_asf;
				fcc.fc_1st_shpboth = &ov9715_liso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &ov9715_liso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &ov9715_liso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &ov9715_liso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &ov9715_liso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &ov9715_liso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &ov9715_liso_fc_1st_shpcoring_scale_coring;
				fcc.fc_video_mctf = &ov9715_liso_fc_video_mctf;
				}
			else if(isp_pipeline ==ISP_PIPELINE_ADV_LISO){
				fcc.fc_ae_target = &ov9715_aliso_fc_ae_target;
				fcc.fc_wb_ratio = &ov9715_aliso_fc_wb_ratio;
				fcc.fc_blc = &ov9715_aliso_fc_blc;
				fcc.fc_antialiasing = &ov9715_aliso_fc_antialiasing;
				fcc.fc_grgbmismatch = &ov9715_aliso_fc_grgbmismatch;
				fcc.fc_dpc = &ov9715_aliso_fc_dpc;
				fcc.fc_cfanf_low = &ov9715_aliso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = NULL;
				fcc.fc_demosaic = &ov9715_aliso_fc_demosaic;
				fcc.fc_cc = &ov9715_aliso_fc_cc;
				fcc.fc_tone =&ov9715_aliso_fc_tone;
				fcc.fc_rgb2yuv = &ov9715_aliso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &ov9715_aliso_fc_chroma_scale;
				fcc.fc_chroma_median = &ov9715_aliso_fc_chroma_median;
				fcc.fc_chroma_nf = &ov9715_aliso_fc_chroma_nf;
				fcc.fc_cdnr = &ov9715_aliso_fc_cdnr;
				fcc.fc_1stmode_sel = &ov9715_aliso_fc_1stmode_sel;
				fcc.fc_asf = &ov9715_aliso_fc_asf;
				fcc.fc_1st_shpboth = &ov9715_aliso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &ov9715_aliso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &ov9715_aliso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &ov9715_aliso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &ov9715_aliso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &ov9715_aliso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &ov9715_aliso_fc_1st_shpcoring_scale_coring;
				fcc.fc_final_shpboth = &ov9715_aliso_fc_final_shpboth;
				fcc.fc_final_shpnoise = &ov9715_aliso_fc_final_shpnoise;
				fcc.fc_final_shpfir = &ov9715_aliso_fc_final_shpfir;
				fcc.fc_final_shpcoring = &ov9715_aliso_fc_final_shpcoring;
				fcc.fc_final_shpcoring_idx_scale = &ov9715_aliso_fc_final_shpcoring_idx_scale;
				fcc.fc_final_shpcoring_min = &ov9715_aliso_fc_final_shpcoring_min;
				fcc.fc_final_shpcoring_scale_coring = &ov9715_aliso_fc_final_shpcoring_scale_coring;
				fcc.fc_wide_chroma_filter =&ov9715_aliso_fc_wide_chroma_filter;
				fcc.fc_wide_chroma_filter_combine =&ov9715_aliso_fc_wide_chroma_filter_combine;
				fcc.fc_video_mctf = &ov9715_aliso_fc_video_mctf;
				fcc.fc_video_mctf_temporal_adjust =&ov9715_aliso_fc_video_mctf_temporal_adjust;
				}
			else if(isp_pipeline ==ISP_PIPELINE_MID_LISO){
				fcc.fc_ae_target = &ov9715_mliso_fc_ae_target;
				fcc.fc_wb_ratio = &ov9715_mliso_fc_wb_ratio;
				fcc.fc_blc = &ov9715_mliso_fc_blc;
				fcc.fc_antialiasing = &ov9715_mliso_fc_antialiasing;
				fcc.fc_grgbmismatch = &ov9715_mliso_fc_grgbmismatch;
				fcc.fc_dpc = &ov9715_mliso_fc_dpc;
				fcc.fc_cfanf_low = &ov9715_mliso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = NULL;
				fcc.fc_demosaic = &ov9715_mliso_fc_demosaic;
				fcc.fc_cc = &ov9715_mliso_fc_cc;
				fcc.fc_tone = &ov9715_mliso_fc_tone;
				fcc.fc_rgb2yuv = &ov9715_mliso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &ov9715_mliso_fc_chroma_scale;
				fcc.fc_chroma_median = &ov9715_mliso_fc_chroma_median;
				fcc.fc_chroma_nf = &ov9715_mliso_fc_chroma_nf;
				fcc.fc_cdnr = &ov9715_mliso_fc_cdnr;
				fcc.fc_1stmode_sel = &ov9715_mliso_fc_1stmode_sel;
				fcc.fc_asf = &ov9715_mliso_fc_asf;
				fcc.fc_1st_shpboth = &ov9715_mliso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &ov9715_mliso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &ov9715_mliso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &ov9715_mliso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &ov9715_mliso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &ov9715_mliso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &ov9715_mliso_fc_1st_shpcoring_scale_coring;
				fcc.fc_video_mctf = &ov9715_mliso_fc_video_mctf;
				fcc.fc_video_mctf_temporal_adjust =&ov9715_mliso_fc_video_mctf_temporal_adjust;
				}
			sprintf(sensor_name, "ov9715");
			break;
		case SENSOR_OV9750:
			printf("ov9750\n");
			if(isp_pipeline ==ISP_PIPELINE_LISO){
				fcc.fc_ae_target = &ov9750_liso_fc_ae_target;
				fcc.fc_wb_ratio = &ov9750_liso_fc_wb_ratio;
				fcc.fc_blc = &ov9750_liso_fc_blc;
				fcc.fc_antialiasing = &ov9750_liso_fc_antialiasing;
				fcc.fc_grgbmismatch = &ov9750_liso_fc_grgbmismatch;
				fcc.fc_dpc = &ov9750_liso_fc_dpc;
				fcc.fc_cfanf_low = &ov9750_liso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = NULL;
				fcc.fc_demosaic = &ov9750_liso_fc_demosaic;
				fcc.fc_cc = &ov9750_liso_fc_cc;
				fcc.fc_tone =&ov9750_liso_fc_tone;
				fcc.fc_rgb2yuv = &ov9750_liso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &ov9750_liso_fc_chroma_scale;
				fcc.fc_chroma_median = &ov9750_liso_fc_chroma_median;
				fcc.fc_chroma_nf = &ov9750_liso_fc_chroma_nf;
				fcc.fc_cdnr = &ov9750_liso_fc_cdnr;
				fcc.fc_1stmode_sel = &ov9750_liso_fc_1stmode_sel;
				fcc.fc_asf = &ov9750_liso_fc_asf;
				fcc.fc_1st_shpboth = &ov9750_liso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &ov9750_liso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &ov9750_liso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &ov9750_liso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &ov9750_liso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &ov9750_liso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &ov9750_liso_fc_1st_shpcoring_scale_coring;
				fcc.fc_video_mctf = &ov9750_liso_fc_video_mctf;
				}
			else if(isp_pipeline ==ISP_PIPELINE_ADV_LISO){
				fcc.fc_ae_target = &ov9750_aliso_fc_ae_target;
				fcc.fc_wb_ratio = &ov9750_aliso_fc_wb_ratio;
				fcc.fc_blc = &ov9750_aliso_fc_blc;
				fcc.fc_antialiasing = &ov9750_aliso_fc_antialiasing;
				fcc.fc_grgbmismatch = &ov9750_aliso_fc_grgbmismatch;
				fcc.fc_dpc = &ov9750_aliso_fc_dpc;
				fcc.fc_cfanf_low = &ov9750_aliso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = NULL;
				fcc.fc_demosaic = &ov9750_aliso_fc_demosaic;
				fcc.fc_cc = &ov9750_aliso_fc_cc;
				fcc.fc_tone =&ov9750_aliso_fc_tone;
				fcc.fc_rgb2yuv = &ov9750_aliso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &ov9750_aliso_fc_chroma_scale;
				fcc.fc_chroma_median = &ov9750_aliso_fc_chroma_median;
				fcc.fc_chroma_nf = &ov9750_aliso_fc_chroma_nf;
				fcc.fc_cdnr = &ov9750_aliso_fc_cdnr;
				fcc.fc_1stmode_sel = &ov9750_aliso_fc_1stmode_sel;
				fcc.fc_asf = &ov9750_aliso_fc_asf;
				fcc.fc_1st_shpboth = &ov9750_aliso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &ov9750_aliso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &ov9750_aliso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &ov9750_aliso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &ov9750_aliso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &ov9750_aliso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &ov9750_aliso_fc_1st_shpcoring_scale_coring;
				fcc.fc_final_shpboth = &ov9750_aliso_fc_final_shpboth;
				fcc.fc_final_shpnoise = &ov9750_aliso_fc_final_shpnoise;
				fcc.fc_final_shpfir = &ov9750_aliso_fc_final_shpfir;
				fcc.fc_final_shpcoring = &ov9750_aliso_fc_final_shpcoring;
				fcc.fc_final_shpcoring_idx_scale = &ov9750_aliso_fc_final_shpcoring_idx_scale;
				fcc.fc_final_shpcoring_min = &ov9750_aliso_fc_final_shpcoring_min;
				fcc.fc_final_shpcoring_scale_coring = &ov9750_aliso_fc_final_shpcoring_scale_coring;
				fcc.fc_wide_chroma_filter =&ov9750_aliso_fc_wide_chroma_filter;
				fcc.fc_wide_chroma_filter_combine =&ov9750_aliso_fc_wide_chroma_filter_combine;
				fcc.fc_video_mctf = &ov9750_aliso_fc_video_mctf;
				fcc.fc_video_mctf_temporal_adjust =&ov9750_aliso_fc_video_mctf_temporal_adjust;
				}
			else if(isp_pipeline ==ISP_PIPELINE_MID_LISO){
				fcc.fc_ae_target = &ov9750_mliso_fc_ae_target;
				fcc.fc_wb_ratio = &ov9750_mliso_fc_wb_ratio;
				fcc.fc_blc = &ov9750_mliso_fc_blc;
				fcc.fc_antialiasing = &ov9750_mliso_fc_antialiasing;
				fcc.fc_grgbmismatch = &ov9750_mliso_fc_grgbmismatch;
				fcc.fc_dpc = &ov9750_mliso_fc_dpc;
				fcc.fc_cfanf_low = &ov9750_mliso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = NULL;
				fcc.fc_demosaic = &ov9750_mliso_fc_demosaic;
				fcc.fc_cc = &ov9750_mliso_fc_cc;
				fcc.fc_tone = &ov9750_mliso_fc_tone;
				fcc.fc_rgb2yuv = &ov9750_mliso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &ov9750_mliso_fc_chroma_scale;
				fcc.fc_chroma_median = &ov9750_mliso_fc_chroma_median;
				fcc.fc_chroma_nf = &ov9750_mliso_fc_chroma_nf;
				fcc.fc_cdnr = &ov9750_mliso_fc_cdnr;
				fcc.fc_1stmode_sel = &ov9750_mliso_fc_1stmode_sel;
				fcc.fc_asf = &ov9750_mliso_fc_asf;
				fcc.fc_1st_shpboth = &ov9750_mliso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &ov9750_mliso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &ov9750_mliso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &ov9750_mliso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &ov9750_mliso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &ov9750_mliso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &ov9750_mliso_fc_1st_shpcoring_scale_coring;
				fcc.fc_video_mctf = &ov9750_mliso_fc_video_mctf;
				fcc.fc_video_mctf_temporal_adjust =&ov9750_mliso_fc_video_mctf_temporal_adjust;
				}
			sprintf(sensor_name, "ov9750");
			break;
		case SENSOR_OV4689:
			printf("ov4689\n");
			if(isp_pipeline ==ISP_PIPELINE_LISO){
				fcc.fc_ae_target = &ov4689_liso_fc_ae_target;
				fcc.fc_wb_ratio = &ov4689_liso_fc_wb_ratio;
				fcc.fc_blc = &ov4689_liso_fc_blc;
				fcc.fc_antialiasing = &ov4689_liso_fc_antialiasing;
				fcc.fc_grgbmismatch = &ov4689_liso_fc_grgbmismatch;
				fcc.fc_dpc = &ov4689_liso_fc_dpc;
				fcc.fc_cfanf_low = &ov4689_liso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = (hdr_pipeline == HDR_PIPELINE_OFF)? NULL : \
					((expo_num == HDR_2X)? &ov4689_liso_fc_2x_hdr_le : &ov4689_liso_fc_3x_hdr_le);
				fcc.fc_demosaic = &ov4689_liso_fc_demosaic;
				fcc.fc_cc = &ov4689_liso_fc_cc;
				fcc.fc_tone = (hdr_pipeline == HDR_PIPELINE_OFF)? &ov4689_liso_fc_tone : \
					((expo_num == HDR_2X)? &ov4689_liso_fc_2x_hdr_tone : &ov4689_liso_fc_3x_hdr_tone);
				fcc.fc_rgb2yuv = &ov4689_liso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &ov4689_liso_fc_chroma_scale;
				fcc.fc_chroma_median = &ov4689_liso_fc_chroma_median;
				fcc.fc_chroma_nf = &ov4689_liso_fc_chroma_nf;
				fcc.fc_cdnr = &ov4689_liso_fc_cdnr;
				fcc.fc_1stmode_sel = &ov4689_liso_fc_1stmode_sel;
				fcc.fc_asf = &ov4689_liso_fc_asf;
				fcc.fc_1st_shpboth = &ov4689_liso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &ov4689_liso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &ov4689_liso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &ov4689_liso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &ov4689_liso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &ov4689_liso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &ov4689_liso_fc_1st_shpcoring_scale_coring;
				fcc.fc_video_mctf = &ov4689_liso_fc_video_mctf;
				fcc.fc_hdr_alpha = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &ov4689_liso_fc_hdr_alpha;
				fcc.fc_hdr_threshold = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &ov4689_liso_fc_hdr_threshold;
				fcc.fc_hdr_ce = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &ov4689_liso_fc_hdr_ce;
				}
			else if(isp_pipeline ==ISP_PIPELINE_ADV_LISO){
				fcc.fc_ae_target = &ov4689_aliso_fc_ae_target;
				fcc.fc_wb_ratio = &ov4689_aliso_fc_wb_ratio;
				fcc.fc_blc = &ov4689_aliso_fc_blc;
				fcc.fc_antialiasing = &ov4689_aliso_fc_antialiasing;
				fcc.fc_grgbmismatch = &ov4689_aliso_fc_grgbmismatch;
				fcc.fc_dpc = &ov4689_aliso_fc_dpc;
				fcc.fc_cfanf_low = &ov4689_aliso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = (hdr_pipeline == HDR_PIPELINE_OFF)? NULL : \
					((expo_num == HDR_2X)? &ov4689_aliso_fc_2x_hdr_le : &ov4689_aliso_fc_3x_hdr_le);
				fcc.fc_demosaic = &ov4689_aliso_fc_demosaic;
				fcc.fc_cc = &ov4689_aliso_fc_cc;
				fcc.fc_tone = (hdr_pipeline == HDR_PIPELINE_OFF)? &ov4689_aliso_fc_tone : \
					((expo_num == HDR_2X)? &ov4689_aliso_fc_2x_hdr_tone : &ov4689_aliso_fc_3x_hdr_tone);
				fcc.fc_rgb2yuv = &ov4689_aliso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &ov4689_aliso_fc_chroma_scale;
				fcc.fc_chroma_median = &ov4689_aliso_fc_chroma_median;
				fcc.fc_chroma_nf = &ov4689_aliso_fc_chroma_nf;
				fcc.fc_cdnr = &ov4689_aliso_fc_cdnr;
				fcc.fc_1stmode_sel = &ov4689_aliso_fc_1stmode_sel;
				fcc.fc_asf = &ov4689_aliso_fc_asf;
				fcc.fc_1st_shpboth = &ov4689_aliso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &ov4689_aliso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &ov4689_aliso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &ov4689_aliso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &ov4689_aliso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &ov4689_aliso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &ov4689_aliso_fc_1st_shpcoring_scale_coring;
				fcc.fc_final_shpboth = &ov4689_aliso_fc_final_shpboth;
				fcc.fc_final_shpnoise = &ov4689_aliso_fc_final_shpnoise;
				fcc.fc_final_shpfir = &ov4689_aliso_fc_final_shpfir;
				fcc.fc_final_shpcoring = &ov4689_aliso_fc_final_shpcoring;
				fcc.fc_final_shpcoring_idx_scale = &ov4689_aliso_fc_final_shpcoring_idx_scale;
				fcc.fc_final_shpcoring_min = &ov4689_aliso_fc_final_shpcoring_min;
				fcc.fc_final_shpcoring_scale_coring = &ov4689_aliso_fc_final_shpcoring_scale_coring;
				fcc.fc_wide_chroma_filter =&ov4689_aliso_fc_wide_chroma_filter;
				fcc.fc_wide_chroma_filter_combine =&ov4689_aliso_fc_wide_chroma_filter_combine;
				fcc.fc_video_mctf = &ov4689_aliso_fc_video_mctf;
				fcc.fc_video_mctf_temporal_adjust =&ov4689_aliso_fc_video_mctf_temporal_adjust;
				fcc.fc_hdr_alpha = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &ov4689_aliso_fc_hdr_alpha;
				fcc.fc_hdr_threshold = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &ov4689_aliso_fc_hdr_threshold;
				fcc.fc_hdr_ce = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &ov4689_aliso_fc_hdr_ce;
				}
			else if(isp_pipeline ==ISP_PIPELINE_MID_LISO){
				if(hdr_pipeline == HDR_PIPELINE_ADV)
				{
					fcc.fc_ae_target = &ov4689_mliso_fc_ae_target_hdr;
					fcc.fc_wb_ratio = &ov4689_mliso_fc_wb_ratio_hdr;
					fcc.fc_blc = &ov4689_mliso_fc_blc_hdr;
					fcc.fc_antialiasing = &ov4689_mliso_fc_antialiasing_hdr;
					fcc.fc_grgbmismatch = &ov4689_mliso_fc_grgbmismatch_hdr;
					fcc.fc_dpc = &ov4689_mliso_fc_dpc_hdr;
					fcc.fc_cfanf_low = &ov4689_mliso_fc_cfanf_hdr;
					fcc.fc_cfanf_high = NULL;
					fcc.fc_le = (hdr_pipeline == HDR_PIPELINE_OFF)? NULL : \
						((expo_num == HDR_2X)? &ov4689_mliso_fc_2x_hdr_le_hdr : &ov4689_mliso_fc_3x_hdr_le_hdr);
					fcc.fc_demosaic = &ov4689_mliso_fc_demosaic_hdr;
					fcc.fc_cc = &ov4689_mliso_fc_cc_hdr;
					fcc.fc_tone = (expo_num == HDR_2X)? \
						&ov4689_mliso_fc_2x_hdr_tone_hdr : &ov4689_mliso_fc_3x_hdr_tone_hdr;
					fcc.fc_rgb2yuv = &ov4689_mliso_fc_rgb2yuv_hdr;
					fcc.fc_chroma_scale = &ov4689_mliso_fc_chroma_scale_hdr;
					fcc.fc_chroma_median = &ov4689_mliso_fc_chroma_median_hdr;
					fcc.fc_chroma_nf = &ov4689_mliso_fc_chroma_nf_hdr;
					fcc.fc_cdnr = &ov4689_mliso_fc_cdnr_hdr;
					fcc.fc_1stmode_sel = &ov4689_mliso_fc_1stmode_sel_hdr;
					fcc.fc_asf = &ov4689_mliso_fc_asf_hdr;
					fcc.fc_1st_shpboth = &ov4689_mliso_fc_1st_shpboth_hdr;
					fcc.fc_1st_shpnoise = &ov4689_mliso_fc_1st_shpnoise_hdr;
					fcc.fc_1st_shpfir = &ov4689_mliso_fc_1st_shpfir_hdr;
					fcc.fc_1st_shpcoring = &ov4689_mliso_fc_1st_shpcoring_hdr;
					fcc.fc_1st_shpcoring_idx_scale = &ov4689_mliso_fc_1st_shpcoring_idx_scale_hdr;
					fcc.fc_1st_shpcoring_min = &ov4689_mliso_fc_1st_shpcoring_min_hdr;
					fcc.fc_1st_shpcoring_scale_coring = &ov4689_mliso_fc_1st_shpcoring_scale_coring_hdr;
					fcc.fc_video_mctf = &ov4689_mliso_fc_video_mctf_hdr;
					fcc.fc_video_mctf_temporal_adjust =&ov4689_mliso_fc_video_mctf_temporal_adjust_hdr;
					fcc.fc_hdr_alpha = &ov4689_mliso_fc_hdr_alpha_hdr;
					fcc.fc_hdr_threshold = &ov4689_mliso_fc_hdr_threshold_hdr;
					fcc.fc_hdr_ce = &ov4689_mliso_fc_hdr_ce_hdr;
					}
				else{
					fcc.fc_ae_target = &ov4689_mliso_fc_ae_target;
					fcc.fc_wb_ratio = &ov4689_mliso_fc_wb_ratio;
					fcc.fc_blc = &ov4689_mliso_fc_blc;
					fcc.fc_antialiasing = &ov4689_mliso_fc_antialiasing;
					fcc.fc_grgbmismatch = &ov4689_mliso_fc_grgbmismatch;
					fcc.fc_dpc = &ov4689_mliso_fc_dpc;
					fcc.fc_cfanf_low = &ov4689_mliso_fc_cfanf;
					fcc.fc_cfanf_high = NULL;
					fcc.fc_le = (hdr_pipeline == HDR_PIPELINE_OFF)? NULL : \
						((expo_num == HDR_2X)? &ov4689_mliso_fc_2x_hdr_le : &ov4689_mliso_fc_3x_hdr_le);
					fcc.fc_demosaic = &ov4689_mliso_fc_demosaic;
					fcc.fc_cc = &ov4689_mliso_fc_cc;
					fcc.fc_tone = (hdr_pipeline == HDR_PIPELINE_OFF)? &ov4689_mliso_fc_tone : \
						((expo_num == HDR_2X)? &ov4689_mliso_fc_2x_hdr_tone : &ov4689_mliso_fc_3x_hdr_tone);
					fcc.fc_rgb2yuv = &ov4689_mliso_fc_rgb2yuv;
					fcc.fc_chroma_scale = &ov4689_mliso_fc_chroma_scale;
					fcc.fc_chroma_median = &ov4689_mliso_fc_chroma_median;
					fcc.fc_chroma_nf = &ov4689_mliso_fc_chroma_nf;
					fcc.fc_cdnr = &ov4689_mliso_fc_cdnr;
					fcc.fc_1stmode_sel = &ov4689_mliso_fc_1stmode_sel;
					fcc.fc_asf = &ov4689_mliso_fc_asf;
					fcc.fc_1st_shpboth = &ov4689_mliso_fc_1st_shpboth;
					fcc.fc_1st_shpnoise = &ov4689_mliso_fc_1st_shpnoise;
					fcc.fc_1st_shpfir = &ov4689_mliso_fc_1st_shpfir;
					fcc.fc_1st_shpcoring = &ov4689_mliso_fc_1st_shpcoring;
					fcc.fc_1st_shpcoring_idx_scale = &ov4689_mliso_fc_1st_shpcoring_idx_scale;
					fcc.fc_1st_shpcoring_min = &ov4689_mliso_fc_1st_shpcoring_min;
					fcc.fc_1st_shpcoring_scale_coring = &ov4689_mliso_fc_1st_shpcoring_scale_coring;
					fcc.fc_video_mctf = &ov4689_mliso_fc_video_mctf;
					fcc.fc_video_mctf_temporal_adjust =&ov4689_mliso_fc_video_mctf_temporal_adjust;
					fcc.fc_hdr_alpha = \
						(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &ov4689_mliso_fc_hdr_alpha;
					fcc.fc_hdr_threshold = \
						(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &ov4689_mliso_fc_hdr_threshold;
					fcc.fc_hdr_ce = \
						(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &ov4689_mliso_fc_hdr_ce;
					}
				}
			sprintf(sensor_name, "ov4689");
			break;
		case SENSOR_OV5658:
			printf("ov5658\n");
			if(isp_pipeline ==ISP_PIPELINE_LISO){
				fcc.fc_ae_target = &ov5658_liso_fc_ae_target;
				fcc.fc_wb_ratio = &ov5658_liso_fc_wb_ratio;
				fcc.fc_blc = &ov5658_liso_fc_blc;
				fcc.fc_antialiasing = &ov5658_liso_fc_antialiasing;
				fcc.fc_grgbmismatch = &ov5658_liso_fc_grgbmismatch;
				fcc.fc_dpc = &ov5658_liso_fc_dpc;
				fcc.fc_cfanf_low = &ov5658_liso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = NULL;
				fcc.fc_demosaic = &ov5658_liso_fc_demosaic;
				fcc.fc_cc = &ov5658_liso_fc_cc;
				fcc.fc_tone =&ov5658_liso_fc_tone;
				fcc.fc_rgb2yuv = &ov5658_liso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &ov5658_liso_fc_chroma_scale;
				fcc.fc_chroma_median = &ov5658_liso_fc_chroma_median;
				fcc.fc_chroma_nf = &ov5658_liso_fc_chroma_nf;
				fcc.fc_cdnr = &ov5658_liso_fc_cdnr;
				fcc.fc_1stmode_sel = &ov5658_liso_fc_1stmode_sel;
				fcc.fc_asf = &ov5658_liso_fc_asf;
				fcc.fc_1st_shpboth = &ov5658_liso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &ov5658_liso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &ov5658_liso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &ov5658_liso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &ov5658_liso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &ov5658_liso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &ov5658_liso_fc_1st_shpcoring_scale_coring;
				fcc.fc_video_mctf = &ov5658_liso_fc_video_mctf;
				}
			else if(isp_pipeline ==ISP_PIPELINE_ADV_LISO){
				fcc.fc_ae_target = &ov5658_aliso_fc_ae_target;
				fcc.fc_wb_ratio = &ov5658_aliso_fc_wb_ratio;
				fcc.fc_blc = &ov5658_aliso_fc_blc;
				fcc.fc_antialiasing = &ov5658_aliso_fc_antialiasing;
				fcc.fc_grgbmismatch = &ov5658_aliso_fc_grgbmismatch;
				fcc.fc_dpc = &ov5658_aliso_fc_dpc;
				fcc.fc_cfanf_low = &ov5658_aliso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = NULL;
				fcc.fc_demosaic = &ov5658_aliso_fc_demosaic;
				fcc.fc_cc = &ov5658_aliso_fc_cc;
				fcc.fc_tone = &ov5658_aliso_fc_tone;
				fcc.fc_rgb2yuv = &ov5658_aliso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &ov5658_aliso_fc_chroma_scale;
				fcc.fc_chroma_median = &ov5658_aliso_fc_chroma_median;
				fcc.fc_chroma_nf = &ov5658_aliso_fc_chroma_nf;
				fcc.fc_cdnr = &ov5658_aliso_fc_cdnr;
				fcc.fc_1stmode_sel = &ov5658_aliso_fc_1stmode_sel;
				fcc.fc_asf = &ov5658_aliso_fc_asf;
				fcc.fc_1st_shpboth = &ov5658_aliso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &ov5658_aliso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &ov5658_aliso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &ov5658_aliso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &ov5658_aliso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &ov5658_aliso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &ov5658_aliso_fc_1st_shpcoring_scale_coring;
				fcc.fc_final_shpboth = &ov5658_aliso_fc_final_shpboth;
				fcc.fc_final_shpnoise = &ov5658_aliso_fc_final_shpnoise;
				fcc.fc_final_shpfir = &ov5658_aliso_fc_final_shpfir;
				fcc.fc_final_shpcoring = &ov5658_aliso_fc_final_shpcoring;
				fcc.fc_final_shpcoring_idx_scale = &ov5658_aliso_fc_final_shpcoring_idx_scale;
				fcc.fc_final_shpcoring_min = &ov5658_aliso_fc_final_shpcoring_min;
				fcc.fc_final_shpcoring_scale_coring = &ov5658_aliso_fc_final_shpcoring_scale_coring;
				fcc.fc_wide_chroma_filter =&ov5658_aliso_fc_wide_chroma_filter;
				fcc.fc_wide_chroma_filter_combine =&ov5658_aliso_fc_wide_chroma_filter_combine;
				fcc.fc_video_mctf = &ov5658_aliso_fc_video_mctf;
				fcc.fc_video_mctf_temporal_adjust =&ov5658_aliso_fc_video_mctf_temporal_adjust;
				}
			else if(isp_pipeline ==ISP_PIPELINE_MID_LISO){
				fcc.fc_ae_target = &ov5658_mliso_fc_ae_target;
				fcc.fc_wb_ratio = &ov5658_mliso_fc_wb_ratio;
				fcc.fc_blc = &ov5658_mliso_fc_blc;
				fcc.fc_antialiasing = &ov5658_mliso_fc_antialiasing;
				fcc.fc_grgbmismatch = &ov5658_mliso_fc_grgbmismatch;
				fcc.fc_dpc = &ov5658_mliso_fc_dpc;
				fcc.fc_cfanf_low = &ov5658_mliso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = NULL;
				fcc.fc_demosaic = &ov5658_mliso_fc_demosaic;
				fcc.fc_cc = &ov5658_mliso_fc_cc;
				fcc.fc_tone = &ov5658_mliso_fc_tone;
				fcc.fc_rgb2yuv = &ov5658_mliso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &ov5658_mliso_fc_chroma_scale;
				fcc.fc_chroma_median = &ov5658_mliso_fc_chroma_median;
				fcc.fc_chroma_nf = &ov5658_mliso_fc_chroma_nf;
				fcc.fc_cdnr = &ov5658_mliso_fc_cdnr;
				fcc.fc_1stmode_sel = &ov5658_mliso_fc_1stmode_sel;
				fcc.fc_asf = &ov5658_mliso_fc_asf;
				fcc.fc_1st_shpboth = &ov5658_mliso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &ov5658_mliso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &ov5658_mliso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &ov5658_mliso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &ov5658_mliso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &ov5658_mliso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &ov5658_mliso_fc_1st_shpcoring_scale_coring;
				fcc.fc_video_mctf = &ov5658_mliso_fc_video_mctf;
				fcc.fc_video_mctf_temporal_adjust =&ov5658_mliso_fc_video_mctf_temporal_adjust;
				}
			sprintf(sensor_name, "ov5658");
			break;
		case SENSOR_IMX123:
			printf("imx123\n");
			if(isp_pipeline ==ISP_PIPELINE_LISO){
				fcc.fc_ae_target = &imx123_liso_fc_ae_target;
				fcc.fc_wb_ratio = &imx123_liso_fc_wb_ratio;
				fcc.fc_blc = &imx123_liso_fc_blc;
				fcc.fc_antialiasing = &imx123_liso_fc_antialiasing;
				fcc.fc_grgbmismatch = &imx123_liso_fc_grgbmismatch;
				fcc.fc_dpc = &imx123_liso_fc_dpc;
				fcc.fc_cfanf_low = &imx123_liso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = (hdr_pipeline == HDR_PIPELINE_OFF)? NULL : \
					((expo_num == HDR_2X)? &imx123_liso_fc_2x_hdr_le : &imx123_liso_fc_3x_hdr_le);
				fcc.fc_demosaic = &imx123_liso_fc_demosaic;
				fcc.fc_cc = &imx123_liso_fc_cc;
				fcc.fc_tone = (hdr_pipeline == HDR_PIPELINE_OFF)? &imx123_liso_fc_tone : \
					((expo_num == HDR_2X)? &imx123_liso_fc_2x_hdr_tone : &imx123_liso_fc_3x_hdr_tone);
				fcc.fc_rgb2yuv = &imx123_liso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &imx123_liso_fc_chroma_scale;
				fcc.fc_chroma_median = &imx123_liso_fc_chroma_median;
				fcc.fc_chroma_nf = &imx123_liso_fc_chroma_nf;
				fcc.fc_cdnr = &imx123_liso_fc_cdnr;
				fcc.fc_1stmode_sel = &imx123_liso_fc_1stmode_sel;
				fcc.fc_asf = &imx123_liso_fc_asf;
				fcc.fc_1st_shpboth = &imx123_liso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &imx123_liso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &imx123_liso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &imx123_liso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &imx123_liso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &imx123_liso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &imx123_liso_fc_1st_shpcoring_scale_coring;
				fcc.fc_video_mctf = &imx123_liso_fc_video_mctf;
				fcc.fc_hdr_alpha =  \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &imx123_liso_fc_hdr_alpha;
				fcc.fc_hdr_threshold = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &imx123_liso_fc_hdr_threshold;
				fcc.fc_hdr_ce = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &imx123_liso_fc_hdr_ce;
			}
			else if(isp_pipeline ==ISP_PIPELINE_ADV_LISO){
				fcc.fc_ae_target = &imx123_aliso_fc_ae_target;
				fcc.fc_wb_ratio = &imx123_aliso_fc_wb_ratio;
				fcc.fc_blc = &imx123_aliso_fc_blc;
				fcc.fc_antialiasing = &imx123_aliso_fc_antialiasing;
				fcc.fc_grgbmismatch = &imx123_aliso_fc_grgbmismatch;
				fcc.fc_dpc = &imx123_aliso_fc_dpc;
				fcc.fc_cfanf_low = &imx123_aliso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = (hdr_pipeline == HDR_PIPELINE_OFF)? NULL : \
					((expo_num == HDR_2X)? &imx123_aliso_fc_2x_hdr_le : &imx123_aliso_fc_3x_hdr_le);
				fcc.fc_demosaic = &imx123_aliso_fc_demosaic;
				fcc.fc_cc = &imx123_aliso_fc_cc;
				fcc.fc_tone = (hdr_pipeline == HDR_PIPELINE_OFF)? &imx123_aliso_fc_tone : \
					((expo_num == HDR_2X)? &imx123_aliso_fc_2x_hdr_tone : &imx123_aliso_fc_3x_hdr_tone);
				fcc.fc_rgb2yuv = &imx123_aliso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &imx123_aliso_fc_chroma_scale;
				fcc.fc_chroma_median = &imx123_aliso_fc_chroma_median;
				fcc.fc_chroma_nf = &imx123_aliso_fc_chroma_nf;
				fcc.fc_cdnr = &imx123_aliso_fc_cdnr;
				fcc.fc_1stmode_sel = &imx123_aliso_fc_1stmode_sel;
				fcc.fc_asf = &imx123_aliso_fc_asf;
				fcc.fc_1st_shpboth = &imx123_aliso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &imx123_aliso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &imx123_aliso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &imx123_aliso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &imx123_aliso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &imx123_aliso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &imx123_aliso_fc_1st_shpcoring_scale_coring;
				fcc.fc_final_shpboth = &imx123_aliso_fc_final_shpboth;
				fcc.fc_final_shpnoise = &imx123_aliso_fc_final_shpnoise;
				fcc.fc_final_shpfir = &imx123_aliso_fc_final_shpfir;
				fcc.fc_final_shpcoring = &imx123_aliso_fc_final_shpcoring;
				fcc.fc_final_shpcoring_idx_scale = &imx123_aliso_fc_final_shpcoring_idx_scale;
				fcc.fc_final_shpcoring_min = &imx123_aliso_fc_final_shpcoring_min;
				fcc.fc_final_shpcoring_scale_coring = &imx123_aliso_fc_final_shpcoring_scale_coring;
				fcc.fc_wide_chroma_filter =&imx123_aliso_fc_wide_chroma_filter;
				fcc.fc_wide_chroma_filter_combine =&imx123_aliso_fc_wide_chroma_filter_combine;
				fcc.fc_video_mctf = &imx123_aliso_fc_video_mctf;
				fcc.fc_video_mctf_temporal_adjust =&imx123_aliso_fc_video_mctf_temporal_adjust;
				fcc.fc_hdr_alpha = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &imx123_aliso_fc_hdr_alpha;
				fcc.fc_hdr_threshold = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &imx123_aliso_fc_hdr_threshold;
				fcc.fc_hdr_ce = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &imx123_aliso_fc_hdr_ce;
			}
			else if(isp_pipeline ==ISP_PIPELINE_MID_LISO){
				if(hdr_pipeline == HDR_PIPELINE_ADV)
				{
					fcc.fc_ae_target = &imx123_mliso_fc_ae_target_hdr;
					fcc.fc_wb_ratio = &imx123_mliso_fc_wb_ratio_hdr;
					fcc.fc_blc = &imx123_mliso_fc_blc_hdr;
					fcc.fc_antialiasing = &imx123_mliso_fc_antialiasing_hdr;
					fcc.fc_grgbmismatch = &imx123_mliso_fc_grgbmismatch_hdr;
					fcc.fc_dpc = &imx123_mliso_fc_dpc_hdr;
					fcc.fc_cfanf_low = &imx123_mliso_fc_cfanf_hdr;
					fcc.fc_cfanf_high = NULL;
					fcc.fc_le = (hdr_pipeline == HDR_PIPELINE_OFF)? NULL : \
						((expo_num == HDR_2X)? &imx123_mliso_fc_2x_hdr_le_hdr : &imx123_mliso_fc_3x_hdr_le_hdr);
					fcc.fc_demosaic = &imx123_mliso_fc_demosaic_hdr;
					fcc.fc_cc = &imx123_mliso_fc_cc_hdr;
					fcc.fc_tone = (expo_num == HDR_2X)? \
						&imx123_mliso_fc_2x_hdr_tone_hdr : &imx123_mliso_fc_3x_hdr_tone_hdr;
					fcc.fc_rgb2yuv = &imx123_mliso_fc_rgb2yuv_hdr;
					fcc.fc_chroma_scale = &imx123_mliso_fc_chroma_scale_hdr;
					fcc.fc_chroma_median = &imx123_mliso_fc_chroma_median_hdr;
					fcc.fc_chroma_nf = &imx123_mliso_fc_chroma_nf_hdr;
					fcc.fc_cdnr = &imx123_mliso_fc_cdnr_hdr;
					fcc.fc_1stmode_sel = &imx123_mliso_fc_1stmode_sel_hdr;
					fcc.fc_asf = &imx123_mliso_fc_asf_hdr;
					fcc.fc_1st_shpboth = &imx123_mliso_fc_1st_shpboth_hdr;
					fcc.fc_1st_shpnoise = &imx123_mliso_fc_1st_shpnoise_hdr;
					fcc.fc_1st_shpfir = &imx123_mliso_fc_1st_shpfir_hdr;
					fcc.fc_1st_shpcoring = &imx123_mliso_fc_1st_shpcoring_hdr;
					fcc.fc_1st_shpcoring_idx_scale = &imx123_mliso_fc_1st_shpcoring_idx_scale_hdr;
					fcc.fc_1st_shpcoring_min = &imx123_mliso_fc_1st_shpcoring_min_hdr;
					fcc.fc_1st_shpcoring_scale_coring = &imx123_mliso_fc_1st_shpcoring_scale_coring_hdr;
					fcc.fc_video_mctf = &imx123_mliso_fc_video_mctf_hdr;
					fcc.fc_video_mctf_temporal_adjust =&imx123_mliso_fc_video_mctf_temporal_adjust_hdr;
					fcc.fc_hdr_alpha = &imx123_mliso_fc_hdr_alpha_hdr;
					fcc.fc_hdr_threshold = &imx123_mliso_fc_hdr_threshold_hdr;
					fcc.fc_hdr_ce = &imx123_mliso_fc_hdr_ce_hdr;
					}
				else{
					fcc.fc_ae_target = &imx123_mliso_fc_ae_target;
					fcc.fc_wb_ratio = &imx123_mliso_fc_wb_ratio;
					fcc.fc_blc = &imx123_mliso_fc_blc;
					fcc.fc_antialiasing = &imx123_mliso_fc_antialiasing;
					fcc.fc_grgbmismatch = &imx123_mliso_fc_grgbmismatch;
					fcc.fc_dpc = &imx123_mliso_fc_dpc;
					fcc.fc_cfanf_low = &imx123_mliso_fc_cfanf;
					fcc.fc_cfanf_high = NULL;
					fcc.fc_le = (hdr_pipeline == HDR_PIPELINE_OFF)? NULL : \
						((expo_num == HDR_2X)? &imx123_mliso_fc_2x_hdr_le : &imx123_mliso_fc_3x_hdr_le);
					fcc.fc_demosaic = &imx123_mliso_fc_demosaic;
					fcc.fc_cc = &imx123_mliso_fc_cc;
					fcc.fc_tone = (hdr_pipeline == HDR_PIPELINE_OFF)? &imx123_mliso_fc_tone : \
						((expo_num == HDR_2X)? &imx123_mliso_fc_2x_hdr_tone : &imx123_mliso_fc_3x_hdr_tone);
					fcc.fc_rgb2yuv = &imx123_mliso_fc_rgb2yuv;
					fcc.fc_chroma_scale = &imx123_mliso_fc_chroma_scale;
					fcc.fc_chroma_median = &imx123_mliso_fc_chroma_median;
					fcc.fc_chroma_nf = &imx123_mliso_fc_chroma_nf;
					fcc.fc_cdnr = &imx123_mliso_fc_cdnr;
					fcc.fc_1stmode_sel = &imx123_mliso_fc_1stmode_sel;
					fcc.fc_asf = &imx123_mliso_fc_asf;
					fcc.fc_1st_shpboth = &imx123_mliso_fc_1st_shpboth;
					fcc.fc_1st_shpnoise = &imx123_mliso_fc_1st_shpnoise;
					fcc.fc_1st_shpfir = &imx123_mliso_fc_1st_shpfir;
					fcc.fc_1st_shpcoring = &imx123_mliso_fc_1st_shpcoring;
					fcc.fc_1st_shpcoring_idx_scale = &imx123_mliso_fc_1st_shpcoring_idx_scale;
					fcc.fc_1st_shpcoring_min = &imx123_mliso_fc_1st_shpcoring_min;
					fcc.fc_1st_shpcoring_scale_coring = &imx123_mliso_fc_1st_shpcoring_scale_coring;
					fcc.fc_video_mctf = &imx123_mliso_fc_video_mctf;
					fcc.fc_video_mctf_temporal_adjust =&imx123_mliso_fc_video_mctf_temporal_adjust;
					fcc.fc_hdr_alpha = \
						(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &imx123_mliso_fc_hdr_alpha;
					fcc.fc_hdr_threshold = \
						(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &imx123_mliso_fc_hdr_threshold;
					fcc.fc_hdr_ce = \
						(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &imx123_mliso_fc_hdr_ce;
					}
				}
			sprintf(sensor_name, "imx123");
			break;
		case SENSOR_IMX124:
			printf("imx124\n");
			if(isp_pipeline ==ISP_PIPELINE_LISO){
				fcc.fc_ae_target = &imx124_liso_fc_ae_target;
				fcc.fc_wb_ratio = &imx124_liso_fc_wb_ratio;
				fcc.fc_blc = &imx124_liso_fc_blc;
				fcc.fc_antialiasing = &imx124_liso_fc_antialiasing;
				fcc.fc_grgbmismatch = &imx124_liso_fc_grgbmismatch;
				fcc.fc_dpc = &imx124_liso_fc_dpc;
				fcc.fc_cfanf_low = &imx124_liso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = NULL;
				fcc.fc_demosaic = &imx124_liso_fc_demosaic;
				fcc.fc_cc = &imx124_liso_fc_cc;
				fcc.fc_tone =&imx124_liso_fc_tone;
				fcc.fc_rgb2yuv = &imx124_liso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &imx124_liso_fc_chroma_scale;
				fcc.fc_chroma_median = &imx124_liso_fc_chroma_median;
				fcc.fc_chroma_nf = &imx124_liso_fc_chroma_nf;
				fcc.fc_cdnr = &imx124_liso_fc_cdnr;
				fcc.fc_1stmode_sel = &imx124_liso_fc_1stmode_sel;
				fcc.fc_asf = &imx124_liso_fc_asf;
				fcc.fc_1st_shpboth = &imx124_liso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &imx124_liso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &imx124_liso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &imx124_liso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &imx124_liso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &imx124_liso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &imx124_liso_fc_1st_shpcoring_scale_coring;
				fcc.fc_video_mctf = &imx124_liso_fc_video_mctf;
				}
			else if(isp_pipeline ==ISP_PIPELINE_ADV_LISO){
				fcc.fc_ae_target = &imx124_aliso_fc_ae_target;
				fcc.fc_wb_ratio = &imx124_aliso_fc_wb_ratio;
				fcc.fc_blc = &imx124_aliso_fc_blc;
				fcc.fc_antialiasing = &imx124_aliso_fc_antialiasing;
				fcc.fc_grgbmismatch = &imx124_aliso_fc_grgbmismatch;
				fcc.fc_dpc = &imx124_aliso_fc_dpc;
				fcc.fc_cfanf_low = &imx124_aliso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = NULL;
				fcc.fc_demosaic = &imx124_aliso_fc_demosaic;
				fcc.fc_cc = &imx124_aliso_fc_cc;
				fcc.fc_tone = &imx124_aliso_fc_tone;
				fcc.fc_rgb2yuv = &imx124_aliso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &imx124_aliso_fc_chroma_scale;
				fcc.fc_chroma_median = &imx124_aliso_fc_chroma_median;
				fcc.fc_chroma_nf = &imx124_aliso_fc_chroma_nf;
				fcc.fc_cdnr = &imx124_aliso_fc_cdnr;
				fcc.fc_1stmode_sel = &imx124_aliso_fc_1stmode_sel;
				fcc.fc_asf = &imx124_aliso_fc_asf;
				fcc.fc_1st_shpboth = &imx124_aliso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &imx124_aliso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &imx124_aliso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &imx124_aliso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &imx124_aliso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &imx124_aliso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &imx124_aliso_fc_1st_shpcoring_scale_coring;
				fcc.fc_final_shpboth = &imx124_aliso_fc_final_shpboth;
				fcc.fc_final_shpnoise = &imx124_aliso_fc_final_shpnoise;
				fcc.fc_final_shpfir = &imx124_aliso_fc_final_shpfir;
				fcc.fc_final_shpcoring = &imx124_aliso_fc_final_shpcoring;
				fcc.fc_final_shpcoring_idx_scale = &imx124_aliso_fc_final_shpcoring_idx_scale;
				fcc.fc_final_shpcoring_min = &imx124_aliso_fc_final_shpcoring_min;
				fcc.fc_final_shpcoring_scale_coring = &imx124_aliso_fc_final_shpcoring_scale_coring;
				fcc.fc_wide_chroma_filter =&imx124_aliso_fc_wide_chroma_filter;
				fcc.fc_wide_chroma_filter_combine =&imx124_aliso_fc_wide_chroma_filter_combine;
				fcc.fc_video_mctf = &imx124_aliso_fc_video_mctf;
				fcc.fc_video_mctf_temporal_adjust =&imx124_aliso_fc_video_mctf_temporal_adjust;
				}
			else if(isp_pipeline ==ISP_PIPELINE_MID_LISO){
				fcc.fc_ae_target = &imx124_mliso_fc_ae_target;
				fcc.fc_wb_ratio = &imx124_mliso_fc_wb_ratio;
				fcc.fc_blc = &imx124_mliso_fc_blc;
				fcc.fc_antialiasing = &imx124_mliso_fc_antialiasing;
				fcc.fc_grgbmismatch = &imx124_mliso_fc_grgbmismatch;
				fcc.fc_dpc = &imx124_mliso_fc_dpc;
				fcc.fc_cfanf_low = &imx124_mliso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = NULL;
				fcc.fc_demosaic = &imx124_mliso_fc_demosaic;
				fcc.fc_cc = &imx124_mliso_fc_cc;
				fcc.fc_tone = &imx124_mliso_fc_tone;
				fcc.fc_rgb2yuv = &imx124_mliso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &imx124_mliso_fc_chroma_scale;
				fcc.fc_chroma_median = &imx124_mliso_fc_chroma_median;
				fcc.fc_chroma_nf = &imx124_mliso_fc_chroma_nf;
				fcc.fc_cdnr = &imx124_mliso_fc_cdnr;
				fcc.fc_1stmode_sel = &imx124_mliso_fc_1stmode_sel;
				fcc.fc_asf = &imx124_mliso_fc_asf;
				fcc.fc_1st_shpboth = &imx124_mliso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &imx124_mliso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &imx124_mliso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &imx124_mliso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &imx124_mliso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &imx124_mliso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &imx124_mliso_fc_1st_shpcoring_scale_coring;
				fcc.fc_video_mctf = &imx124_mliso_fc_video_mctf;
				fcc.fc_video_mctf_temporal_adjust =&imx124_mliso_fc_video_mctf_temporal_adjust;
				}
			sprintf(sensor_name, "imx124");
			break;
		case SENSOR_IMX104:
			printf("imx104\n");
			 if(isp_pipeline ==ISP_PIPELINE_MID_LISO){
				fcc.fc_ae_target = &imx104_mliso_fc_ae_target;
				fcc.fc_wb_ratio = &imx104_mliso_fc_wb_ratio;
				fcc.fc_blc = &imx104_mliso_fc_blc;
				fcc.fc_antialiasing = &imx104_mliso_fc_antialiasing;
				fcc.fc_grgbmismatch = &imx104_mliso_fc_grgbmismatch;
				fcc.fc_dpc = &imx104_mliso_fc_dpc;
				fcc.fc_cfanf_low = &imx104_mliso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = NULL;
				fcc.fc_demosaic = &imx104_mliso_fc_demosaic;
				fcc.fc_cc = &imx104_mliso_fc_cc;
				fcc.fc_tone = &imx104_mliso_fc_tone;
				fcc.fc_rgb2yuv = &imx104_mliso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &imx104_mliso_fc_chroma_scale;
				fcc.fc_chroma_median = &imx104_mliso_fc_chroma_median;
				fcc.fc_chroma_nf = &imx104_mliso_fc_chroma_nf;
				fcc.fc_cdnr = &imx104_mliso_fc_cdnr;
				fcc.fc_1stmode_sel = &imx104_mliso_fc_1stmode_sel;
				fcc.fc_asf = &imx104_mliso_fc_asf;
				fcc.fc_1st_shpboth = &imx104_mliso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &imx104_mliso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &imx104_mliso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &imx104_mliso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &imx104_mliso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &imx104_mliso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &imx104_mliso_fc_1st_shpcoring_scale_coring;
				fcc.fc_video_mctf = &imx104_mliso_fc_video_mctf;
				fcc.fc_video_mctf_temporal_adjust =&imx104_mliso_fc_video_mctf_temporal_adjust;
				}
			sprintf(sensor_name, "imx104");
			break;
		case SENSOR_IMX123_DCG:
			printf("imx123dcg\n");
			if(isp_pipeline ==ISP_PIPELINE_LISO){
				fcc.fc_ae_target = &imx123dcg_liso_fc_ae_target;
				fcc.fc_wb_ratio = &imx123dcg_liso_fc_wb_ratio;
				fcc.fc_blc = &imx123dcg_liso_fc_blc;
				fcc.fc_antialiasing = &imx123dcg_liso_fc_antialiasing;
				fcc.fc_grgbmismatch = &imx123dcg_liso_fc_grgbmismatch;
				fcc.fc_dpc = &imx123dcg_liso_fc_dpc;
				fcc.fc_cfanf_low = &imx123dcg_liso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = (hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &imx123dcg_liso_fc_2x_hdr_le;
				fcc.fc_demosaic = &imx123dcg_liso_fc_demosaic;
				fcc.fc_cc = &imx123dcg_liso_fc_cc;
				fcc.fc_tone = (hdr_pipeline == HDR_PIPELINE_OFF)? \
						&imx123dcg_liso_fc_tone : &imx123dcg_liso_fc_2x_hdr_tone;
				fcc.fc_rgb2yuv = &imx123dcg_liso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &imx123dcg_liso_fc_chroma_scale;
				fcc.fc_chroma_median = &imx123dcg_liso_fc_chroma_median;
				fcc.fc_chroma_nf = &imx123dcg_liso_fc_chroma_nf;
				fcc.fc_cdnr = &imx123dcg_liso_fc_cdnr;
				fcc.fc_1stmode_sel = &imx123dcg_liso_fc_1stmode_sel;
				fcc.fc_asf = &imx123dcg_liso_fc_asf;
				fcc.fc_1st_shpboth = &imx123dcg_liso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &imx123dcg_liso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &imx123dcg_liso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &imx123dcg_liso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &imx123dcg_liso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &imx123dcg_liso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &imx123dcg_liso_fc_1st_shpcoring_scale_coring;
				fcc.fc_video_mctf = &imx123dcg_liso_fc_video_mctf;
				fcc.fc_hdr_alpha =  \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &imx123dcg_liso_fc_hdr_alpha;
				fcc.fc_hdr_threshold = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &imx123dcg_liso_fc_hdr_threshold;
				fcc.fc_hdr_ce = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &imx123dcg_liso_fc_hdr_ce;
			}
			else if(isp_pipeline ==ISP_PIPELINE_ADV_LISO){
				fcc.fc_ae_target = &imx123dcg_aliso_fc_ae_target;
				fcc.fc_wb_ratio = &imx123dcg_aliso_fc_wb_ratio;
				fcc.fc_blc = &imx123dcg_aliso_fc_blc;
				fcc.fc_antialiasing = &imx123dcg_aliso_fc_antialiasing;
				fcc.fc_grgbmismatch = &imx123dcg_aliso_fc_grgbmismatch;
				fcc.fc_dpc = &imx123dcg_aliso_fc_dpc;
				fcc.fc_cfanf_low = &imx123dcg_aliso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = (hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &imx123dcg_aliso_fc_2x_hdr_le;
				fcc.fc_demosaic = &imx123dcg_aliso_fc_demosaic;
				fcc.fc_cc = &imx123dcg_aliso_fc_cc;
				fcc.fc_tone = (hdr_pipeline == HDR_PIPELINE_OFF)? \
						&imx123dcg_aliso_fc_tone : &imx123dcg_aliso_fc_2x_hdr_tone;
				fcc.fc_rgb2yuv = &imx123dcg_aliso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &imx123dcg_aliso_fc_chroma_scale;
				fcc.fc_chroma_median = &imx123dcg_aliso_fc_chroma_median;
				fcc.fc_chroma_nf = &imx123dcg_aliso_fc_chroma_nf;
				fcc.fc_cdnr = &imx123dcg_aliso_fc_cdnr;
				fcc.fc_1stmode_sel = &imx123dcg_aliso_fc_1stmode_sel;
				fcc.fc_asf = &imx123dcg_aliso_fc_asf;
				fcc.fc_1st_shpboth = &imx123dcg_aliso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &imx123dcg_aliso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &imx123dcg_aliso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &imx123dcg_aliso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &imx123dcg_aliso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &imx123dcg_aliso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &imx123dcg_aliso_fc_1st_shpcoring_scale_coring;
				fcc.fc_final_shpboth = &imx123dcg_aliso_fc_final_shpboth;
				fcc.fc_final_shpnoise = &imx123dcg_aliso_fc_final_shpnoise;
				fcc.fc_final_shpfir = &imx123dcg_aliso_fc_final_shpfir;
				fcc.fc_final_shpcoring = &imx123dcg_aliso_fc_final_shpcoring;
				fcc.fc_final_shpcoring_idx_scale = &imx123dcg_aliso_fc_final_shpcoring_idx_scale;
				fcc.fc_final_shpcoring_min = &imx123dcg_aliso_fc_final_shpcoring_min;
				fcc.fc_final_shpcoring_scale_coring = &imx123dcg_aliso_fc_final_shpcoring_scale_coring;
				fcc.fc_wide_chroma_filter =&imx123dcg_aliso_fc_wide_chroma_filter;
				fcc.fc_wide_chroma_filter_combine =&imx123dcg_aliso_fc_wide_chroma_filter_combine;
				fcc.fc_video_mctf = &imx123dcg_aliso_fc_video_mctf;
				fcc.fc_video_mctf_temporal_adjust =&imx123dcg_aliso_fc_video_mctf_temporal_adjust;
				fcc.fc_hdr_alpha = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &imx123dcg_aliso_fc_hdr_alpha;
				fcc.fc_hdr_threshold = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &imx123dcg_aliso_fc_hdr_threshold;
				fcc.fc_hdr_ce = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &imx123dcg_aliso_fc_hdr_ce;
			}
			else if(isp_pipeline ==ISP_PIPELINE_MID_LISO){
				fcc.fc_ae_target = &imx123dcg_mliso_fc_ae_target;
				fcc.fc_wb_ratio = &imx123dcg_mliso_fc_wb_ratio;
				fcc.fc_blc = &imx123dcg_mliso_fc_blc;
				fcc.fc_antialiasing = &imx123dcg_mliso_fc_antialiasing;
				fcc.fc_grgbmismatch = &imx123dcg_mliso_fc_grgbmismatch;
				fcc.fc_dpc = &imx123dcg_mliso_fc_dpc;
				fcc.fc_cfanf_low = &imx123dcg_mliso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = (hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &imx123dcg_mliso_fc_2x_hdr_le;
				fcc.fc_demosaic = &imx123dcg_mliso_fc_demosaic;
				fcc.fc_cc = &imx123dcg_mliso_fc_cc;
				fcc.fc_tone = (hdr_pipeline == HDR_PIPELINE_OFF)? \
					&imx123dcg_mliso_fc_tone : &imx123dcg_mliso_fc_2x_hdr_tone;
				fcc.fc_rgb2yuv = &imx123dcg_mliso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &imx123dcg_mliso_fc_chroma_scale;
				fcc.fc_chroma_median = &imx123dcg_mliso_fc_chroma_median;
				fcc.fc_chroma_nf = &imx123dcg_mliso_fc_chroma_nf;
				fcc.fc_cdnr = &imx123dcg_mliso_fc_cdnr;
				fcc.fc_1stmode_sel = &imx123dcg_mliso_fc_1stmode_sel;
				fcc.fc_asf = &imx123dcg_mliso_fc_asf;
				fcc.fc_1st_shpboth = &imx123dcg_mliso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &imx123dcg_mliso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &imx123dcg_mliso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &imx123dcg_mliso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &imx123dcg_mliso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &imx123dcg_mliso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &imx123dcg_mliso_fc_1st_shpcoring_scale_coring;
				fcc.fc_video_mctf = &imx123dcg_mliso_fc_video_mctf;
				fcc.fc_video_mctf_temporal_adjust =&imx123dcg_mliso_fc_video_mctf_temporal_adjust;
				fcc.fc_hdr_alpha = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &imx123dcg_mliso_fc_hdr_alpha;
				fcc.fc_hdr_threshold = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &imx123dcg_mliso_fc_hdr_threshold;
				fcc.fc_hdr_ce = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &imx123dcg_mliso_fc_hdr_ce;
				}
			sprintf(sensor_name, "imx123dcg");
			break;
		case SENSOR_IMX178:
			printf("imx178\n");
			if(isp_pipeline ==ISP_PIPELINE_LISO){
				fcc.fc_ae_target = &imx178_liso_fc_ae_target;
				fcc.fc_wb_ratio = &imx178_liso_fc_wb_ratio;
				fcc.fc_blc = &imx178_liso_fc_blc;
				fcc.fc_antialiasing = &imx178_liso_fc_antialiasing;
				fcc.fc_grgbmismatch = &imx178_liso_fc_grgbmismatch;
				fcc.fc_dpc = &imx178_liso_fc_dpc;
				fcc.fc_cfanf_low = &imx178_liso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = NULL;
				fcc.fc_demosaic = &imx178_liso_fc_demosaic;
				fcc.fc_cc = &imx178_liso_fc_cc;
				fcc.fc_tone =&imx178_liso_fc_tone;
				fcc.fc_rgb2yuv = &imx178_liso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &imx178_liso_fc_chroma_scale;
				fcc.fc_chroma_median = &imx178_liso_fc_chroma_median;
				fcc.fc_chroma_nf = &imx178_liso_fc_chroma_nf;
				fcc.fc_cdnr = &imx178_liso_fc_cdnr;
				fcc.fc_1stmode_sel = &imx178_liso_fc_1stmode_sel;
				fcc.fc_asf = &imx178_liso_fc_asf;
				fcc.fc_1st_shpboth = &imx178_liso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &imx178_liso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &imx178_liso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &imx178_liso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &imx178_liso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &imx178_liso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &imx178_liso_fc_1st_shpcoring_scale_coring;
				fcc.fc_video_mctf = &imx178_liso_fc_video_mctf;
				}
			else if(isp_pipeline ==ISP_PIPELINE_ADV_LISO){
				fcc.fc_ae_target = &imx178_aliso_fc_ae_target;
				fcc.fc_wb_ratio = &imx178_aliso_fc_wb_ratio;
				fcc.fc_blc = &imx178_aliso_fc_blc;
				fcc.fc_antialiasing = &imx178_aliso_fc_antialiasing;
				fcc.fc_grgbmismatch = &imx178_aliso_fc_grgbmismatch;
				fcc.fc_dpc = &imx178_aliso_fc_dpc;
				fcc.fc_cfanf_low = &imx178_aliso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = NULL;
				fcc.fc_demosaic = &imx178_aliso_fc_demosaic;
				fcc.fc_cc = &imx178_aliso_fc_cc;
				fcc.fc_tone = &imx178_aliso_fc_tone;
				fcc.fc_rgb2yuv = &imx178_aliso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &imx178_aliso_fc_chroma_scale;
				fcc.fc_chroma_median = &imx178_aliso_fc_chroma_median;
				fcc.fc_chroma_nf = &imx178_aliso_fc_chroma_nf;
				fcc.fc_cdnr = &imx178_aliso_fc_cdnr;
				fcc.fc_1stmode_sel = &imx178_aliso_fc_1stmode_sel;
				fcc.fc_asf = &imx178_aliso_fc_asf;
				fcc.fc_1st_shpboth = &imx178_aliso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &imx178_aliso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &imx178_aliso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &imx178_aliso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &imx178_aliso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &imx178_aliso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &imx178_aliso_fc_1st_shpcoring_scale_coring;
				fcc.fc_final_shpboth = &imx178_aliso_fc_final_shpboth;
				fcc.fc_final_shpnoise = &imx178_aliso_fc_final_shpnoise;
				fcc.fc_final_shpfir = &imx178_aliso_fc_final_shpfir;
				fcc.fc_final_shpcoring = &imx178_aliso_fc_final_shpcoring;
				fcc.fc_final_shpcoring_idx_scale = &imx178_aliso_fc_final_shpcoring_idx_scale;
				fcc.fc_final_shpcoring_min = &imx178_aliso_fc_final_shpcoring_min;
				fcc.fc_final_shpcoring_scale_coring = &imx178_aliso_fc_final_shpcoring_scale_coring;
				fcc.fc_wide_chroma_filter =&imx178_aliso_fc_wide_chroma_filter;
				fcc.fc_wide_chroma_filter_combine =&imx178_aliso_fc_wide_chroma_filter_combine;
				fcc.fc_video_mctf = &imx178_aliso_fc_video_mctf;
				fcc.fc_video_mctf_temporal_adjust =&imx178_aliso_fc_video_mctf_temporal_adjust;
				}
			else if(isp_pipeline ==ISP_PIPELINE_MID_LISO){
				fcc.fc_ae_target = &imx178_mliso_fc_ae_target;
				fcc.fc_wb_ratio = &imx178_mliso_fc_wb_ratio;
				fcc.fc_blc = &imx178_mliso_fc_blc;
				fcc.fc_antialiasing = &imx178_mliso_fc_antialiasing;
				fcc.fc_grgbmismatch = &imx178_mliso_fc_grgbmismatch;
				fcc.fc_dpc = &imx178_mliso_fc_dpc;
				fcc.fc_cfanf_low = &imx178_mliso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = NULL;
				fcc.fc_demosaic = &imx178_mliso_fc_demosaic;
				fcc.fc_cc = &imx178_mliso_fc_cc;
				fcc.fc_tone =&imx178_mliso_fc_tone;
				fcc.fc_rgb2yuv = &imx178_mliso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &imx178_mliso_fc_chroma_scale;
				fcc.fc_chroma_median = &imx178_mliso_fc_chroma_median;
				fcc.fc_chroma_nf = &imx178_mliso_fc_chroma_nf;
				fcc.fc_cdnr = &imx178_mliso_fc_cdnr;
				fcc.fc_1stmode_sel = &imx178_mliso_fc_1stmode_sel;
				fcc.fc_asf = &imx178_mliso_fc_asf;
				fcc.fc_1st_shpboth = &imx178_mliso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &imx178_mliso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &imx178_mliso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &imx178_mliso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &imx178_mliso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &imx178_mliso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &imx178_mliso_fc_1st_shpcoring_scale_coring;
				fcc.fc_video_mctf = &imx178_mliso_fc_video_mctf;
				fcc.fc_video_mctf_temporal_adjust =&imx178_mliso_fc_video_mctf_temporal_adjust;
				}
			sprintf(sensor_name, "imx178");
			break;
		case SENSOR_IMX224:
			printf("imx224\n");
			if(isp_pipeline ==ISP_PIPELINE_LISO){
				fcc.fc_ae_target = &imx224_liso_fc_ae_target;
				fcc.fc_wb_ratio = &imx224_liso_fc_wb_ratio;
				fcc.fc_blc = &imx224_liso_fc_blc;
				fcc.fc_antialiasing = &imx224_liso_fc_antialiasing;
				fcc.fc_grgbmismatch = &imx224_liso_fc_grgbmismatch;
				fcc.fc_dpc = &imx224_liso_fc_dpc;
				fcc.fc_cfanf_low = &imx224_liso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = (hdr_pipeline == HDR_PIPELINE_OFF)? NULL : \
					((expo_num == HDR_2X)? &imx224_liso_fc_2x_hdr_le : &imx224_liso_fc_3x_hdr_le);
				fcc.fc_demosaic = &imx224_liso_fc_demosaic;
				fcc.fc_cc = &imx224_liso_fc_cc;
				fcc.fc_tone = (hdr_pipeline == HDR_PIPELINE_OFF)? &imx224_liso_fc_tone : \
					((expo_num == HDR_2X)? &imx224_liso_fc_2x_hdr_tone : &imx224_liso_fc_3x_hdr_tone);
				fcc.fc_rgb2yuv = &imx224_liso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &imx224_liso_fc_chroma_scale;
				fcc.fc_chroma_median = &imx224_liso_fc_chroma_median;
				fcc.fc_chroma_nf = &imx224_liso_fc_chroma_nf;
				fcc.fc_cdnr = &imx224_liso_fc_cdnr;
				fcc.fc_1stmode_sel = &imx224_liso_fc_1stmode_sel;
				fcc.fc_asf = &imx224_liso_fc_asf;
				fcc.fc_1st_shpboth = &imx224_liso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &imx224_liso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &imx224_liso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &imx224_liso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &imx224_liso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &imx224_liso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &imx224_liso_fc_1st_shpcoring_scale_coring;
				fcc.fc_video_mctf = &imx224_liso_fc_video_mctf;
				fcc.fc_hdr_alpha = (hdr_pipeline == HDR_PIPELINE_OFF)? \
					NULL : &imx224_liso_fc_hdr_alpha;
				fcc.fc_hdr_threshold = (hdr_pipeline == HDR_PIPELINE_OFF)? \
					NULL : &imx224_liso_fc_hdr_threshold;
				fcc.fc_hdr_ce = (hdr_pipeline == HDR_PIPELINE_OFF)? \
					NULL : &imx224_liso_fc_hdr_ce;
				}
			else if(isp_pipeline ==ISP_PIPELINE_ADV_LISO){
				fcc.fc_ae_target = &imx224_aliso_fc_ae_target;
				fcc.fc_wb_ratio = &imx224_aliso_fc_wb_ratio;
				fcc.fc_blc = &imx224_aliso_fc_blc;
				fcc.fc_antialiasing = &imx224_aliso_fc_antialiasing;
				fcc.fc_grgbmismatch = &imx224_aliso_fc_grgbmismatch;
				fcc.fc_dpc = &imx224_aliso_fc_dpc;
				fcc.fc_cfanf_low = &imx224_aliso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = (hdr_pipeline == HDR_PIPELINE_OFF)? NULL : \
					((expo_num == HDR_2X)? &imx224_aliso_fc_2x_hdr_le : &imx224_aliso_fc_3x_hdr_le);
				fcc.fc_demosaic = &imx224_aliso_fc_demosaic;
				fcc.fc_cc = &imx224_aliso_fc_cc;
				fcc.fc_tone = (hdr_pipeline == HDR_PIPELINE_OFF)? &imx224_aliso_fc_tone : \
					((expo_num == HDR_2X)? &imx224_aliso_fc_2x_hdr_tone : &imx224_aliso_fc_3x_hdr_tone);
				fcc.fc_rgb2yuv = &imx224_aliso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &imx224_aliso_fc_chroma_scale;
				fcc.fc_chroma_median = &imx224_aliso_fc_chroma_median;
				fcc.fc_chroma_nf = &imx224_aliso_fc_chroma_nf;
				fcc.fc_cdnr = &imx224_aliso_fc_cdnr;
				fcc.fc_1stmode_sel = &imx224_aliso_fc_1stmode_sel;
				fcc.fc_asf = &imx224_aliso_fc_asf;
				fcc.fc_1st_shpboth = &imx224_aliso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &imx224_aliso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &imx224_aliso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &imx224_aliso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &imx224_aliso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &imx224_aliso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &imx224_aliso_fc_1st_shpcoring_scale_coring;
				fcc.fc_final_shpboth = &imx224_aliso_fc_final_shpboth;
				fcc.fc_final_shpnoise = &imx224_aliso_fc_final_shpnoise;
				fcc.fc_final_shpfir = &imx224_aliso_fc_final_shpfir;
				fcc.fc_final_shpcoring = &imx224_aliso_fc_final_shpcoring;
				fcc.fc_final_shpcoring_idx_scale = &imx224_aliso_fc_final_shpcoring_idx_scale;
				fcc.fc_final_shpcoring_min = &imx224_aliso_fc_final_shpcoring_min;
				fcc.fc_final_shpcoring_scale_coring = &imx224_aliso_fc_final_shpcoring_scale_coring;
				fcc.fc_wide_chroma_filter =&imx224_aliso_fc_wide_chroma_filter;
				fcc.fc_wide_chroma_filter_combine =&imx224_aliso_fc_wide_chroma_filter_combine;
				fcc.fc_video_mctf = &imx224_aliso_fc_video_mctf;
				fcc.fc_video_mctf_temporal_adjust =&imx224_aliso_fc_video_mctf_temporal_adjust;
				fcc.fc_hdr_alpha = (hdr_pipeline == HDR_PIPELINE_OFF)? \
					NULL : &imx224_aliso_fc_hdr_alpha;
				fcc.fc_hdr_threshold = (hdr_pipeline == HDR_PIPELINE_OFF)? \
					NULL : &imx224_aliso_fc_hdr_threshold;
				fcc.fc_hdr_ce = (hdr_pipeline == HDR_PIPELINE_OFF)? \
					NULL : &imx224_aliso_fc_hdr_ce;
				}
			else if(isp_pipeline ==ISP_PIPELINE_MID_LISO){
				fcc.fc_ae_target = &imx224_mliso_fc_ae_target;
				fcc.fc_wb_ratio = &imx224_mliso_fc_wb_ratio;
				fcc.fc_blc = &imx224_mliso_fc_blc;
				fcc.fc_antialiasing = &imx224_mliso_fc_antialiasing;
				fcc.fc_grgbmismatch = &imx224_mliso_fc_grgbmismatch;
				fcc.fc_dpc = &imx224_mliso_fc_dpc;
				fcc.fc_cfanf_low = &imx224_mliso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = (hdr_pipeline == HDR_PIPELINE_OFF)? NULL : \
					((expo_num == HDR_2X)? &imx224_mliso_fc_2x_hdr_le : &imx224_mliso_fc_3x_hdr_le);
				fcc.fc_demosaic = &imx224_mliso_fc_demosaic;
				fcc.fc_cc = &imx224_mliso_fc_cc;
				fcc.fc_tone = (hdr_pipeline == HDR_PIPELINE_OFF)? &imx224_mliso_fc_tone : \
					((expo_num == HDR_2X)? &imx224_mliso_fc_2x_hdr_tone : &imx224_mliso_fc_3x_hdr_tone);
				fcc.fc_rgb2yuv = &imx224_mliso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &imx224_mliso_fc_chroma_scale;
				fcc.fc_chroma_median = &imx224_mliso_fc_chroma_median;
				fcc.fc_chroma_nf = &imx224_mliso_fc_chroma_nf;
				fcc.fc_cdnr = &imx224_mliso_fc_cdnr;
				fcc.fc_1stmode_sel = &imx224_mliso_fc_1stmode_sel;
				fcc.fc_asf = &imx224_mliso_fc_asf;
				fcc.fc_1st_shpboth = &imx224_mliso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &imx224_mliso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &imx224_mliso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &imx224_mliso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &imx224_mliso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &imx224_mliso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &imx224_mliso_fc_1st_shpcoring_scale_coring;
				fcc.fc_video_mctf = &imx224_mliso_fc_video_mctf;
				fcc.fc_video_mctf_temporal_adjust =&imx224_mliso_fc_video_mctf_temporal_adjust;
				fcc.fc_hdr_alpha = (hdr_pipeline == HDR_PIPELINE_OFF)? \
					NULL : &imx224_mliso_fc_hdr_alpha;
				fcc.fc_hdr_threshold = (hdr_pipeline == HDR_PIPELINE_OFF)? \
					NULL : &imx224_mliso_fc_hdr_threshold;
				fcc.fc_hdr_ce = (hdr_pipeline == HDR_PIPELINE_OFF)? \
					NULL : &imx224_mliso_fc_hdr_ce;
				}
			sprintf(sensor_name, "imx224");
			break;
		case SENSOR_IMX291:
			printf("imx291\n");
			if(isp_pipeline ==ISP_PIPELINE_LISO){
				fcc.fc_ae_target = &imx291_liso_fc_ae_target;
				fcc.fc_wb_ratio = &imx291_liso_fc_wb_ratio;
				fcc.fc_blc = &imx291_liso_fc_blc;
				fcc.fc_antialiasing = &imx291_liso_fc_antialiasing;
				fcc.fc_grgbmismatch = &imx291_liso_fc_grgbmismatch;
				fcc.fc_dpc = &imx291_liso_fc_dpc;
				fcc.fc_cfanf_low = &imx291_liso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = NULL;
				fcc.fc_demosaic = &imx291_liso_fc_demosaic;
				fcc.fc_cc = &imx291_liso_fc_cc;
				fcc.fc_tone =&imx291_liso_fc_tone;
				fcc.fc_rgb2yuv = &imx291_liso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &imx291_liso_fc_chroma_scale;
				fcc.fc_chroma_median = &imx291_liso_fc_chroma_median;
				fcc.fc_chroma_nf = &imx291_liso_fc_chroma_nf;
				fcc.fc_cdnr = &imx291_liso_fc_cdnr;
				fcc.fc_1stmode_sel = &imx291_liso_fc_1stmode_sel;
				fcc.fc_asf = &imx291_liso_fc_asf;
				fcc.fc_1st_shpboth = &imx291_liso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &imx291_liso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &imx291_liso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &imx291_liso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &imx291_liso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &imx291_liso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &imx291_liso_fc_1st_shpcoring_scale_coring;
				fcc.fc_video_mctf = &imx291_liso_fc_video_mctf;
				}
			else if(isp_pipeline ==ISP_PIPELINE_ADV_LISO){
				fcc.fc_ae_target = &imx291_aliso_fc_ae_target;
				fcc.fc_wb_ratio = &imx291_aliso_fc_wb_ratio;
				fcc.fc_blc = &imx291_aliso_fc_blc;
				fcc.fc_antialiasing = &imx291_aliso_fc_antialiasing;
				fcc.fc_grgbmismatch = &imx291_aliso_fc_grgbmismatch;
				fcc.fc_dpc = &imx291_aliso_fc_dpc;
				fcc.fc_cfanf_low = &imx291_aliso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = NULL;
				fcc.fc_demosaic = &imx291_aliso_fc_demosaic;
				fcc.fc_cc = &imx291_aliso_fc_cc;
				fcc.fc_tone = &imx291_aliso_fc_tone;
				fcc.fc_rgb2yuv = &imx291_aliso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &imx291_aliso_fc_chroma_scale;
				fcc.fc_chroma_median = &imx291_aliso_fc_chroma_median;
				fcc.fc_chroma_nf = &imx291_aliso_fc_chroma_nf;
				fcc.fc_cdnr = &imx291_aliso_fc_cdnr;
				fcc.fc_1stmode_sel = &imx291_aliso_fc_1stmode_sel;
				fcc.fc_asf = &imx291_aliso_fc_asf;
				fcc.fc_1st_shpboth = &imx291_aliso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &imx291_aliso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &imx291_aliso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &imx291_aliso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &imx291_aliso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &imx291_aliso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &imx291_aliso_fc_1st_shpcoring_scale_coring;
				fcc.fc_final_shpboth = &imx291_aliso_fc_final_shpboth;
				fcc.fc_final_shpnoise = &imx291_aliso_fc_final_shpnoise;
				fcc.fc_final_shpfir = &imx291_aliso_fc_final_shpfir;
				fcc.fc_final_shpcoring = &imx291_aliso_fc_final_shpcoring;
				fcc.fc_final_shpcoring_idx_scale = &imx291_aliso_fc_final_shpcoring_idx_scale;
				fcc.fc_final_shpcoring_min = &imx291_aliso_fc_final_shpcoring_min;
				fcc.fc_final_shpcoring_scale_coring = &imx291_aliso_fc_final_shpcoring_scale_coring;
				fcc.fc_wide_chroma_filter =&imx291_aliso_fc_wide_chroma_filter;
				fcc.fc_wide_chroma_filter_combine =&imx291_aliso_fc_wide_chroma_filter_combine;
				fcc.fc_video_mctf = &imx291_aliso_fc_video_mctf;
				fcc.fc_video_mctf_temporal_adjust =&imx291_aliso_fc_video_mctf_temporal_adjust;
				}
			else if(isp_pipeline ==ISP_PIPELINE_MID_LISO){
				fcc.fc_ae_target = &imx291_mliso_fc_ae_target;
				fcc.fc_wb_ratio = &imx291_mliso_fc_wb_ratio;
				fcc.fc_blc = &imx291_mliso_fc_blc;
				fcc.fc_antialiasing = &imx291_mliso_fc_antialiasing;
				fcc.fc_grgbmismatch = &imx291_mliso_fc_grgbmismatch;
				fcc.fc_dpc = &imx291_mliso_fc_dpc;
				fcc.fc_cfanf_low = &imx291_mliso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = NULL;
				fcc.fc_demosaic = &imx291_mliso_fc_demosaic;
				fcc.fc_cc = &imx291_mliso_fc_cc;
				fcc.fc_tone = &imx291_mliso_fc_tone;
				fcc.fc_rgb2yuv = &imx291_mliso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &imx291_mliso_fc_chroma_scale;
				fcc.fc_chroma_median = &imx291_mliso_fc_chroma_median;
				fcc.fc_chroma_nf = &imx291_mliso_fc_chroma_nf;
				fcc.fc_cdnr = &imx291_mliso_fc_cdnr;
				fcc.fc_1stmode_sel = &imx291_mliso_fc_1stmode_sel;
				fcc.fc_asf = &imx291_mliso_fc_asf;
				fcc.fc_1st_shpboth = &imx291_mliso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &imx291_mliso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &imx291_mliso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &imx291_mliso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &imx291_mliso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &imx291_mliso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &imx291_mliso_fc_1st_shpcoring_scale_coring;
				fcc.fc_video_mctf = &imx291_mliso_fc_video_mctf;
				fcc.fc_video_mctf_temporal_adjust =&imx291_mliso_fc_video_mctf_temporal_adjust;
				}
			sprintf(sensor_name, "imx291");
			break;
		case SENSOR_IMX290:
			printf("imx290\n");
			if(isp_pipeline ==ISP_PIPELINE_LISO){
				fcc.fc_ae_target = &imx290_liso_fc_ae_target;
				fcc.fc_wb_ratio = &imx290_liso_fc_wb_ratio;
				fcc.fc_blc = &imx290_liso_fc_blc;
				fcc.fc_antialiasing = &imx290_liso_fc_antialiasing;
				fcc.fc_grgbmismatch = &imx290_liso_fc_grgbmismatch;
				fcc.fc_dpc = &imx290_liso_fc_dpc;
				fcc.fc_cfanf_low = &imx290_liso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = (hdr_pipeline == HDR_PIPELINE_OFF)? NULL : \
					((expo_num == HDR_2X)? &imx290_liso_fc_2x_hdr_le : &imx290_liso_fc_3x_hdr_le);
				fcc.fc_demosaic = &imx290_liso_fc_demosaic;
				fcc.fc_cc = &imx290_liso_fc_cc;
				fcc.fc_tone = (hdr_pipeline == HDR_PIPELINE_OFF)? &imx290_liso_fc_tone : \
					((expo_num == HDR_2X)? &imx290_liso_fc_2x_hdr_tone : &imx290_liso_fc_3x_hdr_tone);
				fcc.fc_rgb2yuv = &imx290_liso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &imx290_liso_fc_chroma_scale;
				fcc.fc_chroma_median = &imx290_liso_fc_chroma_median;
				fcc.fc_chroma_nf = &imx290_liso_fc_chroma_nf;
				fcc.fc_cdnr = &imx290_liso_fc_cdnr;
				fcc.fc_1stmode_sel = &imx290_liso_fc_1stmode_sel;
				fcc.fc_asf = &imx290_liso_fc_asf;
				fcc.fc_1st_shpboth = &imx290_liso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &imx290_liso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &imx290_liso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &imx290_liso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &imx290_liso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &imx290_liso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &imx290_liso_fc_1st_shpcoring_scale_coring;
				fcc.fc_video_mctf = &imx290_liso_fc_video_mctf;
				fcc.fc_hdr_alpha = (hdr_pipeline == HDR_PIPELINE_OFF)? \
					NULL : &imx290_liso_fc_hdr_alpha;
				fcc.fc_hdr_threshold = (hdr_pipeline == HDR_PIPELINE_OFF)? \
					NULL : &imx290_liso_fc_hdr_threshold;
				fcc.fc_hdr_ce = (hdr_pipeline == HDR_PIPELINE_OFF)? \
					NULL : &imx290_liso_fc_hdr_ce;
				}
			else if(isp_pipeline ==ISP_PIPELINE_ADV_LISO){
				fcc.fc_ae_target = &imx290_aliso_fc_ae_target;
				fcc.fc_wb_ratio = &imx290_aliso_fc_wb_ratio;
				fcc.fc_blc = &imx290_aliso_fc_blc;
				fcc.fc_antialiasing = &imx290_aliso_fc_antialiasing;
				fcc.fc_grgbmismatch = &imx290_aliso_fc_grgbmismatch;
				fcc.fc_dpc = &imx290_aliso_fc_dpc;
				fcc.fc_cfanf_low = &imx290_aliso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = (hdr_pipeline == HDR_PIPELINE_OFF)? NULL : \
					((expo_num == HDR_2X)? &imx290_aliso_fc_2x_hdr_le : &imx290_aliso_fc_3x_hdr_le);
				fcc.fc_demosaic = &imx290_aliso_fc_demosaic;
				fcc.fc_cc = &imx290_aliso_fc_cc;
				fcc.fc_tone = (hdr_pipeline == HDR_PIPELINE_OFF)? &imx290_aliso_fc_tone : \
					((expo_num == HDR_2X)? &imx290_aliso_fc_2x_hdr_tone : &imx290_aliso_fc_3x_hdr_tone);
				fcc.fc_rgb2yuv = &imx290_aliso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &imx290_aliso_fc_chroma_scale;
				fcc.fc_chroma_median = &imx290_aliso_fc_chroma_median;
				fcc.fc_chroma_nf = &imx290_aliso_fc_chroma_nf;
				fcc.fc_cdnr = &imx290_aliso_fc_cdnr;
				fcc.fc_1stmode_sel = &imx290_aliso_fc_1stmode_sel;
				fcc.fc_asf = &imx290_aliso_fc_asf;
				fcc.fc_1st_shpboth = &imx290_aliso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &imx290_aliso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &imx290_aliso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &imx290_aliso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &imx290_aliso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &imx290_aliso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &imx290_aliso_fc_1st_shpcoring_scale_coring;
				fcc.fc_final_shpboth = &imx290_aliso_fc_final_shpboth;
				fcc.fc_final_shpnoise = &imx290_aliso_fc_final_shpnoise;
				fcc.fc_final_shpfir = &imx290_aliso_fc_final_shpfir;
				fcc.fc_final_shpcoring = &imx290_aliso_fc_final_shpcoring;
				fcc.fc_final_shpcoring_idx_scale = &imx290_aliso_fc_final_shpcoring_idx_scale;
				fcc.fc_final_shpcoring_min = &imx290_aliso_fc_final_shpcoring_min;
				fcc.fc_final_shpcoring_scale_coring = &imx290_aliso_fc_final_shpcoring_scale_coring;
				fcc.fc_wide_chroma_filter =&imx290_aliso_fc_wide_chroma_filter;
				fcc.fc_wide_chroma_filter_combine =&imx290_aliso_fc_wide_chroma_filter_combine;
				fcc.fc_video_mctf = &imx290_aliso_fc_video_mctf;
				fcc.fc_video_mctf_temporal_adjust =&imx290_aliso_fc_video_mctf_temporal_adjust;
				fcc.fc_hdr_alpha = (hdr_pipeline == HDR_PIPELINE_OFF)? \
					NULL : &imx290_aliso_fc_hdr_alpha;
				fcc.fc_hdr_threshold = (hdr_pipeline == HDR_PIPELINE_OFF)? \
					NULL : &imx290_aliso_fc_hdr_threshold;
				fcc.fc_hdr_ce = (hdr_pipeline == HDR_PIPELINE_OFF)? \
					NULL : &imx290_aliso_fc_hdr_ce;
				}
			else if(isp_pipeline ==ISP_PIPELINE_MID_LISO){
				fcc.fc_ae_target = &imx290_mliso_fc_ae_target;
				fcc.fc_wb_ratio = &imx290_mliso_fc_wb_ratio;
				fcc.fc_blc = &imx290_mliso_fc_blc;
				fcc.fc_antialiasing = &imx290_mliso_fc_antialiasing;
				fcc.fc_grgbmismatch = &imx290_mliso_fc_grgbmismatch;
				fcc.fc_dpc = &imx290_mliso_fc_dpc;
				fcc.fc_cfanf_low = &imx290_mliso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = (hdr_pipeline == HDR_PIPELINE_OFF)? NULL : \
					((expo_num == HDR_2X)? &imx290_mliso_fc_2x_hdr_le : &imx290_mliso_fc_3x_hdr_le);
				fcc.fc_demosaic = &imx290_mliso_fc_demosaic;
				fcc.fc_cc = &imx290_mliso_fc_cc;
				fcc.fc_tone = (hdr_pipeline == HDR_PIPELINE_OFF)? &imx290_mliso_fc_tone : \
					((expo_num == HDR_2X)? &imx290_mliso_fc_2x_hdr_tone : &imx290_mliso_fc_3x_hdr_tone);
				fcc.fc_rgb2yuv = &imx290_mliso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &imx290_mliso_fc_chroma_scale;
				fcc.fc_chroma_median = &imx290_mliso_fc_chroma_median;
				fcc.fc_chroma_nf = &imx290_mliso_fc_chroma_nf;
				fcc.fc_cdnr = &imx290_mliso_fc_cdnr;
				fcc.fc_1stmode_sel = &imx290_mliso_fc_1stmode_sel;
				fcc.fc_asf = &imx290_mliso_fc_asf;
				fcc.fc_1st_shpboth = &imx290_mliso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &imx290_mliso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &imx290_mliso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &imx290_mliso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &imx290_mliso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &imx290_mliso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &imx290_mliso_fc_1st_shpcoring_scale_coring;
				fcc.fc_video_mctf = &imx290_mliso_fc_video_mctf;
				fcc.fc_video_mctf_temporal_adjust =&imx290_mliso_fc_video_mctf_temporal_adjust;
				fcc.fc_hdr_alpha = (hdr_pipeline == HDR_PIPELINE_OFF)? \
					NULL : &imx290_mliso_fc_hdr_alpha;
				fcc.fc_hdr_threshold = (hdr_pipeline == HDR_PIPELINE_OFF)? \
					NULL : &imx290_mliso_fc_hdr_threshold;
				fcc.fc_hdr_ce = (hdr_pipeline == HDR_PIPELINE_OFF)? \
					NULL : &imx290_mliso_fc_hdr_ce;
				}
			else if(isp_pipeline ==ISP_PIPELINE_B_LISO){
				fcc.fc_ae_target = &imx290_bliso_fc_ae_target;
				fcc.fc_wb_ratio = &imx290_bliso_fc_wb_ratio;
				fcc.fc_blc = &imx290_bliso_fc_blc;
				fcc.fc_antialiasing = &imx290_bliso_fc_antialiasing;
				fcc.fc_grgbmismatch = &imx290_bliso_fc_grgbmismatch;
				fcc.fc_dpc = &imx290_bliso_fc_dpc;
				fcc.fc_cfanf_low = &imx290_bliso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = &imx290_bliso_le;
				fcc.fc_demosaic = &imx290_bliso_fc_demosaic;
				fcc.fc_cc = &imx290_bliso_fc_cc;
				fcc.fc_tone = (hdr_pipeline == HDR_PIPELINE_OFF)? &imx290_bliso_fc_tone : \
					((expo_num = HDR_2X)? &imx290_bliso_fc_2x_hdr_tone : &imx290_bliso_fc_3x_hdr_tone);
				fcc.fc_rgb2yuv = &imx290_bliso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &imx290_bliso_fc_chroma_scale;
				fcc.fc_chroma_median = &imx290_bliso_fc_chroma_median;
				fcc.fc_chroma_nf = &imx290_bliso_fc_chroma_nf;
				fcc.fc_1stmode_sel = &imx290_bliso_fc_1stmode_sel;
				fcc.fc_asf = &imx290_bliso_fc_asf;
				fcc.fc_1st_shpboth = &imx290_bliso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &imx290_bliso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &imx290_bliso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &imx290_bliso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &imx290_bliso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &imx290_bliso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &imx290_bliso_fc_1st_shpcoring_scale_coring;

				fcc.fc_video_mctf = &imx290_bliso_fc_video_mctf;
				fcc.fc_video_mctf_temporal_adjust =&imx290_bliso_fc_video_mctf_temporal_adjust;
				fcc.fc_hdr_alpha = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &imx290_bliso_fc_hdr_alpha;
				fcc.fc_hdr_threshold = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &imx290_bliso_fc_hdr_threshold;
				fcc.fc_hdr_ce = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &imx290_bliso_fc_hdr_ce;
				fcc.fc_mo_antialiasing = &imx290_bliso_fc_mo_antialiasing;
				fcc.fc_mo_grgbmismatch = &imx290_bliso_fc_mo_grgbmismatch;
				fcc.fc_mo_dpc = &imx290_bliso_fc_mo_dpc;
				fcc.fc_mo_cfanf = &imx290_bliso_fc_mo_cfanf;
				fcc.fc_mo_demosaic = &imx290_bliso_fc_mo_demosaic;
				fcc.fc_mo_chroma_median = &imx290_bliso_fc_mo_chroma_median;
				fcc.fc_mo_chroma_nf = &imx290_bliso_fc_mo_chroma_nf;
				fcc.fc_mo_1stmode_sel = &imx290_bliso_fc_mo_1stmode_sel;
				fcc.fc_mo_asf = &imx290_bliso_fc_mo_asf;
				fcc.fc_mo_1st_shpboth = &imx290_bliso_fc_mo_1st_shpboth;
				fcc.fc_mo_1st_shpnoise = &imx290_bliso_fc_mo_1st_shpnoise;
				fcc.fc_mo_1st_shpfir = &imx290_bliso_fc_mo_1st_shpfir;
				fcc.fc_mo_1st_shpcoring = &imx290_bliso_fc_mo_1st_shpcoring;
				fcc.fc_mo_1st_shpcoring_idx_scale = &imx290_bliso_fc_mo_1st_shpcoring_idx_scale;
				fcc.fc_mo_1st_shpcoring_min = &imx290_bliso_fc_mo_1st_shpcoring_min;
				fcc.fc_mo_1st_shpcoring_scale_coring = &imx290_bliso_fc_mo_1st_shpcoring_scale_coring;
				}
			sprintf(sensor_name, "imx290");
			break;
		case SENSOR_IMX322:
			printf("imx322\n");
			if(isp_pipeline ==ISP_PIPELINE_LISO){
				fcc.fc_ae_target = &imx322_liso_fc_ae_target;
				fcc.fc_wb_ratio = &imx322_liso_fc_wb_ratio;
				fcc.fc_blc = &imx322_liso_fc_blc;
				fcc.fc_antialiasing = &imx322_liso_fc_antialiasing;
				fcc.fc_grgbmismatch = &imx322_liso_fc_grgbmismatch;
				fcc.fc_dpc = &imx322_liso_fc_dpc;
				fcc.fc_cfanf_low = &imx322_liso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = NULL;
				fcc.fc_demosaic = &imx322_liso_fc_demosaic;
				fcc.fc_cc = &imx322_liso_fc_cc;
				fcc.fc_tone =&imx322_liso_fc_tone;
				fcc.fc_rgb2yuv = &imx322_liso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &imx322_liso_fc_chroma_scale;
				fcc.fc_chroma_median = &imx322_liso_fc_chroma_median;
				fcc.fc_chroma_nf = &imx322_liso_fc_chroma_nf;
				fcc.fc_cdnr = &imx322_liso_fc_cdnr;
				fcc.fc_1stmode_sel = &imx322_liso_fc_1stmode_sel;
				fcc.fc_asf = &imx322_liso_fc_asf;
				fcc.fc_1st_shpboth = &imx322_liso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &imx322_liso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &imx322_liso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &imx322_liso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &imx322_liso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &imx322_liso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &imx322_liso_fc_1st_shpcoring_scale_coring;
				fcc.fc_video_mctf = &imx322_liso_fc_video_mctf;
				}
			else if(isp_pipeline ==ISP_PIPELINE_ADV_LISO){
				fcc.fc_ae_target = &imx322_aliso_fc_ae_target;
				fcc.fc_wb_ratio = &imx322_aliso_fc_wb_ratio;
				fcc.fc_blc = &imx322_aliso_fc_blc;
				fcc.fc_antialiasing = &imx322_aliso_fc_antialiasing;
				fcc.fc_grgbmismatch = &imx322_aliso_fc_grgbmismatch;
				fcc.fc_dpc = &imx322_aliso_fc_dpc;
				fcc.fc_cfanf_low = &imx322_aliso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = NULL;
				fcc.fc_demosaic = &imx322_aliso_fc_demosaic;
				fcc.fc_cc = &imx322_aliso_fc_cc;
				fcc.fc_tone = &imx322_aliso_fc_tone;
				fcc.fc_rgb2yuv = &imx322_aliso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &imx322_aliso_fc_chroma_scale;
				fcc.fc_chroma_median = &imx322_aliso_fc_chroma_median;
				fcc.fc_chroma_nf = &imx322_aliso_fc_chroma_nf;
				fcc.fc_cdnr = &imx322_aliso_fc_cdnr;
				fcc.fc_1stmode_sel = &imx322_aliso_fc_1stmode_sel;
				fcc.fc_asf = &imx322_aliso_fc_asf;
				fcc.fc_1st_shpboth = &imx322_aliso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &imx322_aliso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &imx322_aliso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &imx322_aliso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &imx322_aliso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &imx322_aliso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &imx322_aliso_fc_1st_shpcoring_scale_coring;
				fcc.fc_final_shpboth = &imx322_aliso_fc_final_shpboth;
				fcc.fc_final_shpnoise = &imx322_aliso_fc_final_shpnoise;
				fcc.fc_final_shpfir = &imx322_aliso_fc_final_shpfir;
				fcc.fc_final_shpcoring = &imx322_aliso_fc_final_shpcoring;
				fcc.fc_final_shpcoring_idx_scale = &imx322_aliso_fc_final_shpcoring_idx_scale;
				fcc.fc_final_shpcoring_min = &imx322_aliso_fc_final_shpcoring_min;
				fcc.fc_final_shpcoring_scale_coring = &imx322_aliso_fc_final_shpcoring_scale_coring;
				fcc.fc_wide_chroma_filter =&imx322_aliso_fc_wide_chroma_filter;
				fcc.fc_wide_chroma_filter_combine =&imx322_aliso_fc_wide_chroma_filter_combine;
				fcc.fc_video_mctf = &imx322_aliso_fc_video_mctf;
				fcc.fc_video_mctf_temporal_adjust =&imx322_aliso_fc_video_mctf_temporal_adjust;
				}
			else if(isp_pipeline ==ISP_PIPELINE_MID_LISO){
				fcc.fc_ae_target = &imx322_mliso_fc_ae_target;
				fcc.fc_wb_ratio = &imx322_mliso_fc_wb_ratio;
				fcc.fc_blc = &imx322_mliso_fc_blc;
				fcc.fc_antialiasing = &imx322_mliso_fc_antialiasing;
				fcc.fc_grgbmismatch = &imx322_mliso_fc_grgbmismatch;
				fcc.fc_dpc = &imx322_mliso_fc_dpc;
				fcc.fc_cfanf_low = &imx322_mliso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = NULL;
				fcc.fc_demosaic = &imx322_mliso_fc_demosaic;
				fcc.fc_cc = &imx322_mliso_fc_cc;
				fcc.fc_tone = &imx322_mliso_fc_tone;
				fcc.fc_rgb2yuv = &imx322_mliso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &imx322_mliso_fc_chroma_scale;
				fcc.fc_chroma_median = &imx322_mliso_fc_chroma_median;
				fcc.fc_chroma_nf = &imx322_mliso_fc_chroma_nf;
				fcc.fc_cdnr = &imx322_mliso_fc_cdnr;
				fcc.fc_1stmode_sel = &imx322_mliso_fc_1stmode_sel;
				fcc.fc_asf = &imx322_mliso_fc_asf;
				fcc.fc_1st_shpboth = &imx322_mliso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &imx322_mliso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &imx322_mliso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &imx322_mliso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &imx322_mliso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &imx322_mliso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &imx322_mliso_fc_1st_shpcoring_scale_coring;
				fcc.fc_video_mctf = &imx322_mliso_fc_video_mctf;
				fcc.fc_video_mctf_temporal_adjust =&imx322_mliso_fc_video_mctf_temporal_adjust;
				}
			sprintf(sensor_name, "imx322");
			break;
		case SENSOR_AR0130:
			printf("ar0130\n");
			if(isp_pipeline ==ISP_PIPELINE_LISO){
				fcc.fc_ae_target = &ar0130_liso_fc_ae_target;
				fcc.fc_wb_ratio = &ar0130_liso_fc_wb_ratio;
				fcc.fc_blc = &ar0130_liso_fc_blc;
				fcc.fc_antialiasing = &ar0130_liso_fc_antialiasing;
				fcc.fc_grgbmismatch = &ar0130_liso_fc_grgbmismatch;
				fcc.fc_dpc = &ar0130_liso_fc_dpc;
				fcc.fc_cfanf_low = &ar0130_liso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = NULL;
				fcc.fc_demosaic = &ar0130_liso_fc_demosaic;
				fcc.fc_cc = &ar0130_liso_fc_cc;
				fcc.fc_tone = &ar0130_liso_fc_tone;
				fcc.fc_rgb2yuv = &ar0130_liso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &ar0130_liso_fc_chroma_scale;
				fcc.fc_chroma_median = &ar0130_liso_fc_chroma_median;
				fcc.fc_chroma_nf = &ar0130_liso_fc_chroma_nf;
				fcc.fc_cdnr = &ar0130_liso_fc_cdnr;
				fcc.fc_1stmode_sel = &ar0130_liso_fc_1stmode_sel;
				fcc.fc_asf = &ar0130_liso_fc_asf;
				fcc.fc_1st_shpboth = &ar0130_liso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &ar0130_liso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &ar0130_liso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &ar0130_liso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &ar0130_liso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &ar0130_liso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &ar0130_liso_fc_1st_shpcoring_scale_coring;
				fcc.fc_video_mctf = &ar0130_liso_fc_video_mctf;
				}
			else if(isp_pipeline ==ISP_PIPELINE_ADV_LISO){
				fcc.fc_ae_target = &ar0130_aliso_fc_ae_target;
				fcc.fc_wb_ratio = &ar0130_aliso_fc_wb_ratio;
				fcc.fc_blc = &ar0130_aliso_fc_blc;
				fcc.fc_antialiasing = &ar0130_aliso_fc_antialiasing;
				fcc.fc_grgbmismatch = &ar0130_aliso_fc_grgbmismatch;
				fcc.fc_dpc = &ar0130_aliso_fc_dpc;
				fcc.fc_cfanf_low = &ar0130_aliso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = NULL;
				fcc.fc_demosaic = &ar0130_aliso_fc_demosaic;
				fcc.fc_cc = &ar0130_aliso_fc_cc;
				fcc.fc_tone = &ar0130_aliso_fc_tone;
				fcc.fc_rgb2yuv = &ar0130_aliso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &ar0130_aliso_fc_chroma_scale;
				fcc.fc_chroma_median = &ar0130_aliso_fc_chroma_median;
				fcc.fc_chroma_nf = &ar0130_aliso_fc_chroma_nf;
				fcc.fc_cdnr = &ar0130_aliso_fc_cdnr;
				fcc.fc_1stmode_sel = &ar0130_aliso_fc_1stmode_sel;
				fcc.fc_asf = &ar0130_aliso_fc_asf;
				fcc.fc_1st_shpboth = &ar0130_aliso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &ar0130_aliso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &ar0130_aliso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &ar0130_aliso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &ar0130_aliso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &ar0130_aliso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &ar0130_aliso_fc_1st_shpcoring_scale_coring;
				fcc.fc_final_shpboth = &ar0130_aliso_fc_final_shpboth;
				fcc.fc_final_shpnoise = &ar0130_aliso_fc_final_shpnoise;
				fcc.fc_final_shpfir = &ar0130_aliso_fc_final_shpfir;
				fcc.fc_final_shpcoring = &ar0130_aliso_fc_final_shpcoring;
				fcc.fc_final_shpcoring_idx_scale = &ar0130_aliso_fc_final_shpcoring_idx_scale;
				fcc.fc_final_shpcoring_min = &ar0130_aliso_fc_final_shpcoring_min;
				fcc.fc_final_shpcoring_scale_coring = &ar0130_aliso_fc_final_shpcoring_scale_coring;
				fcc.fc_wide_chroma_filter =&ar0130_aliso_fc_wide_chroma_filter;
				fcc.fc_wide_chroma_filter_combine =&ar0130_aliso_fc_wide_chroma_filter_combine;
				fcc.fc_video_mctf = &ar0130_aliso_fc_video_mctf;
				fcc.fc_video_mctf_temporal_adjust =&ar0130_aliso_fc_video_mctf_temporal_adjust;
				}
			else if(isp_pipeline ==ISP_PIPELINE_MID_LISO){
				fcc.fc_ae_target = &ar0130_mliso_fc_ae_target;
				fcc.fc_wb_ratio = &ar0130_mliso_fc_wb_ratio;
				fcc.fc_blc = &ar0130_mliso_fc_blc;
				fcc.fc_antialiasing = &ar0130_mliso_fc_antialiasing;
				fcc.fc_grgbmismatch = &ar0130_mliso_fc_grgbmismatch;
				fcc.fc_dpc = &ar0130_mliso_fc_dpc;
				fcc.fc_cfanf_low = &ar0130_mliso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = NULL;
				fcc.fc_demosaic = &ar0130_mliso_fc_demosaic;
				fcc.fc_cc = &ar0130_mliso_fc_cc;
				fcc.fc_tone = &ar0130_mliso_fc_tone;
				fcc.fc_rgb2yuv = &ar0130_mliso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &ar0130_mliso_fc_chroma_scale;
				fcc.fc_chroma_median = &ar0130_mliso_fc_chroma_median;
				fcc.fc_chroma_nf = &ar0130_mliso_fc_chroma_nf;
				fcc.fc_cdnr = &ar0130_mliso_fc_cdnr;
				fcc.fc_1stmode_sel = &ar0130_mliso_fc_1stmode_sel;
				fcc.fc_asf = &ar0130_mliso_fc_asf;
				fcc.fc_1st_shpboth = &ar0130_mliso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &ar0130_mliso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &ar0130_mliso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &ar0130_mliso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &ar0130_mliso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &ar0130_mliso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &ar0130_mliso_fc_1st_shpcoring_scale_coring;
				fcc.fc_video_mctf = &ar0130_mliso_fc_video_mctf;
				fcc.fc_video_mctf_temporal_adjust =&ar0130_mliso_fc_video_mctf_temporal_adjust;
				}
			sprintf(sensor_name, "ar0130");
			break;
		case SENSOR_MT9T002:
			printf("mt9t002\n");
			if(isp_pipeline ==ISP_PIPELINE_LISO){
				fcc.fc_ae_target = &mt9t002_liso_fc_ae_target;
				fcc.fc_wb_ratio = &mt9t002_liso_fc_wb_ratio;
				fcc.fc_blc = &mt9t002_liso_fc_blc;
				fcc.fc_antialiasing = &mt9t002_liso_fc_antialiasing;
				fcc.fc_grgbmismatch = &mt9t002_liso_fc_grgbmismatch;
				fcc.fc_dpc = &mt9t002_liso_fc_dpc;
				fcc.fc_cfanf_low = &mt9t002_liso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = NULL;
				fcc.fc_demosaic = &mt9t002_liso_fc_demosaic;
				fcc.fc_cc = &mt9t002_liso_fc_cc;
				fcc.fc_tone = &mt9t002_liso_fc_tone;
				fcc.fc_rgb2yuv = &mt9t002_liso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &mt9t002_liso_fc_chroma_scale;
				fcc.fc_chroma_median = &mt9t002_liso_fc_chroma_median;
				fcc.fc_chroma_nf = &mt9t002_liso_fc_chroma_nf;
				fcc.fc_cdnr = &mt9t002_liso_fc_cdnr;
				fcc.fc_1stmode_sel = &mt9t002_liso_fc_1stmode_sel;
				fcc.fc_asf = &mt9t002_liso_fc_asf;
				fcc.fc_1st_shpboth = &mt9t002_liso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &mt9t002_liso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &mt9t002_liso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &mt9t002_liso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &mt9t002_liso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &mt9t002_liso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &mt9t002_liso_fc_1st_shpcoring_scale_coring;
				fcc.fc_video_mctf = &mt9t002_liso_fc_video_mctf;
				}
			else if(isp_pipeline ==ISP_PIPELINE_ADV_LISO){
				fcc.fc_ae_target = &mt9t002_aliso_fc_ae_target;
				fcc.fc_wb_ratio = &mt9t002_aliso_fc_wb_ratio;
				fcc.fc_blc = &mt9t002_aliso_fc_blc;
				fcc.fc_antialiasing = &mt9t002_aliso_fc_antialiasing;
				fcc.fc_grgbmismatch = &mt9t002_aliso_fc_grgbmismatch;
				fcc.fc_dpc = &mt9t002_aliso_fc_dpc;
				fcc.fc_cfanf_low = &mt9t002_aliso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = NULL;
				fcc.fc_demosaic = &mt9t002_aliso_fc_demosaic;
				fcc.fc_cc = &mt9t002_aliso_fc_cc;
				fcc.fc_tone = &mt9t002_aliso_fc_tone;
				fcc.fc_rgb2yuv = &mt9t002_aliso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &mt9t002_aliso_fc_chroma_scale;
				fcc.fc_chroma_median = &mt9t002_aliso_fc_chroma_median;
				fcc.fc_chroma_nf = &mt9t002_aliso_fc_chroma_nf;
				fcc.fc_cdnr = &mt9t002_aliso_fc_cdnr;
				fcc.fc_1stmode_sel = &mt9t002_aliso_fc_1stmode_sel;
				fcc.fc_asf = &mt9t002_aliso_fc_asf;
				fcc.fc_1st_shpboth = &mt9t002_aliso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &mt9t002_aliso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &mt9t002_aliso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &mt9t002_aliso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &mt9t002_aliso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &mt9t002_aliso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &mt9t002_aliso_fc_1st_shpcoring_scale_coring;
				fcc.fc_final_shpboth = &mt9t002_aliso_fc_final_shpboth;
				fcc.fc_final_shpnoise = &mt9t002_aliso_fc_final_shpnoise;
				fcc.fc_final_shpfir = &mt9t002_aliso_fc_final_shpfir;
				fcc.fc_final_shpcoring = &mt9t002_aliso_fc_final_shpcoring;
				fcc.fc_final_shpcoring_idx_scale = &mt9t002_aliso_fc_final_shpcoring_idx_scale;
				fcc.fc_final_shpcoring_min = &mt9t002_aliso_fc_final_shpcoring_min;
				fcc.fc_final_shpcoring_scale_coring = &mt9t002_aliso_fc_final_shpcoring_scale_coring;
				fcc.fc_wide_chroma_filter =&mt9t002_aliso_fc_wide_chroma_filter;
				fcc.fc_wide_chroma_filter_combine =&mt9t002_aliso_fc_wide_chroma_filter_combine;
				fcc.fc_video_mctf = &mt9t002_aliso_fc_video_mctf;
				fcc.fc_video_mctf_temporal_adjust =&mt9t002_aliso_fc_video_mctf_temporal_adjust;
				}
			else if(isp_pipeline ==ISP_PIPELINE_MID_LISO){
				fcc.fc_ae_target = &mt9t002_mliso_fc_ae_target;
				fcc.fc_wb_ratio = &mt9t002_mliso_fc_wb_ratio;
				fcc.fc_blc = &mt9t002_mliso_fc_blc;
				fcc.fc_antialiasing = &mt9t002_mliso_fc_antialiasing;
				fcc.fc_grgbmismatch = &mt9t002_mliso_fc_grgbmismatch;
				fcc.fc_dpc = &mt9t002_mliso_fc_dpc;
				fcc.fc_cfanf_low = &mt9t002_mliso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = NULL;
				fcc.fc_demosaic = &mt9t002_mliso_fc_demosaic;
				fcc.fc_cc = &mt9t002_mliso_fc_cc;
				fcc.fc_tone = &mt9t002_mliso_fc_tone;
				fcc.fc_rgb2yuv = &mt9t002_mliso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &mt9t002_mliso_fc_chroma_scale;
				fcc.fc_chroma_median = &mt9t002_mliso_fc_chroma_median;
				fcc.fc_chroma_nf = &mt9t002_mliso_fc_chroma_nf;
				fcc.fc_cdnr = &mt9t002_mliso_fc_cdnr;
				fcc.fc_1stmode_sel = &mt9t002_mliso_fc_1stmode_sel;
				fcc.fc_asf = &mt9t002_mliso_fc_asf;
				fcc.fc_1st_shpboth = &mt9t002_mliso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &mt9t002_mliso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &mt9t002_mliso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &mt9t002_mliso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &mt9t002_mliso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &mt9t002_mliso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &mt9t002_mliso_fc_1st_shpcoring_scale_coring;
				fcc.fc_video_mctf = &mt9t002_mliso_fc_video_mctf;
				fcc.fc_video_mctf_temporal_adjust =&mt9t002_mliso_fc_video_mctf_temporal_adjust;
				}
			sprintf(sensor_name, "mt9t002");
			break;
		case SENSOR_AR0141:
			printf("ar0141\n");
			if(isp_pipeline ==ISP_PIPELINE_LISO){
				fcc.fc_ae_target = &ar0141_liso_fc_ae_target;
				fcc.fc_wb_ratio = &ar0141_liso_fc_wb_ratio;
				fcc.fc_blc = &ar0141_liso_fc_blc;
				fcc.fc_antialiasing = &ar0141_liso_fc_antialiasing;
				fcc.fc_grgbmismatch = &ar0141_liso_fc_grgbmismatch;
				fcc.fc_dpc = &ar0141_liso_fc_dpc;
				fcc.fc_cfanf_low = &ar0141_liso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = NULL;
				fcc.fc_demosaic = &ar0141_liso_fc_demosaic;
				fcc.fc_cc = &ar0141_liso_fc_cc;
				fcc.fc_tone = &ar0141_liso_fc_tone;
				fcc.fc_rgb2yuv = &ar0141_liso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &ar0141_liso_fc_chroma_scale;
				fcc.fc_chroma_median = &ar0141_liso_fc_chroma_median;
				fcc.fc_chroma_nf = &ar0141_liso_fc_chroma_nf;
				fcc.fc_cdnr = &ar0141_liso_fc_cdnr;
				fcc.fc_1stmode_sel = &ar0141_liso_fc_1stmode_sel;
				fcc.fc_asf = &ar0141_liso_fc_asf;
				fcc.fc_1st_shpboth = &ar0141_liso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &ar0141_liso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &ar0141_liso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &ar0141_liso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &ar0141_liso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &ar0141_liso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &ar0141_liso_fc_1st_shpcoring_scale_coring;
				fcc.fc_video_mctf = &ar0141_liso_fc_video_mctf;
				}
			else if(isp_pipeline ==ISP_PIPELINE_ADV_LISO){
				fcc.fc_ae_target = &ar0141_aliso_fc_ae_target;
				fcc.fc_wb_ratio = &ar0141_aliso_fc_wb_ratio;
				fcc.fc_blc = &ar0141_aliso_fc_blc;
				fcc.fc_antialiasing = &ar0141_aliso_fc_antialiasing;
				fcc.fc_grgbmismatch = &ar0141_aliso_fc_grgbmismatch;
				fcc.fc_dpc = &ar0141_aliso_fc_dpc;
				fcc.fc_cfanf_low = &ar0141_aliso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = NULL;
				fcc.fc_demosaic = &ar0141_aliso_fc_demosaic;
				fcc.fc_cc = &ar0141_aliso_fc_cc;
				fcc.fc_tone = &ar0141_aliso_fc_tone;
				fcc.fc_rgb2yuv = &ar0141_aliso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &ar0141_aliso_fc_chroma_scale;
				fcc.fc_chroma_median = &ar0141_aliso_fc_chroma_median;
				fcc.fc_chroma_nf = &ar0141_aliso_fc_chroma_nf;
				fcc.fc_cdnr = &ar0141_aliso_fc_cdnr;
				fcc.fc_1stmode_sel = &ar0141_aliso_fc_1stmode_sel;
				fcc.fc_asf = &ar0141_aliso_fc_asf;
				fcc.fc_1st_shpboth = &ar0141_aliso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &ar0141_aliso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &ar0141_aliso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &ar0141_aliso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &ar0141_aliso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &ar0141_aliso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &ar0141_aliso_fc_1st_shpcoring_scale_coring;
				fcc.fc_final_shpboth = &ar0141_aliso_fc_final_shpboth;
				fcc.fc_final_shpnoise = &ar0141_aliso_fc_final_shpnoise;
				fcc.fc_final_shpfir = &ar0141_aliso_fc_final_shpfir;
				fcc.fc_final_shpcoring = &ar0141_aliso_fc_final_shpcoring;
				fcc.fc_final_shpcoring_idx_scale = &ar0141_aliso_fc_final_shpcoring_idx_scale;
				fcc.fc_final_shpcoring_min = &ar0141_aliso_fc_final_shpcoring_min;
				fcc.fc_final_shpcoring_scale_coring = &ar0141_aliso_fc_final_shpcoring_scale_coring;
				fcc.fc_wide_chroma_filter =&ar0141_aliso_fc_wide_chroma_filter;
				fcc.fc_wide_chroma_filter_combine =&ar0141_aliso_fc_wide_chroma_filter_combine;
				fcc.fc_video_mctf = &ar0141_aliso_fc_video_mctf;
				fcc.fc_video_mctf_temporal_adjust =&ar0141_aliso_fc_video_mctf_temporal_adjust;
				}
			else if(isp_pipeline ==ISP_PIPELINE_MID_LISO){
				fcc.fc_ae_target = &ar0141_mliso_fc_ae_target;
				fcc.fc_wb_ratio = &ar0141_mliso_fc_wb_ratio;
				fcc.fc_blc = &ar0141_mliso_fc_blc;
				fcc.fc_antialiasing = &ar0141_mliso_fc_antialiasing;
				fcc.fc_grgbmismatch = &ar0141_mliso_fc_grgbmismatch;
				fcc.fc_dpc = &ar0141_mliso_fc_dpc;
				fcc.fc_cfanf_low = &ar0141_mliso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = NULL;
				fcc.fc_demosaic = &ar0141_mliso_fc_demosaic;
				fcc.fc_cc = &ar0141_mliso_fc_cc;
				fcc.fc_tone = &ar0141_mliso_fc_tone;
				fcc.fc_rgb2yuv = &ar0141_mliso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &ar0141_mliso_fc_chroma_scale;
				fcc.fc_chroma_median = &ar0141_mliso_fc_chroma_median;
				fcc.fc_chroma_nf = &ar0141_mliso_fc_chroma_nf;
				fcc.fc_cdnr = &ar0141_mliso_fc_cdnr;
				fcc.fc_1stmode_sel = &ar0141_mliso_fc_1stmode_sel;
				fcc.fc_asf = &ar0141_mliso_fc_asf;
				fcc.fc_1st_shpboth = &ar0141_mliso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &ar0141_mliso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &ar0141_mliso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &ar0141_mliso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &ar0141_mliso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &ar0141_mliso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &ar0141_mliso_fc_1st_shpcoring_scale_coring;
				fcc.fc_video_mctf = &ar0141_mliso_fc_video_mctf;
				fcc.fc_video_mctf_temporal_adjust =&ar0141_mliso_fc_video_mctf_temporal_adjust;
				}
			sprintf(sensor_name, "ar0141");
			break;
		case SENSOR_AR0230:
			printf("ar0230\n");
			if(isp_pipeline ==ISP_PIPELINE_LISO){
				fcc.fc_ae_target = &ar0230_liso_fc_ae_target;
				fcc.fc_wb_ratio = &ar0230_liso_fc_wb_ratio;
				fcc.fc_blc = (hdr_method == HDR_BUILD_IN_METHOD)? \
					&ar0230_liso_fc_build_in_wdr_blc : &ar0230_liso_fc_blc;
				fcc.fc_antialiasing = &ar0230_liso_fc_antialiasing;
				fcc.fc_grgbmismatch = &ar0230_liso_fc_grgbmismatch;
				fcc.fc_dpc = &ar0230_liso_fc_dpc;
				fcc.fc_cfanf_low = &ar0230_liso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = (hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &ar0230_liso_fc_2x_hdr_le;
				fcc.fc_demosaic = &ar0230_liso_fc_demosaic;
				fcc.fc_cc = &ar0230_liso_fc_cc;
				fcc.fc_tone = (hdr_pipeline == HDR_PIPELINE_OFF)? \
					&ar0230_liso_fc_tone : &ar0230_liso_fc_2x_hdr_tone;
				fcc.fc_rgb2yuv = &ar0230_liso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &ar0230_liso_fc_chroma_scale;
				fcc.fc_chroma_median = &ar0230_liso_fc_chroma_median;
				fcc.fc_chroma_nf = &ar0230_liso_fc_chroma_nf;
				fcc.fc_cdnr = &ar0230_liso_fc_cdnr;
				fcc.fc_1stmode_sel = &ar0230_liso_fc_1stmode_sel;
				fcc.fc_asf = &ar0230_liso_fc_asf;
				fcc.fc_1st_shpboth = &ar0230_liso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &ar0230_liso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &ar0230_liso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &ar0230_liso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &ar0230_liso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &ar0230_liso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &ar0230_liso_fc_1st_shpcoring_scale_coring;
				fcc.fc_video_mctf = &ar0230_liso_fc_video_mctf;
				fcc.fc_hdr_alpha = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &ar0230_liso_fc_hdr_alpha;
				fcc.fc_hdr_threshold = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &ar0230_liso_fc_hdr_threshold;
				fcc.fc_hdr_ce = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &ar0230_liso_fc_hdr_ce;
				}
			else if(isp_pipeline ==ISP_PIPELINE_ADV_LISO){
				fcc.fc_ae_target = &ar0230_aliso_fc_ae_target;
				fcc.fc_wb_ratio = &ar0230_aliso_fc_wb_ratio;
				fcc.fc_blc = (hdr_method == HDR_BUILD_IN_METHOD)? \
					&ar0230_aliso_fc_build_in_wdr_blc : &ar0230_aliso_fc_blc;
				fcc.fc_antialiasing = &ar0230_aliso_fc_antialiasing;
				fcc.fc_grgbmismatch = &ar0230_aliso_fc_grgbmismatch;
				fcc.fc_dpc = &ar0230_aliso_fc_dpc;
				fcc.fc_cfanf_low = &ar0230_aliso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = (hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &ar0230_aliso_fc_2x_hdr_le;
				fcc.fc_demosaic = &ar0230_aliso_fc_demosaic;
				fcc.fc_cc = &ar0230_aliso_fc_cc;
				fcc.fc_tone = (hdr_pipeline == HDR_PIPELINE_OFF)?\
				&ar0230_aliso_fc_tone : &ar0230_aliso_fc_2x_hdr_tone;
				fcc.fc_rgb2yuv = &ar0230_aliso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &ar0230_aliso_fc_chroma_scale;
				fcc.fc_chroma_median = &ar0230_aliso_fc_chroma_median;
				fcc.fc_chroma_nf = &ar0230_aliso_fc_chroma_nf;
				fcc.fc_cdnr = &ar0230_aliso_fc_cdnr;
				fcc.fc_1stmode_sel = &ar0230_aliso_fc_1stmode_sel;
				fcc.fc_asf = &ar0230_aliso_fc_asf;
				fcc.fc_1st_shpboth = &ar0230_aliso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &ar0230_aliso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &ar0230_aliso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &ar0230_aliso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &ar0230_aliso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &ar0230_aliso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &ar0230_aliso_fc_1st_shpcoring_scale_coring;
				fcc.fc_final_shpboth = &ar0230_aliso_fc_final_shpboth;
				fcc.fc_final_shpnoise = &ar0230_aliso_fc_final_shpnoise;
				fcc.fc_final_shpfir = &ar0230_aliso_fc_final_shpfir;
				fcc.fc_final_shpcoring = &ar0230_aliso_fc_final_shpcoring;
				fcc.fc_final_shpcoring_idx_scale = &ar0230_aliso_fc_final_shpcoring_idx_scale;
				fcc.fc_final_shpcoring_min = &ar0230_aliso_fc_final_shpcoring_min;
				fcc.fc_final_shpcoring_scale_coring = &ar0230_aliso_fc_final_shpcoring_scale_coring;
				fcc.fc_wide_chroma_filter =&ar0230_aliso_fc_wide_chroma_filter;
				fcc.fc_wide_chroma_filter_combine =&ar0230_aliso_fc_wide_chroma_filter_combine;
				fcc.fc_video_mctf = &ar0230_aliso_fc_video_mctf;
				fcc.fc_video_mctf_temporal_adjust =&ar0230_aliso_fc_video_mctf_temporal_adjust;
				fcc.fc_hdr_alpha = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &ar0230_aliso_fc_hdr_alpha;
				fcc.fc_hdr_threshold = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &ar0230_aliso_fc_hdr_threshold;
				fcc.fc_hdr_ce = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &ar0230_aliso_fc_hdr_ce;
				}
			else if(isp_pipeline ==ISP_PIPELINE_MID_LISO){
				fcc.fc_ae_target = &ar0230_mliso_fc_ae_target;
				fcc.fc_wb_ratio = &ar0230_mliso_fc_wb_ratio;
				fcc.fc_blc = (hdr_method == HDR_BUILD_IN_METHOD)? \
					&ar0230_mliso_fc_build_in_wdr_blc : &ar0230_mliso_fc_blc;
				fcc.fc_antialiasing = &ar0230_mliso_fc_antialiasing;
				fcc.fc_grgbmismatch = &ar0230_mliso_fc_grgbmismatch;
				fcc.fc_dpc = &ar0230_mliso_fc_dpc;
				fcc.fc_cfanf_low = &ar0230_mliso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = (hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &ar0230_mliso_fc_2x_hdr_le;
				fcc.fc_demosaic = &ar0230_mliso_fc_demosaic;
				fcc.fc_cc = &ar0230_mliso_fc_cc;
				fcc.fc_tone = (hdr_pipeline == HDR_PIPELINE_OFF)? \
					&ar0230_mliso_fc_tone : &ar0230_mliso_fc_2x_hdr_tone;
				fcc.fc_rgb2yuv = &ar0230_mliso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &ar0230_mliso_fc_chroma_scale;
				fcc.fc_chroma_median = &ar0230_mliso_fc_chroma_median;
				fcc.fc_chroma_nf = &ar0230_mliso_fc_chroma_nf;
				fcc.fc_cdnr = &ar0230_mliso_fc_cdnr;
				fcc.fc_1stmode_sel = &ar0230_mliso_fc_1stmode_sel;
				fcc.fc_asf = &ar0230_mliso_fc_asf;
				fcc.fc_1st_shpboth = &ar0230_mliso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &ar0230_mliso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &ar0230_mliso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &ar0230_mliso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &ar0230_mliso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &ar0230_mliso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &ar0230_mliso_fc_1st_shpcoring_scale_coring;
				fcc.fc_video_mctf = &ar0230_mliso_fc_video_mctf;
				fcc.fc_video_mctf_temporal_adjust =&ar0230_mliso_fc_video_mctf_temporal_adjust;
				fcc.fc_hdr_alpha = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &ar0230_mliso_fc_hdr_alpha;
				fcc.fc_hdr_threshold = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &ar0230_mliso_fc_hdr_threshold;
				fcc.fc_hdr_ce = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &ar0230_mliso_fc_hdr_ce;
				}
			else if(isp_pipeline ==ISP_PIPELINE_B_LISO){
				fcc.fc_ae_target = &ar0230_bliso_fc_ae_target;
				fcc.fc_wb_ratio = &ar0230_bliso_fc_wb_ratio;
				fcc.fc_blc = &ar0230_bliso_fc_blc;
				fcc.fc_antialiasing = &ar0230_bliso_fc_antialiasing;
				fcc.fc_grgbmismatch = &ar0230_bliso_fc_grgbmismatch;
				fcc.fc_dpc = &ar0230_bliso_fc_dpc;
				fcc.fc_cfanf_low = &ar0230_bliso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = &ar0230_bliso_le;
				fcc.fc_demosaic = &ar0230_bliso_fc_demosaic;
				fcc.fc_cc = &ar0230_bliso_fc_cc;
				fcc.fc_tone = (hdr_pipeline == HDR_PIPELINE_OFF)? &ar0230_bliso_fc_tone : \
					((expo_num = HDR_2X)? &ar0230_bliso_fc_2x_hdr_tone : &ar0230_bliso_fc_3x_hdr_tone);
				fcc.fc_rgb2yuv = &ar0230_bliso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &ar0230_bliso_fc_chroma_scale;
				fcc.fc_chroma_median = &ar0230_bliso_fc_chroma_median;
				fcc.fc_chroma_nf = &ar0230_bliso_fc_chroma_nf;
				fcc.fc_1stmode_sel = &ar0230_bliso_fc_1stmode_sel;
				fcc.fc_asf = &ar0230_bliso_fc_asf;
				fcc.fc_1st_shpboth = &ar0230_bliso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &ar0230_bliso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &ar0230_bliso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &ar0230_bliso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &ar0230_bliso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &ar0230_bliso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &ar0230_bliso_fc_1st_shpcoring_scale_coring;

				fcc.fc_video_mctf = &ar0230_bliso_fc_video_mctf;
				fcc.fc_video_mctf_temporal_adjust =&ar0230_bliso_fc_video_mctf_temporal_adjust;
				fcc.fc_hdr_alpha = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &ar0230_bliso_fc_hdr_alpha;
				fcc.fc_hdr_threshold = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &ar0230_bliso_fc_hdr_threshold;
				fcc.fc_hdr_ce = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &ar0230_bliso_fc_hdr_ce;
				fcc.fc_mo_antialiasing = &ar0230_bliso_fc_mo_antialiasing;
				fcc.fc_mo_grgbmismatch = &ar0230_bliso_fc_mo_grgbmismatch;
				fcc.fc_mo_dpc = &ar0230_bliso_fc_mo_dpc;
				fcc.fc_mo_cfanf = &ar0230_bliso_fc_mo_cfanf;
				fcc.fc_mo_demosaic = &ar0230_bliso_fc_mo_demosaic;
				fcc.fc_mo_chroma_median = &ar0230_bliso_fc_mo_chroma_median;
				fcc.fc_mo_chroma_nf = &ar0230_bliso_fc_mo_chroma_nf;
				fcc.fc_mo_1stmode_sel = &ar0230_bliso_fc_mo_1stmode_sel;
				fcc.fc_mo_asf = &ar0230_bliso_fc_mo_asf;
				fcc.fc_mo_1st_shpboth = &ar0230_bliso_fc_mo_1st_shpboth;
				fcc.fc_mo_1st_shpnoise = &ar0230_bliso_fc_mo_1st_shpnoise;
				fcc.fc_mo_1st_shpfir = &ar0230_bliso_fc_mo_1st_shpfir;
				fcc.fc_mo_1st_shpcoring = &ar0230_bliso_fc_mo_1st_shpcoring;
				fcc.fc_mo_1st_shpcoring_idx_scale = &ar0230_bliso_fc_mo_1st_shpcoring_idx_scale;
				fcc.fc_mo_1st_shpcoring_min = &ar0230_bliso_fc_mo_1st_shpcoring_min;
				fcc.fc_mo_1st_shpcoring_scale_coring = &ar0230_bliso_fc_mo_1st_shpcoring_scale_coring;
				}
			sprintf(sensor_name, "ar0230");
			break;
		case SENSOR_AR0237:
			printf("ar0237\n");
			if(isp_pipeline ==ISP_PIPELINE_LISO){
				fcc.fc_ae_target = &ar0237_liso_fc_ae_target;
				fcc.fc_wb_ratio = &ar0237_liso_fc_wb_ratio;
				fcc.fc_blc = &ar0237_liso_fc_blc;
				fcc.fc_antialiasing = &ar0237_liso_fc_antialiasing;
				fcc.fc_grgbmismatch = &ar0237_liso_fc_grgbmismatch;
				fcc.fc_dpc = &ar0237_liso_fc_dpc;
				fcc.fc_cfanf_low = &ar0237_liso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = (hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &ar0237_liso_fc_2x_hdr_le;
				fcc.fc_demosaic = &ar0237_liso_fc_demosaic;
				fcc.fc_cc = &ar0237_liso_fc_cc;
				fcc.fc_tone = (hdr_pipeline == HDR_PIPELINE_OFF)? \
					&ar0237_liso_fc_tone : &ar0237_liso_fc_2x_hdr_tone;
				fcc.fc_rgb2yuv = &ar0237_liso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &ar0237_liso_fc_chroma_scale;
				fcc.fc_chroma_median = &ar0237_liso_fc_chroma_median;
				fcc.fc_chroma_nf = &ar0237_liso_fc_chroma_nf;
				fcc.fc_cdnr = &ar0237_liso_fc_cdnr;
				fcc.fc_1stmode_sel = &ar0237_liso_fc_1stmode_sel;
				fcc.fc_asf = &ar0237_liso_fc_asf;
				fcc.fc_1st_shpboth = &ar0237_liso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &ar0237_liso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &ar0237_liso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &ar0237_liso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &ar0237_liso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &ar0237_liso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &ar0237_liso_fc_1st_shpcoring_scale_coring;
				fcc.fc_video_mctf = &ar0237_liso_fc_video_mctf;
				fcc.fc_hdr_alpha = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &ar0237_liso_fc_hdr_alpha;
				fcc.fc_hdr_threshold = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &ar0237_liso_fc_hdr_threshold;
				fcc.fc_hdr_ce = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &ar0237_liso_fc_hdr_ce;
				}
			else if(isp_pipeline ==ISP_PIPELINE_ADV_LISO){
				fcc.fc_ae_target = &ar0237_aliso_fc_ae_target;
				fcc.fc_wb_ratio = &ar0237_aliso_fc_wb_ratio;
				fcc.fc_blc = &ar0237_aliso_fc_blc;
				fcc.fc_antialiasing = &ar0237_aliso_fc_antialiasing;
				fcc.fc_grgbmismatch = &ar0237_aliso_fc_grgbmismatch;
				fcc.fc_dpc = &ar0237_aliso_fc_dpc;
				fcc.fc_cfanf_low = &ar0237_aliso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = (hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &ar0237_aliso_fc_2x_hdr_le;
				fcc.fc_demosaic = &ar0237_aliso_fc_demosaic;
				fcc.fc_cc = &ar0237_aliso_fc_cc;
				fcc.fc_tone = (hdr_pipeline == HDR_PIPELINE_OFF)?\
				&ar0237_aliso_fc_tone : &ar0237_aliso_fc_2x_hdr_tone;
				fcc.fc_rgb2yuv = &ar0237_aliso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &ar0237_aliso_fc_chroma_scale;
				fcc.fc_chroma_median = &ar0237_aliso_fc_chroma_median;
				fcc.fc_chroma_nf = &ar0237_aliso_fc_chroma_nf;
				fcc.fc_cdnr = &ar0237_aliso_fc_cdnr;
				fcc.fc_1stmode_sel = &ar0237_aliso_fc_1stmode_sel;
				fcc.fc_asf = &ar0237_aliso_fc_asf;
				fcc.fc_1st_shpboth = &ar0237_aliso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &ar0237_aliso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &ar0237_aliso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &ar0237_aliso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &ar0237_aliso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &ar0237_aliso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &ar0237_aliso_fc_1st_shpcoring_scale_coring;
				fcc.fc_final_shpboth = &ar0237_aliso_fc_final_shpboth;
				fcc.fc_final_shpnoise = &ar0237_aliso_fc_final_shpnoise;
				fcc.fc_final_shpfir = &ar0237_aliso_fc_final_shpfir;
				fcc.fc_final_shpcoring = &ar0237_aliso_fc_final_shpcoring;
				fcc.fc_final_shpcoring_idx_scale = &ar0237_aliso_fc_final_shpcoring_idx_scale;
				fcc.fc_final_shpcoring_min = &ar0237_aliso_fc_final_shpcoring_min;
				fcc.fc_final_shpcoring_scale_coring = &ar0237_aliso_fc_final_shpcoring_scale_coring;
				fcc.fc_wide_chroma_filter =&ar0237_aliso_fc_wide_chroma_filter;
				fcc.fc_wide_chroma_filter_combine =&ar0237_aliso_fc_wide_chroma_filter_combine;
				fcc.fc_video_mctf = &ar0237_aliso_fc_video_mctf;
				fcc.fc_video_mctf_temporal_adjust =&ar0237_aliso_fc_video_mctf_temporal_adjust;
				fcc.fc_hdr_alpha = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &ar0237_aliso_fc_hdr_alpha;
				fcc.fc_hdr_threshold = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &ar0237_aliso_fc_hdr_threshold;
				fcc.fc_hdr_ce = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &ar0237_aliso_fc_hdr_ce;
				}
			else if(isp_pipeline ==ISP_PIPELINE_MID_LISO){
				fcc.fc_ae_target = &ar0237_mliso_fc_ae_target;
				fcc.fc_wb_ratio = &ar0237_mliso_fc_wb_ratio;
				fcc.fc_blc = &ar0237_mliso_fc_blc;
				fcc.fc_antialiasing = &ar0237_mliso_fc_antialiasing;
				fcc.fc_grgbmismatch = &ar0237_mliso_fc_grgbmismatch;
				fcc.fc_dpc = &ar0237_mliso_fc_dpc;
				fcc.fc_cfanf_low = &ar0237_mliso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = (hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &ar0237_mliso_fc_2x_hdr_le;
				fcc.fc_demosaic = &ar0237_mliso_fc_demosaic;
				fcc.fc_cc = &ar0237_mliso_fc_cc;
				fcc.fc_tone = (hdr_pipeline == HDR_PIPELINE_OFF)? \
					&ar0237_mliso_fc_tone : &ar0237_mliso_fc_2x_hdr_tone;
				fcc.fc_rgb2yuv = &ar0237_mliso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &ar0237_mliso_fc_chroma_scale;
				fcc.fc_chroma_median = &ar0237_mliso_fc_chroma_median;
				fcc.fc_chroma_nf = &ar0237_mliso_fc_chroma_nf;
				fcc.fc_cdnr = &ar0237_mliso_fc_cdnr;
				fcc.fc_1stmode_sel = &ar0237_mliso_fc_1stmode_sel;
				fcc.fc_asf = &ar0237_mliso_fc_asf;
				fcc.fc_1st_shpboth = &ar0237_mliso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &ar0237_mliso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &ar0237_mliso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &ar0237_mliso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &ar0237_mliso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &ar0237_mliso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &ar0237_mliso_fc_1st_shpcoring_scale_coring;
				fcc.fc_video_mctf = &ar0237_mliso_fc_video_mctf;
				fcc.fc_video_mctf_temporal_adjust =&ar0237_mliso_fc_video_mctf_temporal_adjust;
				fcc.fc_hdr_alpha = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &ar0237_mliso_fc_hdr_alpha;
				fcc.fc_hdr_threshold = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &ar0237_mliso_fc_hdr_threshold;
				fcc.fc_hdr_ce = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &ar0237_mliso_fc_hdr_ce;
				}
			else if(isp_pipeline ==ISP_PIPELINE_B_LISO){
				fcc.fc_ae_target = &ar0237_bliso_fc_ae_target;
				fcc.fc_wb_ratio = &ar0237_bliso_fc_wb_ratio;
				fcc.fc_blc = &ar0237_bliso_fc_blc;
				fcc.fc_antialiasing = &ar0237_bliso_fc_antialiasing;
				fcc.fc_grgbmismatch = &ar0237_bliso_fc_grgbmismatch;
				fcc.fc_dpc = &ar0237_bliso_fc_dpc;
				fcc.fc_cfanf_low = &ar0237_bliso_fc_cfanf;
				fcc.fc_cfanf_high = NULL;
				fcc.fc_le = &ar0237_bliso_le;
				fcc.fc_demosaic = &ar0237_bliso_fc_demosaic;
				fcc.fc_cc = &ar0237_bliso_fc_cc;
				fcc.fc_tone = (hdr_pipeline == HDR_PIPELINE_OFF)? &ar0237_bliso_fc_tone : \
					((expo_num = HDR_2X)? &ar0237_bliso_fc_2x_hdr_tone : &ar0237_bliso_fc_3x_hdr_tone);
				fcc.fc_rgb2yuv = &ar0237_bliso_fc_rgb2yuv;
				fcc.fc_chroma_scale = &ar0237_bliso_fc_chroma_scale;
				fcc.fc_chroma_median = &ar0237_bliso_fc_chroma_median;
				fcc.fc_chroma_nf = &ar0237_bliso_fc_chroma_nf;
				fcc.fc_1stmode_sel = &ar0237_bliso_fc_1stmode_sel;
				fcc.fc_asf = &ar0237_bliso_fc_asf;
				fcc.fc_1st_shpboth = &ar0237_bliso_fc_1st_shpboth;
				fcc.fc_1st_shpnoise = &ar0237_bliso_fc_1st_shpnoise;
				fcc.fc_1st_shpfir = &ar0237_bliso_fc_1st_shpfir;
				fcc.fc_1st_shpcoring = &ar0237_bliso_fc_1st_shpcoring;
				fcc.fc_1st_shpcoring_idx_scale = &ar0237_bliso_fc_1st_shpcoring_idx_scale;
				fcc.fc_1st_shpcoring_min = &ar0237_bliso_fc_1st_shpcoring_min;
				fcc.fc_1st_shpcoring_scale_coring = &ar0237_bliso_fc_1st_shpcoring_scale_coring;

				fcc.fc_video_mctf = &ar0237_bliso_fc_video_mctf;
				fcc.fc_video_mctf_temporal_adjust =&ar0237_bliso_fc_video_mctf_temporal_adjust;
				fcc.fc_hdr_alpha = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &ar0237_bliso_fc_hdr_alpha;
				fcc.fc_hdr_threshold = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &ar0237_bliso_fc_hdr_threshold;
				fcc.fc_hdr_ce = \
					(hdr_pipeline == HDR_PIPELINE_OFF)? NULL : &ar0237_bliso_fc_hdr_ce;
				fcc.fc_mo_antialiasing = &ar0237_bliso_fc_mo_antialiasing;
				fcc.fc_mo_grgbmismatch = &ar0237_bliso_fc_mo_grgbmismatch;
				fcc.fc_mo_dpc = &ar0237_bliso_fc_mo_dpc;
				fcc.fc_mo_cfanf = &ar0237_bliso_fc_mo_cfanf;
				fcc.fc_mo_demosaic = &ar0237_bliso_fc_mo_demosaic;
				fcc.fc_mo_chroma_median = &ar0237_bliso_fc_mo_chroma_median;
				fcc.fc_mo_chroma_nf = &ar0237_bliso_fc_mo_chroma_nf;
				fcc.fc_mo_1stmode_sel = &ar0237_bliso_fc_mo_1stmode_sel;
				fcc.fc_mo_asf = &ar0237_bliso_fc_mo_asf;
				fcc.fc_mo_1st_shpboth = &ar0237_bliso_fc_mo_1st_shpboth;
				fcc.fc_mo_1st_shpnoise = &ar0237_bliso_fc_mo_1st_shpnoise;
				fcc.fc_mo_1st_shpfir = &ar0237_bliso_fc_mo_1st_shpfir;
				fcc.fc_mo_1st_shpcoring = &ar0237_bliso_fc_mo_1st_shpcoring;
				fcc.fc_mo_1st_shpcoring_idx_scale = &ar0237_bliso_fc_mo_1st_shpcoring_idx_scale;
				fcc.fc_mo_1st_shpcoring_min = &ar0237_bliso_fc_mo_1st_shpcoring_min;
				fcc.fc_mo_1st_shpcoring_scale_coring = &ar0237_bliso_fc_mo_1st_shpcoring_scale_coring;
				}
			sprintf(sensor_name, "ar0237");
			break;
		default:
			printf("undefined sensor id\n");
			return -1;
		}
	img_adj_init_containers_liso(&fcc);

	return 0;
}

int img_aaa_config(int fd_iav, struct vindev_aaa_info* vin_aaa_info, img_config_info_t *p_img_cfg)
{
	int i =0;
	ae_cfg_tbl_t ae_tbl[MAX_HDR_EXPOSURE_NUM];
	u8 ae_tbl_num = 0;
	u32 ae_gain_step = 0;
	lens_param_t lens_param_info = {
		{NULL, NULL, 0, NULL},
	};

	hdr_pipeline_t hdr_pipeline = p_img_cfg->hdr_config.pipeline;
	hdr_method_t hdr_method = p_img_cfg->hdr_config.method;
	u8 expo_num = p_img_cfg->hdr_config.expo_num;

	img_aeb_sensor_config_t * p_sensor_cfg = NULL;
	img_aeb_tile_config_t * p_tile_cfg = NULL;
	img_aeb_expo_lines_t * p_ae_lines = NULL;
	img_aeb_gain_table_t* p_gain_tbl = NULL;
	img_aeb_sht_nl_table_t* p_sht_nl_tbl = NULL;
	img_aeb_wb_param_t* p_wb_param = NULL;
	img_auto_knee_config_info_t* p_auto_knee_cfg =NULL;
	switch(vin_aaa_info->sensor_id){
		case SENSOR_MN34220PL:
			p_sensor_cfg = &mn34220pl_aeb_sensor_config;
			p_tile_cfg = &mn34220pl_aeb_tile_config;
			p_gain_tbl = &mn34220pl_aeb_gain_table;
			p_sht_nl_tbl = &mn34220pl_aeb_sht_nl_table;
			p_wb_param = &mn34220pl_aeb_wb_param;
			p_auto_knee_cfg =&mn34220pl_auto_knee_config;

			if(hdr_pipeline == HDR_PIPELINE_OFF){
				p_ae_lines = &mn34220pl_aeb_expo_lines;
			}else if(hdr_pipeline != HDR_PIPELINE_OFF && expo_num == HDR_2X){
				p_ae_lines = &mn34220pl_aeb_expo_lines_2x_hdr;
			}else if(hdr_pipeline != HDR_PIPELINE_OFF && expo_num == HDR_3X){
				p_ae_lines = &mn34220pl_aeb_expo_lines_3x_hdr;
			}else{
				printf("error: can't find the proper ae lines for mode %d, expo num %d\n", \
					hdr_pipeline, expo_num);
				return -1;
			}
			break;
		case SENSOR_OV9718:
			p_sensor_cfg = &ov9718_aeb_sensor_config;
			p_tile_cfg = &ov9718_aeb_tile_config;
			p_gain_tbl = &ov9718_aeb_gain_table;
			p_sht_nl_tbl = &ov9718_aeb_sht_nl_table;
			p_wb_param = &ov9718_aeb_wb_param;
			p_ae_lines = &ov9718_aeb_expo_lines;
			break;
		case SENSOR_OV9710:
			p_sensor_cfg = &ov9715_aeb_sensor_config;
			p_tile_cfg = &ov9715_aeb_tile_config;
			p_gain_tbl = &ov9715_aeb_gain_table;
			p_sht_nl_tbl = &ov9715_aeb_sht_nl_table;
			p_wb_param = &ov9715_aeb_wb_param;
			p_ae_lines = &ov9715_aeb_expo_lines;
			break;
		case SENSOR_OV9750:
			p_sensor_cfg = &ov9750_aeb_sensor_config;
			p_tile_cfg = &ov9750_aeb_tile_config;
			p_gain_tbl = &ov9750_aeb_gain_table;
			p_sht_nl_tbl = &ov9750_aeb_sht_nl_table;
			p_wb_param = &ov9750_aeb_wb_param;
			p_ae_lines = &ov9750_aeb_expo_lines;
			break;
		case SENSOR_OV4689:
			p_sensor_cfg = &ov4689_aeb_sensor_config;
			p_tile_cfg = &ov4689_aeb_tile_config;
			p_gain_tbl = &ov4689_aeb_gain_table;
			p_sht_nl_tbl = &ov4689_aeb_sht_nl_table;
			p_wb_param = &ov4689_aeb_wb_param;

			if(hdr_pipeline == HDR_PIPELINE_OFF){
				if(lens_mount_id==LENS_M13VP288IR_ID) {
				//	lens_param_info.piris_std.dgain = &M13VP288IR_PIRIS_DGAIN;	//not need for S2L AE
					lens_param_info.piris_std.scope = M13VP288IR_PIRIS_SCOPE.table;
					lens_param_info.piris_std.table = M13VP288IR_PIRIS_STEP.table;
					lens_param_info.piris_std.tbl_size = M13VP288IR_PIRIS_STEP.header.array_size;
					p_ae_lines = &ov4689_aeb_expo_lines_m13vp288ir;
				} else if(lens_mount_id==LENS_MZ128BP2810ICR_ID) {
				//	lens_param_info.piris_std.dgain = &MZ128BP2810ICR_PIRIS_DGAIN;	//not need for S2L AE
					lens_param_info.piris_std.scope = MZ128BP2810ICR_PIRIS_SCOPE.table;
					lens_param_info.piris_std.table = MZ128BP2810ICR_PIRIS_STEP.table;
					lens_param_info.piris_std.tbl_size = MZ128BP2810ICR_PIRIS_STEP.header.array_size;
					p_ae_lines = &ov4689_aeb_expo_lines_mz128bp2810icr;
				} else {
					p_ae_lines = &ov4689_aeb_expo_lines;
				}
			}else if(hdr_pipeline != HDR_PIPELINE_OFF && expo_num == HDR_2X){
				p_ae_lines = &ov4689_aeb_expo_lines_2x_hdr;
			}else if(hdr_pipeline != HDR_PIPELINE_OFF && expo_num == HDR_3X){
				p_ae_lines = &ov4689_aeb_expo_lines_3x_hdr;
			}else{
				printf("error: can't find the proper ae lines for mode %d, expo num %d\n", \
					hdr_pipeline, expo_num);
				return -1;
			}
			break;
		case SENSOR_OV5658:
			p_sensor_cfg = &ov5658_aeb_sensor_config;
			p_tile_cfg = &ov5658_aeb_tile_config;
			p_gain_tbl = &ov5658_aeb_gain_table;
			p_sht_nl_tbl = &ov5658_aeb_sht_nl_table;
			p_wb_param = &ov5658_aeb_wb_param;
			p_ae_lines = &ov5658_aeb_expo_lines;
			break;
		case SENSOR_IMX123:
			p_sensor_cfg = &imx123_aeb_sensor_config;
			p_tile_cfg = &imx123_aeb_tile_config;
			p_gain_tbl = &imx123_aeb_gain_table;
			p_sht_nl_tbl = &imx123_aeb_sht_nl_table;
			p_wb_param = &imx123_aeb_wb_param;

			if(hdr_pipeline == HDR_PIPELINE_OFF){
				p_ae_lines = &imx123_aeb_expo_lines;
			}else if(hdr_pipeline != HDR_PIPELINE_OFF && expo_num == HDR_2X){
				p_ae_lines = &imx123_aeb_expo_lines_2x_hdr;
			}else if(hdr_pipeline != HDR_PIPELINE_OFF && expo_num == HDR_3X){
				p_ae_lines = &imx123_aeb_expo_lines_3x_hdr;
			}else{
				printf("error: can't find the proper ae lines for mode %d, expo num %d\n", \
					hdr_pipeline, expo_num);
				return -1;
			}
			break;
		case SENSOR_IMX123_DCG:
			p_sensor_cfg = &imx123dcg_aeb_sensor_config;
			p_tile_cfg = &imx123dcg_aeb_tile_config;
			p_gain_tbl = &imx123dcg_aeb_gain_table;
			p_sht_nl_tbl = &imx123dcg_aeb_sht_nl_table;
			p_wb_param = &imx123dcg_aeb_wb_param;

			if(hdr_pipeline != HDR_PIPELINE_OFF && expo_num == HDR_2X){
				p_ae_lines = &imx123dcg_aeb_expo_lines_2x_hdr;
			}else{
				printf("error: can't find the proper ae lines for mode %d, expo num %d\n", \
					hdr_pipeline, expo_num);
				return -1;
			}
			break;
		case SENSOR_IMX124:
			p_sensor_cfg = &imx124_aeb_sensor_config;
			p_tile_cfg = &imx124_aeb_tile_config;
			p_gain_tbl = &imx124_aeb_gain_table;
			p_sht_nl_tbl = &imx124_aeb_sht_nl_table;
			p_wb_param = &imx124_aeb_wb_param;
			p_ae_lines = &imx124_aeb_expo_lines;
			break;
		case SENSOR_IMX104:
			p_sensor_cfg = &imx104_aeb_sensor_config;
			p_tile_cfg = &imx104_aeb_tile_config;
			p_gain_tbl = &imx104_aeb_gain_table;
			p_sht_nl_tbl = &imx104_aeb_sht_nl_table;
			p_wb_param = &imx104_aeb_wb_param;
			p_ae_lines = &imx104_aeb_expo_lines;
			break;
		case SENSOR_IMX178:
			p_sensor_cfg = &imx178_aeb_sensor_config;
			p_tile_cfg = &imx178_aeb_tile_config;
			p_gain_tbl = &imx178_aeb_gain_table;
			p_sht_nl_tbl = &imx178_aeb_sht_nl_table;
			p_wb_param = &imx178_aeb_wb_param;
			p_ae_lines = &imx178_aeb_expo_lines;
			break;
		case SENSOR_IMX224:
			p_sensor_cfg = &imx224_aeb_sensor_config;
			p_tile_cfg = &imx224_aeb_tile_config;
			p_gain_tbl = &imx224_aeb_gain_table;
			p_sht_nl_tbl = &imx224_aeb_sht_nl_table;
			p_wb_param = &imx224_aeb_wb_param;

			if(hdr_pipeline == HDR_PIPELINE_OFF){
				p_ae_lines = &imx224_aeb_expo_lines;
			}else if(hdr_pipeline != HDR_PIPELINE_OFF && expo_num == HDR_2X){
				p_ae_lines = &imx224_aeb_expo_lines_2x_hdr;
			}else if(hdr_pipeline != HDR_PIPELINE_OFF && expo_num == HDR_3X){
				p_ae_lines = &imx224_aeb_expo_lines_3x_hdr;
			}else{
				printf("error: can't find the proper ae lines for mode %d, expo num %d\n", \
					hdr_pipeline, expo_num);
				return -1;
			}
			break;
		case SENSOR_IMX291:
			p_sensor_cfg = &imx291_aeb_sensor_config;
			p_tile_cfg = &imx291_aeb_tile_config;
			p_gain_tbl = &imx291_aeb_gain_table;
			p_sht_nl_tbl = &imx291_aeb_sht_nl_table;
			p_wb_param = &imx291_aeb_wb_param;
			p_ae_lines = &imx291_aeb_expo_lines;
			break;
		case SENSOR_IMX290:
			p_sensor_cfg = &imx290_aeb_sensor_config;
			p_tile_cfg = &imx290_aeb_tile_config;
			p_gain_tbl = &imx290_aeb_gain_table;
			p_sht_nl_tbl = &imx290_aeb_sht_nl_table;
			p_wb_param = &imx290_aeb_wb_param;

			if(hdr_pipeline == HDR_PIPELINE_OFF){
				p_ae_lines = &imx290_aeb_expo_lines;
			}else if(hdr_pipeline != HDR_PIPELINE_OFF && expo_num == HDR_2X){
				p_ae_lines = &imx290_aeb_expo_lines_2x_hdr;
			}else if(hdr_pipeline != HDR_PIPELINE_OFF && expo_num == HDR_3X){
				p_ae_lines = &imx290_aeb_expo_lines_3x_hdr;
			}else{
				printf("error: can't find the proper ae lines for mode %d, expo num %d\n", \
					hdr_pipeline, expo_num);
				return -1;
			}
			break;
		case SENSOR_IMX322:
			p_sensor_cfg = &imx322_aeb_sensor_config;
			p_tile_cfg = &imx322_aeb_tile_config;
			p_gain_tbl = &imx322_aeb_gain_table;
			p_sht_nl_tbl = &imx322_aeb_sht_nl_table;
			p_wb_param = &imx322_aeb_wb_param;
			p_ae_lines = &imx322_aeb_expo_lines;
			break;
		case SENSOR_AR0130:
			p_sensor_cfg = &ar0130_aeb_sensor_config;
			p_tile_cfg = &ar0130_aeb_tile_config;
			p_gain_tbl = &ar0130_aeb_gain_table;
			p_sht_nl_tbl = &ar0130_aeb_sht_nl_table;
			p_wb_param = &ar0130_aeb_wb_param;
			p_ae_lines = &ar0130_aeb_expo_lines;
			break;
		case SENSOR_MT9T002:
			p_sensor_cfg = &mt9t002_aeb_sensor_config;
			p_tile_cfg = &mt9t002_aeb_tile_config;
			p_gain_tbl = &mt9t002_aeb_gain_table;
			p_sht_nl_tbl = &mt9t002_aeb_sht_nl_table;
			p_wb_param = &mt9t002_aeb_wb_param;
			p_ae_lines = &mt9t002_aeb_expo_lines;
			break;
		case SENSOR_AR0141:
			p_sensor_cfg = &ar0141_aeb_sensor_config;
			p_tile_cfg = &ar0141_aeb_tile_config;
			p_gain_tbl = &ar0141_aeb_gain_table;
			p_sht_nl_tbl = &ar0141_aeb_sht_nl_table;
			p_wb_param = &ar0141_aeb_wb_param;
			p_ae_lines = &ar0141_aeb_expo_lines;
			break;
		case SENSOR_AR0230:
			p_sensor_cfg = (hdr_method == HDR_BUILD_IN_METHOD)? \
				&ar0230_aeb_build_in_wdr_sensor_config: &ar0230_aeb_sensor_config;
			p_tile_cfg = &ar0230_aeb_tile_config;
			p_gain_tbl = &ar0230_aeb_gain_table;
			p_sht_nl_tbl = &ar0230_aeb_sht_nl_table;
			p_wb_param = &ar0230_aeb_wb_param;

			if(hdr_pipeline == HDR_PIPELINE_OFF){
				p_ae_lines = (hdr_method == HDR_BUILD_IN_METHOD)? \
					&ar0230_aeb_build_in_wdr_expo_lines: &ar0230_aeb_expo_lines;
			}else if(hdr_pipeline != HDR_PIPELINE_OFF && expo_num == HDR_2X){
				p_ae_lines = &ar0230_aeb_expo_lines_2x_hdr;
			}else{
				printf("error: can't find the proper ae lines for mode %d, expo num %d\n", \
					hdr_pipeline, expo_num);
				return -1;
			}
			break;
		case SENSOR_AR0237:
			p_sensor_cfg = &ar0237_aeb_sensor_config;
			p_tile_cfg = &ar0237_aeb_tile_config;
			p_gain_tbl = &ar0237_aeb_gain_table;
			p_sht_nl_tbl = &ar0237_aeb_sht_nl_table;
			p_wb_param = &ar0237_aeb_wb_param;

			if(hdr_pipeline == HDR_PIPELINE_OFF){
				p_ae_lines = &ar0237_aeb_expo_lines;
			}else if(hdr_pipeline != HDR_PIPELINE_OFF && expo_num == HDR_2X){
				p_ae_lines = &ar0237_aeb_expo_lines_2x_hdr;
			}else{
				printf("error: can't find the proper ae lines for mode %d, expo num %d\n", \
					hdr_pipeline, expo_num);
				return -1;
			}
			break;
		default:
			printf("undefined sensor id\n");
			return -1;
	}
	ae_gain_step =vin_aaa_info->agc_step;
	ae_tbl_num = p_ae_lines->header.total_tbl_num;
	if(ae_tbl_num > MAX_HDR_EXPOSURE_NUM){
		printf("error: total ae table number can't be greater than %d\n", MAX_HDR_EXPOSURE_NUM);
		return -1;
	}
	for(i = 0; i < ae_tbl_num; ++i){
		ae_tbl[i].p_expo_lines = p_ae_lines->expo_tables[i].expo_lines;
		ae_tbl[i].line_num = p_ae_lines->header.item_per_tbl[i];
		ae_tbl[i].belt = ae_tbl[i].line_num;
		ae_tbl[i].db_step = ae_gain_step;
		ae_tbl[i].p_gain_tbl = p_gain_tbl->gain_table;
		ae_tbl[i].gain_tbl_size = p_gain_tbl->header.item_per_tbl[0];
		ae_tbl[i].p_sht_nl_tbl = p_sht_nl_tbl->sht_nl_table;
		ae_tbl[i].sht_nl_tbl_size = p_sht_nl_tbl->header.item_per_tbl[0];
	}
	p_sensor_cfg->sensor_config.p_vindev_aaa_info =vin_aaa_info;
	img_config_sensor_info(&p_sensor_cfg->sensor_config);
	img_config_stat_tiles(&p_tile_cfg->tile_config);
	img_config_ae_tables(ae_tbl, ae_tbl_num);
	img_config_awb_param(&p_wb_param->wb_param);

	if(img_config_lens_info(lens_mount_id) < 0) {
			return -1;
	}
	if(img_load_lens_param(&lens_param_info) < 0) {
			return -1;
	}
	if(img_lens_init() < 0) {
			return -1;
	}
	if(p_auto_knee_cfg){
		img_config_auto_knee_info(p_auto_knee_cfg);
	}
	return 0;
}

int binding_param_with_sensor(int fd_iav,
					struct vindev_aaa_info* vin_aaa_info,
					img_config_info_t* p_img_config,
					amba_img_dsp_mode_cfg_t* p_ik_mode)
{
	u32 sensor_id;
	char sensor_name[128];
	sensor_id =(u32)vin_aaa_info->sensor_id;

	if(load_containers(sensor_id,sensor_name,p_img_config)<0)
		return -1;
	if(load_cc_bin(sensor_name)<0)
		return -1;
	if(enable_cc(fd_iav,sensor_name,p_ik_mode)<0)
		return -1;
	if(img_aaa_config(fd_iav, vin_aaa_info, p_img_config)<0)
		return -1;
	return 0;
}

