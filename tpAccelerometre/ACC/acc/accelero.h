/*
 *  Fichier : accelero.h
 *  Package : Qualité Soft
 *  Description : Gestion de l'accéléromètre de la carte STM32
 *  Auteur : Valentin
 */

#ifndef ACCELEROMETRE_H
	#define ACCELEROMETRE_H

	#include <stdint.h>

	//Procédure initialisant l'accéléromètre
	int8_t ACC_init(void);

	//Procédure lisant les valeurs données par l'accéléromètre et les sauvegardes dans les variables X, Y et Z
	void ACC_read(void);

	//Procédure donnant accès à la variable X
	int16_t ACC_getX(void);

	//Procédure donnant accès à la variable Y
	int16_t ACC_getY(void);

	//Procédure donnant accès à la variable Z
	int16_t ACC_getZ(void);


#endif /* ACCELEROMETRE_H */
