from cv2 import cv2
import numpy as np

color=35
lo=np.array([color-5, 100, 50])
hi=np.array([color+5, 255,255])
color_info=(0, 0, 255)
cap=cv2.VideoCapture(0)
while True:
    ret, frame=cap.read()
    image=cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    mask=cv2.inRange(image, lo, hi)
    mask=cv2.erode(mask, None, iterations=2)
    mask=cv2.dilate(mask, None, iterations=2)
    elements=cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)[-2]
    if len(elements) > 0:
        c=max(elements, key=cv2.contourArea)
        ((x, y), radius)=cv2.minEnclosingCircle(c)
        if radius>30:
            cv2.putText(mask, "x= "+ str(x), (30,30), cv2.FONT_HERSHEY_DUPLEX, 1, (255,255,255), 1, cv2.LINE_AA)
    cv2.imshow('Mask', mask)
    if cv2.waitKey(1)&0xFF==ord('q'):
        break
    try:
        if  x < 285:
            print("gauche")
        elif x > 355:
            print("droite")
        else:
            print("ok")
    except:
        pass
cap.release()
cv2.destroyAllWindows()