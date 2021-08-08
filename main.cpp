#include <algorithm>
#include <cppjieba/Jieba.hpp>
#include <cstddef>
#include <data_config.h>
// 这个头文件会导致库没法使用
// #include <lauxlib.h>
#include "lua.hpp"
#include <string>
#include <vector>

extern "C"{
#include <dlfcn.h>
}

static int getPos(const std::string &line, size_t pos, bool isRight) {
  std::vector<std::string> segList;
  Dl_info dl_info;
  dladdr((void*)getPos, &dl_info);
  std::string path(dl_info.dli_fname);
  path = path.substr(0, path.find_last_of('/')+1);
  cppjieba::Jieba jieba(
        path+ DICT_PATH,
        path + HMM_PATH,
        path + USER_DICT_PATH,
        path + IDF_PATH,
        path + STOP_WORD_PATH);
  jieba.Cut(line, segList, true);
  if (isRight) {
    size_t curPos = 0;
    for (auto const &item : segList) {
      curPos += item.length();
      if (curPos > pos)
        return curPos;
    }
  }
  size_t curPos = line.length();
  for (auto beg = segList.rbegin(); curPos >= pos && curPos; ++beg) {
    curPos -= beg->length();
  }

  return curPos;
}

extern "C" int l_pos(lua_State *L) {
  auto line_data = luaL_checkstring(L, 1);
  std::string line(line_data);
  auto pos = luaL_checknumber(L, 2);
  auto isRight = luaL_checknumber(L, 3) != 0;
  auto result = getPos(line, pos, isRight);

  lua_pushinteger(L, result);
  return 1;
}

static luaL_Reg jieba_move[] = {{"getPos", l_pos}, {NULL, NULL}};

extern "C" int luaopen_libjiebamove(lua_State *L) {
  luaL_newlib(L, jieba_move);
  return 1;
}
