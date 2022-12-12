/*
 *  Fichier : accelero.h
 *  Package : Qualit� Soft
 *  Description : Gestion de l'acc�l�rom�tre de la carte STM32
 *  Auteur : Valentin
 */

#ifndef ACCELEROMETRE_H
	#define ACCELEROMETRE_H

	#include <stdint.h>

	//Proc�dure initialisant l'acc�l�rom�tre
	int8_t ACC_init(void);

	//Proc�dure lisant les valeurs donn�es par l'acc�l�rom�tre et les sauvegardes dans les variables X, Y et Z
	void ACC_read(void);

	//Proc�dure donnant acc�s � la variable X
	int16_t ACC_getX(void);

	//Proc�dure donnant acc�s � la variable Y
	int16_t ACC_getY(void);

	//Proc�dure donnant acc�s � la variable Z
	int16_t ACC_getZ(void);


#endif /* ACCELEROMETRE_H */
