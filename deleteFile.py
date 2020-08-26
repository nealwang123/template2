import os
import sys
import os.path
import shutil


# 获取当前路径
def fileDir():
    path = sys.path[0]
    print(path)
    # 判断为脚本文件还是编译后文件，如果是脚本文件则返回脚本目录，否则返回编译后的文件路径
    if os.path.isdir(path):
        return path
    elif os.path.isfile(path):
        return os.path.dirname(path)


# 获取文件后缀名
def suffix(file, *suffixName):
    array = map(file.endswith, suffixName)
    if True in array:
        return True
    else:
        return False


# 递归删除目录下扩展名为'.o', '.obj', '.Release', '.release', '.Debug', '.debug', '.bak' 的文件
def deleteFile(filepath):
    for file in os.listdir(filepath):
        targetFile = os.path.join(filepath, file)
        if os.path.isfile(targetFile):  # 如果是文件
            if suffix(file, '.o', '.obj', '.Release', '.release', '.Debug', '.debug', '.bak'):
                print("删除：" + targetFile)
                os.remove(targetFile)
        else:
            deleteFile(targetFile)


if __name__ == '__main__':
    filepath = fileDir()
    deleteFile(filepath)
    print("delete done")
