/*************************************
* Commande du robot caps 
* Le Rpi envoi la position et la taille de la cible
* Ces informations sont déterminées par analyse des
* images acquises par une camera
* Le Rpi est vu comme un capteur qui rafraichit
* les données toutes les 80ms environ
*
**************************************/
#include <Arduino.h>
#include "MotorsIboum17.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define VMAX 160

MotorsIboum17 bot;      // Objet qui permet de commander les mouvements du robot
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display



unsigned long memTime = 0L;
unsigned long memTimeSerial = 0L;

unsigned int val = 0;
unsigned int val_x=0;
unsigned int val_y=0;
unsigned int val_r=0;
char commande;

void setup() {
  // Attente démarrage 5s
  lcd.init();
  lcd.backlight();
  lcd.print("Demarrage");
  pinMode(LED_BUILTIN, OUTPUT);
  // for(int i = 0; i < 10; i++) {
  //   digitalWrite(LED_BUILTIN, HIGH);
  //   delay(200);
  //   digitalWrite(LED_BUILTIN, LOW);
  //   delay(300);
  // }
  Serial.begin(115200);
  memTime = millis();
  memTimeSerial = millis();
}

void loop() {
  // put your main code here, to run repeatedly:

  // A faire le plus fréquemment possible
  if (Serial.available() > 0)
  {
    memTimeSerial = millis();
    String chaine_recu = Serial.readStringUntil('\n');
    commande = chaine_recu.charAt(0);
    chaine_recu.remove(0,1);
    val = chaine_recu.toInt();
    switch(commande) 
    {
      case 'X':
        val_x = val;
        break;

      case 'Y':
        val_y = val;
        break;
      
      case 'R':
        val_r = val;
        break;

      default:
        val_r = 0;
        break;
    }
  }
  else if (millis() - memTimeSerial > 200)
  {
    memTimeSerial = millis();
    val_r = 0;
  }

  // A faire toutes les 100ms
  if (millis() - memTime >= 100)
  {
    memTime = millis();

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(val_x);
    lcd.setCursor(8,0);
    lcd.print(val_y);
    lcd.setCursor(0,1);
    lcd.print(val_r);
   
    if (val_r < 25)
    {
      bot.setSpeeds(0, 0);
    }
    else
    {
      int ecart = val_x - 320;
      if (ecart < -20)
      {
        int vitesse_droite = VMAX + ecart;
        if (vitesse_droite < 0)
          vitesse_droite = 0;
        bot.setSpeeds(vitesse_droite, VMAX);  // Tourner à droite proportionnellement à l'écart
      }

      else if (ecart > 20)
      {
        int vitesse_gauche = VMAX - ecart;
        if (vitesse_gauche < 0)
          vitesse_gauche = 0;
        bot.setSpeeds(VMAX, vitesse_gauche);  // Tourner à gauche proportionnellement à l'écart
      }

      else
      {
        bot.setSpeeds(VMAX, VMAX);  // Aller tout droit
      }
    }
  }
}

