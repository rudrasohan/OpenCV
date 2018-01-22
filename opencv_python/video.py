import numpy as np
import matplotlib.pyplot as plt
import cv2

yellow_HSV_th_min = np.array([0, 70, 70])
yellow_HSV_th_max = np.array([50, 255, 255])

cap = cv2.VideoCapture('project_video.mp4')

while (cap.isOpened()):
    ret, frame = cap.read();
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    min_th_ok = np.all(hsv > yellow_HSV_th_min, axis=2)
    #print min_th_ok
    max_th_ok = np.all(hsv < yellow_HSV_th_max, axis=2)
    out = np.logical_and(min_th_ok,max_th_ok)
    im = np.array(out * 255, dtype = np.uint8)
    #print (int)out
    cv2.imshow('frame',im)
    if (cv2.waitKey(1) & 0xFF == ord('q')):
        break
    pass
cap.release()
cv2.destroyAllWindows()
