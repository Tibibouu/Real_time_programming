/*
 *  Fichier : accelero.c
 *  Package : Qualité Soft
 *  Description : Gestion de l'accéléromètre de la carte STM32
 *  Auteur : Valentin
 */

#include "accelero.h"
#include "tm_stm32f4_lis302dl_lis3dsh.h"

static TM_LIS302DL_LIS3DSH_t Axes_Data;


int8_t ACC_init(void){
	/* Detect proper device */
	if (TM_LIS302DL_LIS3DSH_Detect() == TM_LIS302DL_LIS3DSH_Device_LIS302DL) {
		/* Initialize LIS302DL */
		TM_LIS302DL_LIS3DSH_Init(TM_LIS302DL_Sensitivity_2_3G, TM_LIS302DL_Filter_2Hz);
	} else if (TM_LIS302DL_LIS3DSH_Detect() == TM_LIS302DL_LIS3DSH_Device_LIS3DSH) {
		/* Initialize LIS3DSH */
		TM_LIS302DL_LIS3DSH_Init(TM_LIS3DSH_Sensitivity_2G, TM_LIS3DSH_Filter_800Hz);
	} else {
		/* Device is not recognized */
		return 0;
	}
	return 1;
}

void ACC_read(void){
	/* Read axes data from initialized accelerometer */
    TM_LIS302DL_LIS3DSH_ReadAxes(&Axes_Data);
}

int16_t ACC_getX(void){
	return Axes_Data.X;
}

int16_t ACC_getY(void){
	return Axes_Data.Y;
}

int16_t ACC_getZ(void){
	return Axes_Data.Z;
}

