/*
 * MotorsIboum17.h
 *
 *  Created on: 3 déc. 2017
 *      Author: patrice
 */


// A DOCUMENTER !!!!!!!!!!!!!!!!!

#ifndef MOTORSIBOUM17_H_
#define MOTORSIBOUM17_H_

class MotorsIboum17 {
public:
	MotorsIboum17();						//Constructeur
	void setRtSpeed(int spd);				//Fixe la vitesse du moteur droit
	void setLtSpeed(int spd);				//Fixe la vitesse du moteur gauche
	void setSpeeds(int ltSpd, int rtSpd);  	//Fixe la vitesse des moteurs gauche et droit
	void forward(int spd);					//Avancer à la vitesse 0 <= spd <= 255
	void rotateLeft(void);					// Rotation vers la gauche autour du centre de l'axe des roues. Vitesse réduite fixe
	void rotateRight(void);					// Rotation vers la droite autour du centre de l'axe des roues. Vitesse réduite fixe
	void turnLeft(int spd, int gp);			// Virage à gauche à la vitesse 0 <= spd <= 255, la vitesse de la roue gauche est réduite de spd * gp/ 100 avec 0 <= gp <= 100
	void turnRight(int spd, int gp);		// Virage à droite à la vitesse 0 <= spd <= 255, la vitesse de la roue drote est réduite de spd * gp/ 100 avec 0 <= gp <= 100
	void reverse(int spd);					// Marche arrière à la vitesse 0 <= spd <= 255
};

#endif /* MOTORSIBOUM17_H_ */
