import threading
import time
from time import sleep
import serial
import serial.tools.list_ports as lp

class connexionRobot():
    def __init__(self):
        self.port = None
        self.serial = serial.Serial()
        self.serial.timeout = 0.1
        self.serial.baudrate = 115200

    def _trouvePort(self):
        '''
        retrouve le nom d'un port à partir de sa description
        :return: le chemin du périphérique
        '''
        for port,desc, hwid in lp.grep('2341:0042'):
            self.port = port
            return port
        else:
            raise serial.SerialException('Périphérique non trouvé')

    def ouvrir_port(self):
        '''
        Tente d'ouvrir le port série
        :return: None en cas de succès, la nature de l'erreur sinon.
        '''
        try:
            port = self._trouvePort()
        except serial.SerialException as e:
            print(f"Erreur: {e}")
            return -1
        self.serial.port = port
        try:
            self.serial.open()
        except serial.SerialException as e:
            print(f"Erreur: {e}")
        print(f"{port} est ouvert")

    def fermer_port(self):
        if self.serial.isOpen():
            self.serial.close()
            print(f"{self.port} est fermé")

    def commande(self, args):
        self.serial.write(args.encode())
        
    def ecrire(self, trame):
        self.serial.write(trame.encode())
        #print(trame)  #debug !!

    def lire(self):
        trame = self.serial.readline()
        return trame.decode()

#############################################################
# Programme de test du module de communication avec le robot
#############################################################

if __name__ == '__main__':
    from time import sleep
    robot = connexionRobot()
    robot.ouvrir_port()
    sleep(1)  # Attendre le boot de l'arduino à l'ouverture du port
    for cpt in range(100):
        if cpt % 2:
            chaine = ('W' + str(cpt) + '\r\n')
        else:
            chaine = ('H' + str(cpt) + '\r\n')
        robot.ecrire(chaine)
        print(f"Chaîne envoyée : {chaine}")
        print(robot.lire())
    robot.fermer_port()





