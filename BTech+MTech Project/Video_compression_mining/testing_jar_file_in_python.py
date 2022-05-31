# from subprocess import *

# def jarWrapper(*args):
#     process = Popen(['java', '-jar']+list(args), stdout=PIPE, stderr=PIPE)
#     ret = []
#     while process.poll() is None:
#         line = process.stdout.readline()
#         if line != '' and line.endswith('\n'):
#             ret.append(line[:-1])
#     stdout, stderr = process.communicate()
#     ret += stdout.split('\n')
#     if stderr != '':
#         ret += stderr.split('\n')
#     ret.remove('')
#     return ret


# result = jarWrapper(*args)
# print(result)


# import subprocess
# subprocess.call(['java', '-jar', 'spmf.jar','PrefixSpan','contextPrefixSpan.txt','output.txt','60%'])

import os

if __name__ == "__main__":
    starting_dir = os.getcwd()
    test_dir = "/home/krishna/final_project/"
    os.chdir(test_dir)
    os.system(
        "java -jar spmf.jar run HirateYamana contextSequencesTimeExtended.txt output.txt 85% 0 2 0 2"
    )
    os.chdir(starting_dir)
