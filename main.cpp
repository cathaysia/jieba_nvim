#include <algorithm>
#include <cppjieba/Jieba.hpp>
#include <cstddef>
// WARNING: 这个头文件会导致库没法使用
// #include <lauxlib.h>
#include <atomic>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include "lua.hpp"

extern "C" {
#include <dlfcn.h>
}

constexpr char const *const DICT_PATH      = "data/jieba.dict.utf8";
constexpr char const *const HMM_PATH       = "data/hmm_model.utf8";
constexpr char const *const USER_DICT_PATH = "data/user.dict.utf8";
constexpr char const *const IDF_PATH       = "data/idf.utf8";
constexpr char const *const STOP_WORD_PATH = "data/stop_words.utf8";

int getPos(const std::string &, size_t, bool);

cppjieba::Jieba &getJieba() {
    // INFO: 这里会有内存泄漏吗？不会，Lua 的模块是没法 unload 的
    static cppjieba::Jieba *     pjieba = nullptr;
    static std::mutex            mu;
    std::scoped_lock<std::mutex> guard(mu);
    if(!pjieba) {
        Dl_info dl_info;
        dladdr((void *)getJieba, &dl_info);
        std::string path(dl_info.dli_fname);
        path   = path.substr(0, path.find_last_of('/') + 1);
        pjieba = new cppjieba::Jieba(path + DICT_PATH, path + HMM_PATH, path + USER_DICT_PATH, path + IDF_PATH,
                                     path + STOP_WORD_PATH);
    }
    return *pjieba;
}
int getPos(const std::string &line, size_t pos, bool isRight) {
    std::vector<std::string> segList;
    auto &                   jieba = getJieba();
    jieba.Cut(line, segList, true);
    if(isRight) {
        size_t curPos = 0;
        for(auto const &item: segList) {
            curPos += item.length();
            if(curPos > pos) {
                // 跳过空格
                while(line[curPos] == ' ' && curPos < line.length()) ++curPos;
                return curPos;
            };
        }
    }
    size_t curPos = line.length();
    for(auto beg = segList.rbegin(); curPos >= pos && curPos; ++beg) { curPos -= beg->length(); }
    // 跳过空格
    while(line[curPos] == ' ' && curPos > 0) --curPos;
    return curPos;
}

extern "C" int l_pos(lua_State *L) {
    auto        line_data = luaL_checkstring(L, 1);
    std::string line(line_data);
    auto        pos     = luaL_checkinteger(L, 2);
    auto        isRight = luaL_checkinteger(L, 3) != 0;
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
