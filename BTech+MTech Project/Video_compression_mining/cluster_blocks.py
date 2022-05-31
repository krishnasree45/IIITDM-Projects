import heapq
import subprocess
import time
from multiprocessing import Manager
from multiprocessing import Process
import os
import cv2
import numpy as np
from scipy.cluster.vq import kmeans, vq

m = Manager()
n1 = 1


class Node(object):
    def __init__(self, pairs, frequency):
        # print pairs
        self.pairs = pairs
        self.frequency = frequency

    def __repr__(self):
        return repr(self.pairs) + ", " + repr(self.frequency)

    def merge(self, other):
        total_frequency = self.frequency + other.frequency
        for p in self.pairs:
            p[1] = "1" + p[1]
        for p in other.pairs:
            p[1] = "0" + p[1]
        new_pairs = self.pairs + other.pairs
        return Node(new_pairs, total_frequency)

    def __lt__(self, other):
        return self.frequency < other.frequency


def block_shaped(arr, nrows, ncols):
    ncols = int(ncols)
    h, w = arr.shape
    return arr.reshape(h // nrows, nrows, -1, ncols).swapaxes(1, 2).reshape(-1, nrows, ncols)

    # val = arr.shape
    # ncols = int(ncols)
    # # import ipdb;ipdb.set_trace()
    # return arr.reshape(val[0] // nrows, nrows, -1, ncols).swapaxes(1, 2).reshape(-1, nrows, ncols)


def un_block_shaped(arr, h, w):
    n, nrows, ncols = arr.shape
    return (arr.reshape(h // nrows, -1, nrows, ncols)
            .swapaxes(1, 2)
            .reshape(h, w))


def get_all_video_related_dimensions_components():
    video_name = 'SampleVideo_320x240_1mb.mp4'
    cap = cv2.VideoCapture(video_name)
    if cap.isOpened() is False:
        print("Error opening video stream or file")
        return
    frame_count = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))
    height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
    width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
    count = 1
    success, image = cap.read()
    # import ipdb;ipdb.set_trace()
    blue, green, red = image[:, :, 0], image[:, :, 1], image[:, :, 2]

    zb = blue.reshape((blue.shape[0] * blue.shape[1]), 1)
    zg = green.reshape((green.shape[0] * green.shape[1]), 1)
    zr = red.reshape((red.shape[0] * red.shape[1]), 1)
    zb = np.c_[zb]
    zg = np.c_[zg]
    zr = np.c_[zr]
    success, image = cap.read()
    while success:
        # (blue,green,red)= cv2.split(image)
        blue, green, red = image[:, :, 0], image[:, :, 1], image[:, :, 2]

        data_blue = blue.reshape((blue.shape[0] * blue.shape[1]), 1)
        data_green = green.reshape((green.shape[0] * green.shape[1]), 1)
        data_red = red.reshape((red.shape[0] * red.shape[1]), 1)
        zb = np.c_[zb, data_blue]
        zg = np.c_[zg, data_green]
        zr = np.c_[zr, data_red]

        success, image = cap.read()
        count += 1

    # if count > 10:
    # break
    np.savetxt('blue_file.txt', zb, fmt='%i')
    np.savetxt('green_file.txt', np.c_[zg], fmt='%i')
    np.savetxt('red_file.txt', np.c_[zr], fmt='%i')

    cap.release()
    cv2.destroyAllWindows()
    # print("frame_count : {}".format(str(frame_count)))
    return height, width, frame_count


def clustering(string, filename, shot_number):
    print("entered clustering")
    dt = np.dtype(np.float32)
    b = np.loadtxt(filename + ".txt", dtype=dt)
    z = b.reshape(1, b.shape[0] * b.shape[1])

    number_of_clusters = 8
    print("Before kmeans")
    centroids, _ = kmeans(z[0].astype(float), number_of_clusters)
    print("After k means")
    k = 0
    red_cluster = open(string + "Cluster" + shot_number + ".txt", "w")

    # Write centroids of clusters into RedClusterTable1.txt
    with open(string + "ClusterTable" + shot_number + ".txt", "w") as redClusterTable:
        for identifier in centroids:
            redClusterTable.write(str(k))
            redClusterTable.write(" " + str(identifier) + "\n")
            k += 1
    idx, _ = vq(z[0], centroids)
    c = 1
    a = list()
    for cid in idx:
        red_cluster.write(str(cid) + " ")
        a.append(cid)
        c += 1
        if c == b.shape[1] + 1:
            red_cluster.write("\n")
            c = 1
    print("clustering done")


def run_clustering_all_shots_parallel(component):
    proc = []
    for i in range(0, n1):
        filename = component + "_file"
        print("FILE NAME ", filename)
        p = Process(target=clustering, args=(component, filename, str(i),))
        p.start()
        proc.append(p)
    for p in proc:
        p.join()


def run_clustering_all_components_parallel():
    proc = []
    for component in ["blue", "green", "red"]:
        p = Process(target=run_clustering_all_shots_parallel, args=(component,))
        p.start()
        proc.append(p)
    for p in proc:
        p.join()
    print("run_clustering_all_components_parallel done")


def mining(string, filename, block_number):
    filename1 = string + "patterns" + block_number + ".txt"
    subprocess.call(['java', '-jar', 'spmf.jar', 'run', 'CM-ClaSP', filename, filename1, '5%'])


def miner(string, filename, shot_number):
    red_file = []
    dt = np.dtype(np.int32)
    b = np.loadtxt(filename, dtype=dt)
    # import ipdb;ipdb.set_trace()
    with open(filename, "r") as r:
        # import ipdb;ipdb.set_trace()
        for line in r:
            line = line.replace("\n", "")
            red_file.append(" " + line + " ")
    # print red_file
    # Find all one length sequences
    frequent_patterns = {}
    current_length = 1
    frequent_patterns[current_length] = {}
    number_of_clusters = 8
    minimum_support = (b.shape[0] * 9) / 100
    print(filename, "min sup=", minimum_support)

    columns = b.shape[1]
    # import ipdb;ipdb.set_trace()
    for i in range(0, number_of_clusters):
        c_key = " " + str(i) + " "
        for eachLine in red_file:
            if c_key in eachLine:
                if c_key in frequent_patterns[current_length].keys():
                    frequent_patterns[current_length][c_key] += 1
                else:
                    frequent_patterns[current_length][c_key] = 1
                    # print str(frequent_patterns[current_length][c_key]) + filename + "key value " + c_key

    # print frequentPattern
    # Enumerate length 2 frequent sequences
    current_length = 2
    frequent_patterns[current_length] = {}
    # import ipdb;ipdb.set_trace()
    for key1 in frequent_patterns[current_length - 1].keys():
        for key2 in frequent_patterns[current_length - 1].keys():
            new_key = key1.rstrip() + key2
            frequent_patterns[current_length][new_key] = 0
    # import ipdb;ipdb.set_trace()

    for eachKey in frequent_patterns[current_length].keys():
        for line in red_file:
            if eachKey in line:
                frequent_patterns[current_length][eachKey] += 1

    dummy_frequent_patterns = frequent_patterns[current_length].copy()

    # import ipdb;ipdb.set_trace()

    # size_of_dict = len(frequent_patterns)
    for key in dummy_frequent_patterns.keys():
        if frequent_patterns[current_length][key] < minimum_support:
            frequent_patterns[current_length].pop(key, None)
    current_length += 1
    # print frequent_patterns
    # Enumerate all frequent sequences
    # import ipdb;ipdb.set_trace()

    for i in range(current_length, columns + 1):
        # print i
        # if(len(frequent_patterns[i].keys())!=0):
        frequent_patterns[i] = {}
        # Generate the i-length keys
        for key1 in frequent_patterns[i - 1].keys():
            for key2 in frequent_patterns[i - 1].keys():
                mkey1 = key1.lstrip().rstrip().split(" ")
                mkey2 = key2.lstrip().rstrip().split(" ")
                if mkey1[1:len(mkey1)] == mkey2[0:len(mkey2) - 1]:
                    key_to_split = key2.lstrip().rstrip().split(" ")
                    new_key = key1 + key_to_split[len(key_to_split) - 1] + " "
                    frequent_patterns[i][new_key] = 0
        # print "Loop"
        # Find the support of each i-length key
        for line in red_file:
            for eachKey in frequent_patterns[i].keys():
                if eachKey in line:
                    frequent_patterns[i][eachKey] += 1

        dummy_frequent_patterns = frequent_patterns[i].copy()
        # Pop all sequences that don't satisfy the minimum support threshold
        for key in dummy_frequent_patterns.keys():
            if frequent_patterns[i][key] < minimum_support:
                frequent_patterns[i].pop(key, None)

        # Pop all non closed sequences
        dummy_frequent_patterns = frequent_patterns[i].copy()
        second_dummy_frequent_patterns = frequent_patterns[i - 1].copy()
        for key1 in dummy_frequent_patterns.keys():
            for key2 in second_dummy_frequent_patterns.keys():
                if (key2 in key1) and (frequent_patterns[i][key1] == second_dummy_frequent_patterns[key2]):
                    frequent_patterns[i - 1].pop(key2, None)
    # import ipdb;ipdb.set_trace()

    all_keys = []
    all_frequent_patterns = {}
    final_keys = open(string + "patterns" + shot_number + ".txt", "w")
    for i in range(1, columns + 1):
        for key in frequent_patterns[i].keys():
            all_keys.insert(0, key.lstrip().rstrip())
            all_frequent_patterns[key.lstrip().rstrip()] = 0
            # print key+"---"+str(frequent_patterns[i][key])
            final_keys.write(key.lstrip().rstrip() + "-" + str(frequent_patterns[i][key]) + "\n")

            # By this time all frequent sequences are generated
            # *********************************************
            # Find the modified frequency of each sequence


def mining_shots_parallel(string):
    proc = []
    for i in range(0, n1):
        filename = string + "Cluster" + str(i) + ".txt"
        # print("hi")

        p = Process(target=miner, args=(string, filename, str(i),))
        p.start()
        proc.append(p)
    for p in proc:
        p.join()


def mining_all_components_parallel():
    proc = []
    for string in ["blue", "green", "red"]:
        p = Process(target=mining_shots_parallel, args=(string,))
        p.start()
        proc.append(p)
    for p in proc:
        p.join()


def huffman_codes(s):
    table = []
    with open(s, "r") as s:
        for line in s:
            x = line.replace("\n", '').rstrip().split('-')
            # print(x)
            # print(x[1])
            table.append(Node([[x[0], '']], int(x[1])))
    heapq.heapify(table)
    while len(table) > 1:
        first_node = heapq.heappop(table)
        second_node = heapq.heappop(table)
        new_node = first_node.merge(second_node)
        heapq.heappush(table, new_node)
    return dict(table[0].pairs)


def huff_encode(string, filename, shotno):
    s = open(string + shotno + "Codetable.txt", "w")
    x = huffman_codes(filename)
    for i in x.keys():
        s.write(i + "-" + x[i] + "\n")


def huff_encode_shots_parallel(string):
    proc = []
    for i in range(0, n1):
        filename = string + "patterns" + str(i) + ".txt"

        p = Process(target=huff_encode, args=(string, filename, str(i),))
        p.start()
        proc.append(p)
    for p in proc:
        p.join()


def huff_encode_all_components_parallel():
    proc = []
    for string in ["blue", "green", "red"]:
        p = Process(target=huff_encode_shots_parallel, args=(string,))
        p.start()
        proc.append(p)
    for p in proc:
        p.join()


def huffman_encoder(string, filename, shotno):
    codetable = {}

    # string = "red"
    with open(filename, "r") as ct:
        for line in ct:
            (pattern, code) = line.replace("\n", "").split("-")
            codetable[pattern] = code
            # print(pattern)
    code_table_items = codetable.items()
    sorted_code_table = sorted(code_table_items, key=lambda s: len(s[0]))
    reversed_sorted_code_table = list(reversed(sorted_code_table))
    # print reversedSortedCodeTable[0][0]
    codes = []
    for item in reversed_sorted_code_table:
        codes.append(item[0])

    # print codetable[0]

    red_encoding = open(string + shotno + "Compressed.txt", "w")
    with open(string + "Cluster" + shotno + ".txt", "r") as rc:
        for line in rc:
            line = " " + line + " "
            current_line = " " + line
            for key in codes:

                if len(key.split(" ")) == 1:
                    # print "yes"
                    k1 = " " + key + ""
                    k2 = " " + key + ""
                    if k1 in current_line:
                        while k1 in current_line:
                            current_line = current_line.replace(k1, " -" + codetable[key] + "- ")
                            # redEncoding.write("no-"+k1+"$$"+currentLine+"\n")
                    elif k2 in current_line:
                        while k2 in current_line:
                            current_line = current_line.replace(k2, " -" + codetable[key] + "- ")
                            # redEncoding.write("no-"+k2+"$$"+currentLine+"\n")
                else:

                    if " " + key + " " in current_line:

                        while " " + key + " " in current_line:
                            # print "yes"
                            current_line = current_line.replace(" " + key + " ", " -" + codetable[key] + "- ")
            # import ipdb;ipdb.set_trace()
            current_line = current_line.replace(" ", "").replace("-", "")
            red_encoding.write(current_line + "\n")


def compressor_shots_parallel(string):
    proc = []
    for i in range(0, n1):
        filename = string + str(i) + "Codetable.txt"

        p = Process(target=huffman_encoder, args=(string, filename, str(i),))
        p.start()
        proc.append(p)
    for p in proc:
        p.join()


def compressor():
    proc = []
    for string in ["blue", "green", "red"]:
        p = Process(target=compressor_shots_parallel, args=(string,))
        p.start()
        proc.append(p)

    for p in proc:
        p.join()


if __name__ == "__main__":
    start_time = time.time()
    h, w, fc = get_all_video_related_dimensions_components()
    number_of_blocks = 1
    block_size = fc / number_of_blocks

    # run_clustering_all_components_parallel()
    # mining_all_components_parallel()
    # print("done mining")
    # huff_encode_all_components_parallel()
    # print("Done huffmann coding")
    # compressor()
    # print("Done compressor")

    print("calculating Statistics")
    compressedImageSize = os.stat("all_files/red0Compressed.txt").st_size + os.stat("all_files/green0Compressed.txt").st_size + os.stat(
        "all_files/blue0Compressed.txt").st_size
    compressedImageSize /= 80000.0
    codetablesize = os.stat("all_files/blue0Codetable.txt").st_size + os.stat("all_files/red0Codetable.txt").st_size + os.stat(
        "all_files/green0Codetable.txt").st_size
    codetablesize /= 1000.0
    # pca_digits_size = os.stat("blueblock0pca_digits.txt").st_size + os.stat(
    #     "redblock0pca_digits.txt").st_size + os.stat("greenblock0pca_digits.txt").st_size
    # pca_digits_size = pca_digits_size / (1000.0)
    clustertable_size = os.stat("all_files/redClusterTable0.txt").st_size + os.stat("all_files/blueClusterTable0.txt").st_size + os.stat(
        "all_files/greenClusterTable0.txt").st_size
    clustertable_size /= 1000.0
    overallcompression_size = compressedImageSize + codetablesize + clustertable_size
    original_size = os.stat("SampleVideo_320x240_1mb.mp4").st_size / (1000.0)
    compression_ratio = original_size / overallcompression_size
    print("overall compression size", round(overallcompression_size, 2))
    print("compression ratio", round(compression_ratio, 2))
    print("original size", round(original_size, 2))
