/*************************************
* Ecouter le port série et afficher 
* les données entrantes
*
**************************************/
#include <Arduino.h>


void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Serial.println("Ready");
  delay(100);
}

void loop() {
  // put your main code here, to run repeatedly:
  long val = 12;
  char commande;
  if (Serial.available() > 0)
  {
    String chaine_recu = Serial.readStringUntil('\n');
    //Serial.println(chaine_recu);
    commande = chaine_recu.charAt(0);
    chaine_recu.remove(0,1);
    val = chaine_recu.toInt();
    Serial.print("Commande ");
    Serial.print(commande);
    Serial.print("\t Valeur X 2 : ");
    Serial.println(val*2);
  }
}

