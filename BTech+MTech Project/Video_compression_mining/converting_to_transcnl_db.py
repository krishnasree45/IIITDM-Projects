# import os
# import ipdb;ipdb.set_trace()
# current_dir = os.getcwd()
# next_dir = current_dir + '/sliced_frames'
# os.chdir(next_dir)
# all_dirs = next(os.walk('/home/krishna/final_project/codes/pca+c+mining/sliced_frames'))[1]
# frame = dict()
# file = open('transactional_data.txt', "w")
# for cluster_no in all_dirs:
#     # import ipdb;ipdb.set_trace()
#     frame[cluster_no] = list()
#     all_files = next(os.walk(
#         '/home/krishna/final_project/codes/pca+c+mining/sliced_frames/'+str(cluster_no)))[2]
#     if all_files:
#         for each_file in all_files:
#             frame[cluster_no].append(str(each_file))
#     file.write(cluster_no + str(frame[cluster_no]) + '\n')
# file.close()
# os.chdir(current_dir)

file = open('transactional_data.txt', 'r')

