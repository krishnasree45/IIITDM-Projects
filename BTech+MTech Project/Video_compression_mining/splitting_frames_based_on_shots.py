import cv2
from shot_boundary_detection import sbd_eucledian_kmeans
video_name = 'SampleVideo_320x240_640kb_1st_shot.mp4'
cap = cv2.VideoCapture(video_name)

