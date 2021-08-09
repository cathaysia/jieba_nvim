#include <data_config.h>
#include <algorithm>
#include <cppjieba/Jieba.hpp>
#include <cstddef>
// 这个头文件会导致库没法使用
// #include <lauxlib.h>
#include <atomic>
#include <mutex>
#include <string>
#include <vector>
#include "lua.hpp"
#include <thread>

extern "C" {
#include <dlfcn.h>
}

int getPos(const std::string &, size_t, bool);

// 这里绝对不能返回一个左值引用
cppjieba::Jieba& getJieba() {
    // @TODO 这里会有内存泄漏吗？
    static cppjieba::Jieba *     pjieba = nullptr;
    static std::mutex            mu;
    std::scoped_lock<std::mutex> guard(mu);
    if(!pjieba) {
        Dl_info dl_info;
        dladdr((void *)getPos, &dl_info);
        std::string path(dl_info.dli_fname);
        path   = path.substr(0, path.find_last_of('/') + 1);
        pjieba = new cppjieba::Jieba(path + DICT_PATH, path + HMM_PATH, path + USER_DICT_PATH, path + IDF_PATH,
                                     path + STOP_WORD_PATH);
    }
    return *pjieba;
}
int getPos(const std::string &line, size_t pos, bool isRight) {
    std::vector<std::string> segList;
    auto&                     jieba = getJieba();
    jieba.Cut(line, segList, true);
    if(isRight) {
        size_t curPos = 0;
        for(auto const &item: segList) {
            curPos += item.length();
            if(curPos > pos) return curPos;
        }
    }
    size_t curPos = line.length();
    for(auto beg = segList.rbegin(); curPos >= pos && curPos; ++beg) { curPos -= beg->length(); }

    return curPos;
}

extern "C" int l_pos(lua_State *L) {
    auto        line_data = luaL_checkstring(L, 1);
    std::string line(line_data);
    auto        pos     = luaL_checknumber(L, 2);
    auto        isRight = luaL_checknumber(L, 3) != 0;
    auto        result  = getPos(line, pos, isRight);

    lua_pushinteger(L, result);
    return 1;
}

static luaL_Reg jieba_move[] = { { "getPos", l_pos }, { NULL, NULL } };

extern "C" int luaopen_libjiebamove(lua_State *L) {
    std::thread t([]() {
        getJieba();
    });
    t.detach();
    luaL_newlib(L, jieba_move);
    return 1;
}
