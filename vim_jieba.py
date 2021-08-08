#!/bin/python3
import jieba
import sys
import urllib

# 期待三个参数，一个参数是字符串，一个参数是当前位置，一个参数为方向
# 返回值为推荐位置

if sys.argv.__len__() < 4:
    exit(0)

sys.argv[1]=urllib.parse.unquote(sys.argv[1])
seg_list = jieba.lcut(sys.argv[1], cut_all=False)
cur_pos = 0
if sys.argv[3] == "right":
    cur_pos = 0
    for item in seg_list:
        cur_pos += item.encode("utf-8").__len__()
        if cur_pos > int(sys.argv[2]):
            break;
else:
    cur_pos = sys.argv[1].__len__()
    size = seg_list.__len__() - 1
    while cur_pos >= int(sys.argv[2]) and cur_pos >= 0:
        cur_pos -= seg_list[size].__len__()
        size -= 1
print(cur_pos)
