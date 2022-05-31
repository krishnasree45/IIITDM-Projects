import cv2
import numpy as np
from cv2 import VideoWriter_fourcc
from multiprocessing import Manager

m = Manager()

digits_dict = m.dict()


w = 320
h = 240


def reconstruct_video_from_blocks():
    dt = np.dtype(np.int32)
    b = np.loadtxt("finalblue" + "file" + ".txt", dtype=dt)
    dt = np.dtype(np.int32)
    g = np.loadtxt("finalgreen" + "file" + ".txt", dtype=dt)
    dt = np.dtype(np.int32)
    r = np.loadtxt("finalred" + "file" + ".txt", dtype=dt)
    bc = []
    gc = []
    rc = []
    ba = []
    ga = []
    ra = []
    bd = []
    gd = []
    rd = []
    fourcc = VideoWriter_fourcc(*'mp4v')
    images = []

    out = cv2.VideoWriter('output.avi', cv2.VideoWriter_fourcc('M', 'J', 'P', 'G'), 15.0, (320, 240))
    for i in range(0, b.shape[1]):
        bc = (b[:, [i]])
        gc = (g[:, [i]])
        rc = (r[:, [i]])

        bd = np.array(bc)
        gd = np.array(gc)
        rd = np.array(rc)
        bd = bd.reshape(240, 320)
        gd = gd.reshape(240, 320)
        rd = rd.reshape(240, 320)

        img = cv2.merge((bd.astype(np.int), gd.astype(np.int), rd.astype(np.int)))
        # img = np.dstack((bd, gd, rd))

        cv2.imwrite(str(i) + '.png', img)
        images.append(str(i) + '.png')

    for image in images:
        out.write(cv2.imread(image))
    cv2.destroyAllWindows()
    out.release()
    print("stored ")


reconstruct_video_from_blocks()
