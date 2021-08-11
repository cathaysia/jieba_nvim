cmake_minimum_required(VERSION 3.20)

project(jiebaMove LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_library(jiebamove SHARED main.cpp)

include(FetchContent)
fetchcontent_declare(
  cppjieba
  GIT_REPOSITORY https://github.com/yanyiwu/cppjieba
  GIT_TAG v5.0.3
)
fetchcontent_makeavailable(cppjieba)

set(
  data_files
  ${cppjieba_SOURCE_DIR}/dict/jieba.dict.utf8
  ${cppjieba_SOURCE_DIR}/dict/hmm_model.utf8
  ${cppjieba_SOURCE_DIR}/dict/user.dict.utf8
  ${cppjieba_SOURCE_DIR}/dict/idf.utf8
  ${cppjieba_SOURCE_DIR}/dict/stop_words.utf8
)

add_custom_command(
  TARGET jiebamove
  PRE_BUILD
  COMMAND cmake -E make_directory ${CMAKE_BINARY_DIR}/data
  COMMAND cmake -E copy ${data_files} ${CMAKE_BINARY_DIR}/data
)

find_package(Lua REQUIRED)
target_include_directories(jiebamove PRIVATE ${LUA_INCLUDE_DIR} ${cppjieba_SOURCE_DIR}/deps ${cppjieba_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include)
target_link_libraries(jiebamove PRIVATE ${LUA_LIBRARIES})
