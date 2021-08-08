cmake_minimum_required(VERSION 3.20)
project(jiebaMove)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE True)

add_library(jiebamove SHARED main.cpp)

include(FetchContent)
FetchContent_Declare(
    cppjieba
    GIT_REPOSITORY https://github.com/yanyiwu/cppjieba
    GIT_TAG v5.0.3
    )
FetchContent_MakeAvailable(cppjieba)

set(data_files ${cppjieba_SOURCE_DIR}/dict/jieba.dict.utf8
    ${cppjieba_SOURCE_DIR}/dict/hmm_model.utf8 ${cppjieba_SOURCE_DIR}/dict/user.dict.utf8
    ${cppjieba_SOURCE_DIR}/dict/idf.utf8 ${cppjieba_SOURCE_DIR}/dict/stop_words.utf8
)

add_custom_command(
    TARGET jiebamove
    PRE_BUILD
    COMMAND cmake -E make_directory ${CMAKE_BINARY_DIR}/data
    )
add_custom_command(
    TARGET jiebamove
    PRE_BUILD
    COMMAND cmake -E copy ${data_files} ${CMAKE_BINARY_DIR}/data
    )
set(DICT_PATH data/jieba.dict.utf8)
set(HMM_PATH data/hmm_model.utf8)
set(USER_DICT_PATH data/user.dict.utf8)
set(IDF_PATH data/idf.utf8)
set(STOP_WORD_PATH data/stop_words.utf8)

# set(DICT_PATH ${cppjieba_SOURCE_DIR}/dict/jieba.dict.utf8)
# set(HMM_PATH ${cppjieba_SOURCE_DIR}/dict/hmm_model.utf8)
# set(USER_DICT_PATH ${cppjieba_SOURCE_DIR}/dict/user.dict.utf8)
# set(IDF_PATH ${cppjieba_SOURCE_DIR}/dict/idf.utf8)
# set(STOP_WORD_PATH ${cppjieba_SOURCE_DIR}/dict/stop_words.utf8)
configure_file(data_config.h.in ${PROJECT_BINARY_DIR}/include/data_config.h)

find_package(Lua REQUIRED)
target_include_directories(jiebamove PRIVATE ${LUA_INCLUDE_DIR} ${cppjieba_SOURCE_DIR}/deps ${cppjieba_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include)
target_link_libraries(jiebamove PRIVATE ${LUA_LIBRARIES})

