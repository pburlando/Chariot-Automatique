/*
 * MotorsIboum17.cpp
 *
 *  Created on: 3 déc. 2017
 *      Author: patrice
 */

#include "MotorsIboum17.h"
#include "Arduino.h"

const int LtPwmPin = 6;
const int RtPwmPin = 5;
const int LtDirectionPin = 11;
const int RtDirectionPin = 10;

MotorsIboum17::MotorsIboum17() {
	// TODO Auto-generated constructor stub
	// Reserved IO


	pinMode(LtDirectionPin, OUTPUT);
	pinMode(RtDirectionPin, OUTPUT);

}

void MotorsIboum17::setRtSpeed(int spd) {
	/*
	 * Fixe la vitesse et le sens du moteur droit
	 * -255 <= speed <= 255
	 * speed > 0 ==> sens horaire
	 */
	int speed = spd;
	int dir = 0;
	if(speed < 0) {
		speed = 255 + speed;  // Quand dir=1 on envoie le complément du signal PWM
		dir = 1;
	}
	if (speed > 255)
		speed = 255;
	digitalWrite(RtDirectionPin, dir);
	analogWrite(RtPwmPin,speed);
}


void MotorsIboum17::setLtSpeed(int spd) {
	/*
	 * Fixe la vitesse et le sens du moteur gauche
	 * -255 <= speed <= 255
	 * speed > 0 ==> sens horaire
	 */
	int speed = spd;
	int dir = 0;
	if(speed < 0) {
		speed = 255 + speed;
		dir = 1;
	}
	if (speed > 255)
		speed = 255;
	digitalWrite(LtDirectionPin, dir);
	analogWrite(LtPwmPin,speed);
}

void MotorsIboum17::setSpeeds(int ltSpd, int rtSpd) {
	/*
	 * Fixe la vitesse des moteurs gauche et droit
	 */

	setLtSpeed(ltSpd);
	setRtSpeed(rtSpd);
}

void MotorsIboum17::forward(int spd) {
	setSpeeds(spd,spd);
}

void MotorsIboum17::rotateLeft(void) {
	setSpeeds(-80,80);
}

void MotorsIboum17::rotateRight(void) {
	setSpeeds(80,-80);
}

void MotorsIboum17::turnLeft(int spd, int gp) {
	int gap = gp;
	int speed = spd;

	if (speed < 0)
		speed = -speed;

	if (gap < 0)
		gap = -gap;
	if (gap > 100)
		gap = 100;

	setSpeeds(speed - gap*speed/100, speed);
}

void MotorsIboum17::turnRight(int spd, int gp) {
	int gap = gp;
	int speed = spd;

	if (speed < 0)
		speed = -speed;

	if (gap < 0)
		gap = -gap;
	if (gap > 100)
		gap = 100;

	setSpeeds(speed, speed - gap*speed/100);
}

void MotorsIboum17::reverse(int spd) {
	int speed =spd;
	if (speed < 0)
			speed = -speed;
	setSpeeds(-speed, -speed);
}
