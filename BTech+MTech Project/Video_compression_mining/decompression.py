import numpy as np
from multiprocessing import Process
from multiprocessing import Manager

m = Manager()
digits_dict = m.dict()

n1 = 1


def pca_each_block(string, blockno, blockfilename, componentsfilename, meanfilename, pca_digits_name):

    dt = np.dtype(np.int32)
    data_reduced = np.loadtxt(blockfilename, dtype=dt)
    np.savetxt(string + "final" + blockno + ".txt", data_reduced, fmt='%i')


def pca_all_blocks_parallel(string):
    proc = []
    for i in range(0, n1):
        block_file_name = string + str(i) + "Decompressed" + ".txt"
        components_filename = string + "block" + str(i) + "pca_components1" + ".txt"
        mean_filename = string + "block" + str(i) + "mean" + ".txt"
        pca_digits_name = string + "block" + str(i) + "pca_digits" + ".txt"

        p = Process(target=pca_each_block,
                    args=(
                        string, str(i), block_file_name, components_filename, mean_filename, pca_digits_name,))
        p.start()
        proc.append(p)
    for p in proc:
        p.join()


def run_pca_decoding_parallel():
    proc = []
    for string in ["blue", "green", "red"]:
        p = Process(target=pca_all_blocks_parallel, args=(string,))
        p.start()
        proc.append(p)

    for p in proc:
        p.join()


def huffman_decoder(string, blockno, filename):
    code_table = {}
    with open(filename, "r") as ct:
        for line in ct:
            (pattern, code) = line.replace("\n", "").replace("  ", " ").split("-")
            code_table[code] = pattern

    red_decomp = open(string + blockno + "HuffmanDecoded.txt", "w")
    with open(string + blockno + "Compressed.txt", "r") as rc:
        for line in rc:
            # print line
            current_line = ""
            c_string = ""
            count = 0
            for i in line:
                # print i
                c_string += i
                count = 0

                for key in code_table.keys():
                    if c_string == key:
                        count = 1
                        break
                if count == 1:
                    # redDecomp.write(cString+"---"+codeTable[cString]+"\n")
                    # currentLine = currentLine.replace(cString,"-"+codeTable[cString]+"-")
                    current_line = current_line + " " + code_table[c_string]
                    # redDecomp.write(currentLine+"\n")
                    c_string = ""
            red_decomp.write(current_line.rstrip().lstrip() + '\n')


def huffman_all_blocks_parallel(string):
    proc = []
    for i in range(0, n1):
        filename = string + str(i) + "Codetable.txt"

        p = Process(target=huffman_decoder, args=(string, str(i), filename))
        p.start()
        proc.append(p)
    for p in proc:
        p.join()


def decoder():
    proc = []
    for string in ["blue", "green", "red"]:
        p = Process(target=huffman_all_blocks_parallel, args=(string,))
        p.start()
        proc.append(p)

    for p in proc:
        p.join()


def block_shaped(arr, n_rows, n_cols):
    h, w = arr.shape
    return arr.reshape(h // n_rows, n_rows, -1, n_cols).swapaxes(1, 2).reshape(-1, n_rows, n_cols)


def un_block_shaped(arr, h, w):
    n, n_rows, n_cols = arr.shape
    return (arr.reshape(h // n_rows, -1, n_rows, n_cols)
            .swapaxes(1, 2)
            .reshape(h, w))


def cluster_decoder(string, block_no, filename):
    final = []

    dt = np.dtype(np.int32)
    final = np.loadtxt(filename, dtype=dt)
    # print(final)

    y = block_shaped(final, final.shape[0], final.shape[1])
    val = {}
    for i in range(0, 1):
        val[i] = {}
        with open(string + "ClusterTable" + block_no + ".txt", "r") as s:
            for line in s:
                mat = line.strip().split(" ")
                # val[i][int(mat[0])] = int(mat[1])
                val[i][float(mat[0])] = float(mat[1])
    new_final = []
    for i in range(0, 1):
        new_y = y[i].reshape(1, final.shape[0] * final.shape[1])
        y_new = []
        for element in new_y[0]:
            y_new.append(val[i][int(element)])
        y_new = np.asarray(y_new)
        y_new = y_new.reshape(final.shape[0], final.shape[1])
        new_final.append(y_new)
    new_final = np.asarray(new_final)
    shaped_new_final = un_block_shaped(new_final, final.shape[0], final.shape[1])
    with open(string + block_no + "Decompressed.txt", "w") as w:
        for i in range(0, final.shape[0]):
            for m in shaped_new_final[i]:
                w.write(str(m) + " ")
            w.write("\n")


def cluster_decoding_blocks_parallel(string):
    proc = []
    for i in range(0, n1):
        filename = string + str(i) + "HuffmanDecoded.txt"

        p = Process(target=cluster_decoder, args=(string, str(i), filename))
        p.start()
        proc.append(p)
    for p in proc:
        p.join()


def run_cluster_decoding_parallel():
    proc = []
    for string in ["red", "green", "blue"]:
        p = Process(target=cluster_decoding_blocks_parallel, args=(string,))
        p.start()
        proc.append(p)
    for p in proc:
        p.join()


def eachcolorconstruct(string):
    print("entered color construct")
    zb = []
    filename = string + "final" + str(0) + ".txt"
    dt = np.dtype(np.int32)
    reshape_frame = np.loadtxt(filename, dtype=dt)
    zb = reshape_frame.reshape(reshape_frame.shape[0], reshape_frame.shape[1])

    for i in range(1, n1):
        filename = string + "final" + str(i) + ".txt"
        dt = np.dtype(np.int32)
        reshape_frame = np.loadtxt(filename, dtype=dt)
        each_block = reshape_frame.reshape(reshape_frame.shape[0], reshape_frame.shape[1])
        zb = np.c_[zb, each_block]

    np.savetxt("final" + string + "file" + ".txt", zb, fmt='%i')


def reconstructshape():
    proc = []
    for string in ["blue", "green", "red"]:
        p = Process(target=eachcolorconstruct, args=(string,))
        p.start()
        proc.append(p)
    for p in proc:
        p.join()


if __name__ == "__main__":
    numberOfBlocks = 1
    w = 360
    h = 240
    fc = 123
    block_size = fc / numberOfBlocks
    print("decoder")
    decoder()
    print("end decoder")
    run_cluster_decoding_parallel()
    print("end clustering")
    run_pca_decoding_parallel()
    reconstructshape()
    print("reconstructed")
# reconstructvideofromblocks()
