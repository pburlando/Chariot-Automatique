'''
    Programme de test communication série RPI Arduino
    Test du parser de commande coté arduino
    Envoie des commandes suivies de valeurs
    Sur le port série'''

import serial
from time import sleep
with serial.Serial('/dev/ttyUSB0', 115200, timeout=1) as ser:
    for cpt in range(100):
        if cpt%2:
            chaine = ('W' + str(cpt) + '\r\n').encode()
        else:
            chaine = ('H' + str(cpt) + '\r\n').encode()
        ser.write(chaine)
        print(f"Chaîne envoyée : {chaine}")
        print(ser.readline().decode())
        #sleep(0.05)
    ser.close()