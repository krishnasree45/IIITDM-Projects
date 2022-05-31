
from functools import reduce

import cv2
from sklearn.cluster import KMeans


def eucledian_distance(hist1, hist2):
    row = len(hist1)
    col = len(hist1[0])
    if len(hist2) != row or len(hist2[0]) != col:
        return False

    # normalization each histogram
    sum1 = 0
    sum2 = 0

    for val in range(row):
        a1 = reduce(lambda a, b: a+b, hist1[val], 0)
        a2 = reduce(lambda a, b: a+b, hist2[val], 0)
        sum1 += a1
        sum2 += a2
    for val in range(row):
        hist1[val] = list(map(lambda a: float(a)/sum1, hist1[val]))
        hist2[val] = list(map(lambda a: float(a)/sum2, hist2[val]))

    # measuring eucledian distance
    distance = 0
    for val in range(row):
        for j in range(col):
            distance += abs(hist1[val][j] - hist2[val][j])
    return distance


def sbd_wrapper():
    n_rows = 3  # row number
    n_images_per_row = 3  # to split 9 pieces
    fc = 0  # frame_counter
    images = []  # frames
    hist = []  # histograms
    video_name = 'SampleVideo_320x240_1mb.mp4'
    # import ipdb;ipdb.set_trace()
    cap = cv2.VideoCapture(video_name)
    property_id = int(cv2.CAP_PROP_FRAME_COUNT)
    total_number_of_frames = int(cv2.VideoCapture.get(cap, property_id))
    print("frame count ", total_number_of_frames)
    if cap.isOpened() is False:
        print("Error opening video stream or file")
        exit(0)

    while cap.isOpened():
        # Capture frame-by-frame
        ret, frame = cap.read()
        if not ret:
            break

        hueFrames = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)  # convert Hue value
        height, width, ch = hueFrames.shape  # find shape of frame and channel

        roi_height = int(height / n_rows)  # to divide 9 pieces find height value for each frame
        roi_width = int(width / n_images_per_row)  # to divide 9 pieces find width value for each frame

        # ROI part
        images.append([])  # first row frame_id, column raw piece_id
        hist.append([])  # first row frame_id, column raw piece_id
        for x in range(0, n_rows):
            for y in range(0, n_images_per_row):
                # i am splitting into 9 pieces of each frame and added to the images 2d matrix
                # row defines frame_id and column defines piece_id
                tmp_image = hueFrames[x*roi_height:(x+1)*roi_height, y*roi_width:(y+1)*roi_width]
                images[fc].append(tmp_image)

        # Display the resulting sub-frame and calc local histogram values
        for i in range(0, n_rows*n_images_per_row):
                hist[fc].append(cv2.calcHist([images[fc][i]], [0], None, [256], [0, 256]))
        fc += 1  # frame counter 1 up

        if cv2.waitKey(1) & 0xFF == ord('q'):  # if something gets wrong about frame it'll ended.
            break
    dist = []  # distance list

    # calculate eucledian distance
    for i in range(0, len(hist)-1):
        dist.append(eucledian_distance(hist[i], hist[i+1]))  # calculate for each 2 frame

    clt = KMeans(n_clusters=2)   # n is cluster number
    clt.fit(dist)  # calculate K means
    big_center = 1  # select which cluster includes shot frames

    shots = []  # shots list. List will be include frame id

    for i in range(0, len(clt.labels_)):
        if big_center == clt.labels_[i]:
            # shot frame id is appending to the list
            shots.append(i+1)

    # after we're done we'll destroy everything
    cap.release()
    cv2.destroyAllWindows()

    # from here, this part for store the shot frames under directory
    cap = cv2.VideoCapture(video_name)
    shot_frames = list()
    shot_frames.append(0)
    fc = 0  # frame_counter
    i = 0
    print("Before storing the frames")
    while cap.isOpened():
        # Capture frame-by-frame
        ret, frame = cap.read()
        if not ret:
            break
        if i < len(shots) and shots[i] == fc:
            # if captured frame is from my shot list then we'll store the frame
            cv2.imwrite("shot_"+str(fc)+".png", frame)
            shot_frames.append(fc)
            # print("While storing the frames")
            i += 1
        elif i == len(shots):
            # if we reached out end of the shot list we'll exit
            break

        fc += 1

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    # print("frames from", count_of_frames, "to", total_number_of_frames)
    print("Done with storing the shots frames")
    cap.release()
    cv2.destroyAllWindows()
    shot_frames.append(total_number_of_frames)
    return shot_frames


if __name__ == "__main__":
    shot_frames = sbd_wrapper()
    print(shot_frames)
