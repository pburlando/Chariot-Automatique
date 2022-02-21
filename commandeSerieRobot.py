import threading
import time
from time import sleep
import serial
import serial.tools.list_ports as lp

class connexionRobot():
    def __init__(self):
        self.port = None
        self.serial = serial.Serial()
        self.serial.timeout = 0.5
        self.serial.baudrate = 9600

    def _trouvePort(self):
        for port,desc, hwid in lp.grep('CP2102 USB to UART Bridge Controller'):
            self.port = port
            return port
        else:
            raise serial.SerialException('Périphérique non trouvé')

    def ouvrir_port(self):
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

    def commande(self, *args):
        if(len(args) < 2):
            commande = args[0]
            vitesse = 80
        else:
            commande, vitesse = args

        if vitesse > 255:
            vitesse = 255

        if vitesse < -255:
            vitesse = -255

        val = commande + chr(vitesse)
        self.serial.write(val.encode())


class RepeatedTimer(object):
    def __init__(self, interval, function, *args, **kwargs):
        self._timer = None
        self.interval = interval
        self.function = function
        self.args = args
        self.kwargs = kwargs
        self.is_running = False
        self.next_call = time.time()
        self.start()

    def _run(self):
        self.is_running = False
        self.start()
        self.function(*self.args, **self.kwargs)

    def start(self):
        if not self.is_running:
            self.next_call += self.interval
            self._timer = threading.Timer(self.next_call - time.time(), self._run)
            self._timer.start()
            self.is_running = True

    def stop(self):
        self._timer.cancel()
        self.is_running = False


def hello(name):
    print(name)

robot = connexionRobot()
robot.ouvrir_port()

for vitesse in range(0, 250, 10):
    for boucle in range(5):
        print(vitesse)
        robot.commande('F', vitesse)
        sleep(0.1)

# print("starting...")
# rt = RepeatedTimer(0.1, robot.commande, "FFF")  # it auto-starts, no need of rt.start()
# sleep(1)  # your long-running job goes here...
# rt.stop()  # better in a try/finally block to make sure the program ends!
# rt = RepeatedTimer(0.1, robot.commande, "LFF")  # it auto-starts, no need of rt.start()
# sleep(1)  # your long-running job goes here...
# rt.stop()  # better in a try/finally block to make sure the program ends!

robot.fermer_port()
