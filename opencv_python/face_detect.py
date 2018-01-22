# import the necessary packages
import numpy as np
import argparse
import cv2
 
# construct the argument parse and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-v", "--video",
	help = "path to the (optional) video file")
args = vars(ap.parse_args())
 
# define the upper and lower boundaries of the HSV pixel
# intensities to be considered 'skin'
lower = np.array([0, 100, 100], dtype = "uint8")
upper = np.array([10, 255, 255], dtype = "uint8")
# if a video path was not supplied, grab the reference
# to the gray
if not args.get("video", False):
	camera = cv2.VideoCapture(0)
 
# otherwise, load the video
else:
	camera = cv2.VideoCapture(args["video"])
# keep looping over the frames in the video
while True:
	# grab the current frame
	(grabbed, frame) = camera.read()
 
	# if we are viewing a video and we did not grab a
	# frame, then we have reached the end of the video
	if args.get("video") and not grabbed:
		break
 
	# resize the frame, convert it to the HSV color space,
	# and determine the HSV pixel intensities that fall into
	# the speicifed upper and lower boundaries
	height = np.size(frame,0)
	frame = cv2.resize(frame,(400,height))
	converted = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
	whiteMask = cv2.inRange(converted, lower, upper)
 
	# apply a series of erosions and dilations to the mask
	# using an elliptical kernel
	kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (11, 11))
	whiteMask = cv2.erode(whiteMask, kernel, iterations = 2)
	whiteMask = cv2.dilate(whiteMask, kernel, iterations = 2)
 
	# blur the mask to help remove noise, then apply the
	# mask to the frame
	whiteMask = cv2.GaussianBlur(whiteMask, (3, 3), 0)
	white = cv2.bitwise_and(frame, frame, mask = whiteMask)
 
	# show the skin in the image along with the mask
	cv2.imshow("images", np.hstack([frame, white]))
 
	# if the 'q' key is pressed, stop the loop
	if cv2.waitKey(1) & 0xFF == ord("q"):
		break
 
# cleanup the camera and close any open windows
camera.release()
cv2.destroyAllWindows()