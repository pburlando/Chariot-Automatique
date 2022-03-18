/*  Programme de test des moteurs
    Projet CAPS
    Commande par gCode sur port USB
    G0 XspeedValue  Avancer à la vitesse speedValue
    G1 Xvalue Yvalue  Tourner à gauche vitesse Xvalue écart Yvalue
    G2 Xvalue Yvalue  Tourner à droite vitesse Xvalue écart Yvalue
    G3 Le robot tourne sur lui même à gauche 
    G4 Le robot tourne sur lui même à droite
    Si le robot ne reçoit pas de nouvelle commande au bout de 0,1s il s'arrête
*/

#include <Arduino.h>
#include "MotorsIboum17.h"
#include <GCodeParser.h>

MotorsIboum17 bot;      // Objet qui permet de commander les mouvements du robot
GCodeParser GCode = GCodeParser();    // Objet Gcode qui permet d'interpréter les commandes reçues au format texte sur le port série

unsigned long memTime = 0L;
int time_out;



void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  for(int i = 0; i < 10; i++) {
    digitalWrite(13, HIGH);
    delay(200);
    digitalWrite(13, LOW);
    delay(300);
  }
  Serial.begin(115200);
  Serial.println("Ready");
  delay(100);
  
  time_out = 1;    // Aucune commande reçue depuis 100ms
  memTime = millis();
}

void loop() {
  // put your main code here, to run repeatedly:

  // Timeout 100ms
  if (millis() - memTime >= 100)
  {
    memTime = millis();
    time_out = 1;
  }

  if(time_out)
  {
    bot.forward(0);
  }

  if (Serial.available() > 0)
  {
    if (GCode.AddCharToLine(Serial.read()))
    {
      GCode.ParseLine();
      // Code to process the line of G-Code here…

      Serial.print("Command Line: ");
      Serial.println(GCode.line);

      GCode.RemoveCommentSeparators();

      Serial.print("Comment(s): ");
      Serial.println(GCode.comments);
      
      if (GCode.HasWord('G'))
      {
        int gCodeNumber = (int)GCode.GetWordValue('G');
        Serial.print("Process G code: ");
        Serial.println(gCodeNumber);
        switch (gCodeNumber)
        {
        case 0:
          // Si G-Code contient la valeur de vitesse aller tout droit
          // Le robot recule si speedValue < 0
          if(GCode.HasWord('X'))
          {
            int speedValue = (int)GCode.GetWordValue('X');
            Serial.print("Process speedValue: ");
            Serial.println(speedValue);
            bot.forward(speedValue);
            time_out = 0;
            memTime = millis();  //réarmer le time out
          }
          break;
        
        case 1:
          if(GCode.HasWord('X') && GCode.HasWord('Y'))
          // Si G-Code contient les valeurs de vitesse et d'écart tourner à gauche
          {
            int speedValue = (int)GCode.GetWordValue('X');
            int gapValue = (int)GCode.GetWordValue('Y');
            Serial.print("Process speedValue: ");
            Serial.print(speedValue);
            Serial.print(" - gapValue: ");
            Serial.println(gapValue);
            bot.turnLeft(speedValue, gapValue);
            time_out = 0;
            memTime = millis();
          }
          break;

        case 2:
          if(GCode.HasWord('X') && GCode.HasWord('Y'))
          // Si G-Code contient les valeurs de vitesse et d'écart tourner à droite
          {
            int speedValue = (int)GCode.GetWordValue('X');
            int gapValue = (int)GCode.GetWordValue('Y');
            Serial.print("Process speedValue: ");
            Serial.print(speedValue);
            Serial.print(" - gapValue: ");
            Serial.println(gapValue);
            bot.turnRight(speedValue, gapValue);
            time_out = 0;
            memTime = millis();
          }
          break;

        case 3:
          /* Rotation à gauche sur lui même */
          bot.rotateLeft();
          time_out = 0;
          memTime = millis();
          break;

        case 4:
          /* Rotation à droite sur lui même */
          bot.rotateRight();
          time_out = 0;
          memTime = millis();
          break;
        
        default:
          /* Arrêt si commande inconnue */
          bot.forward(0);
          Serial.println("Commande inconnue");
          time_out = 0;
          memTime = millis();
          break;
        }
      }
    }
  }
}