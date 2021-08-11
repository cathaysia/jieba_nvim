cmake_minimum_required(VERSION 3.20)

project(jiebamove LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

add_library(jiebamove SHARED main.cpp)

target_link_options(jiebamove PRIVATE -s)

find_package(Lua REQUIRED)
target_include_directories(jiebamove PRIVATE ${LUA_INCLUDE_DIR} extern/cppjieba/deps extern/cppjieba/include)
target_link_libraries(jiebamove PRIVATE ${LUA_LIBRARIES})

install(
  TARGETS jiebamove
  LIBRARY DESTINATION .config/nvim/lua
)

install(
  FILES
  extern/cppjieba/dict/jieba.dict.utf8
  extern/cppjieba/dict/hmm_model.utf8
  extern/cppjieba/dict/user.dict.utf8
  extern/cppjieba/dict/idf.utf8
  extern/cppjieba/dict/stop_words.utf8
  DESTINATION .config/nvim/lua/data
)
