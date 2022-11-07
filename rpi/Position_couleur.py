'''
Programme de détection d'une couleur dans le champs de la caméra
Définition caméra = 640 x 480
Le rayon et la position de la forme dans le champs sont déterminés.
Unités = pixels
Les trois informations sont envoyées au robot par le port série
via le périphérique USB.
Temps de boucle avec affichage de la vidéo sur la sortie HDMI du Rpi environ 80ms
Temps de boucle sans affichage de la vidéo environ 35ms
Temps d'initialisation des périphériques USB et vidéo environ 2s !!
'''

import communication_robot
import cv2
import numpy as np
from time import time, sleep

robot = communication_robot.connexionRobot()
robot.ouvrir_port()
sleep(1)  # Attendre le démarrage de l'arduino à l'ouverture du port série

color=164  # Valeur de teinte pour la détection du masque (de 0 à 180)
COLOR_INFO=(128, 128, 128)  # gris moyen pour un espace colorimétrique RVB

cap=cv2.VideoCapture(2)
#ret = cap.set(cv2.CAP_PROP_FRAME_WIDTH, 320)  # redimensionne la largeur de l'image à 320 px
#ret = cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 240) # redimensionne la hauteur de l'image à 240 px

while True:
    start_time = time()
    radius = 0
    x, y = 0, 0   # Position de la cible dans l'image
    if cv2.waitKey(33) == ord('a'):
        if color > 10:
            color -= 5  # Diminuer la valeur de teinte de 5
    elif cv2.waitKey(33) == ord('z'):
        if color < 180:
            color += 5  # Augmenter la valeur de teinte de 5
    elif cv2.waitKey(1)&0xFF==ord('q'):
        robot.fermer_port()
        break  # Sortir de la boucle while true et terminer l'aquisition vidéo

    lo=np.array([color-5,50, 50])   # Valeurs minimales des composantes TSV pour activer le masque TSVmin = 0
    hi=np.array([color+5, 200,200])   # Valeurs maximales des composantes TSV pour activer le masque Tmax = 180, SVmax = 255
    ret, frame=cap.read()
    image=cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)  # Changer l'espace colorimétrique des pixels de valeurs RVB en valeurs TSV
    mask=cv2.inRange(image, lo, hi)  # Créer une image binaire en appliquant un masque colorimétrique
    mask=cv2.erode(mask, None, iterations=2)
    mask=cv2.dilate(mask, None, iterations=2)
    elements=cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)[-2]
    cv2.putText(mask, "color =" + str(color),(30,60),cv2.FONT_HERSHEY_DUPLEX, 1, COLOR_INFO, 1, cv2.LINE_AA)
    if len(elements) > 0:
        c=max(elements, key=cv2.contourArea)
        ((x, y), radius)=cv2.minEnclosingCircle(c)
        x = int(x)
        y = int(y)
        radius = int(radius)
        if radius>10:
            cv2.putText(mask, f"x = {x} y = {y} rad = {radius}", (30,30), cv2.FONT_HERSHEY_DUPLEX, 1, COLOR_INFO, 1, cv2.LINE_AA)

    else:
        x, y, rad = 0, 0, 0
    robot.ecrire(f"X{x}\r\n")
    robot.ecrire(f"Y{y}\r\n")
    robot.ecrire(f"R{radius}\r\n")
    cv2.imshow('Mask', mask)
    cv2.imshow('Image', image)
    print(f"Temps exécution boucle = {(time() - start_time) * 1000:0.3f} ms")  #debug


            

    
cap.release()
cv2.destroyAllWindows()

