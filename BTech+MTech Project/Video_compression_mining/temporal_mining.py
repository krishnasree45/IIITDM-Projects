import cv2
import image_slicer
import os

print("Done")
video_name = 'SampleVideo_320x240_640kb_1st_shot.mp4'
cap = cv2.VideoCapture(video_name)
if cap.isOpened() is False:
    print("Error opening video stream or file")
    exit(0)

frame_count = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))
height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
# frame_no = 1
# cap.set(1, frame_no)
# ret, frame = cap.read()
# cv2.imshow('output', frame)
# cv2.imwrite('frame_1.png', frame)
# cv2.waitKey()
count = 0
# success = True
while count < frame_count:
    success, image = cap.read()
    cv2.imwrite('/home/krishna/final_project/codes/pca+c+mining/sliced_frames/frame_'+str(count)+'.png', image)
    current_dir = os.getcwd()
    os.chdir('/home/krishna/final_project/codes/pca+c+mining/sliced_frames')
    image_slicer.slice('frame_'+str(count)+'.png', 8)
    os.chdir(current_dir)
    try:
        os.remove('/home/krishna/final_project/codes/pca+c+mining/sliced_frames/frame_'+str(count)+'.png')
    except:
        print('Image not found for deletion')
        pass
    count += 1
