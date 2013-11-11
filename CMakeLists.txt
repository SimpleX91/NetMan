cmake_minimum_required(VERSION 2.8)

project(netmanager)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
  set(OS_WINDOWS TRUE)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
  set(OS_LINUX TRUE)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  set(OS_DARWIN TRUE)
endif (CMAKE_SYSTEM_NAME STREQUAL "Windows")

if(OS_LINUX)
  message(STATUS "Building for OS Linux")
elseif(OS_WINDOWS)
  message(STATUS "Building for OS Windows")
  add_definitions(-DWIN32 -D_WIN32_WINNT=0x0600)
endif(OS_LINUX)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")

#Подключаем директорию с заголовочниками:
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/netman)
file(GLOB_RECURSE HEADERS_LIST "${CMAKE_SOURCE_DIR}/include/*.h")

aux_source_directory(${CMAKE_SOURCE_DIR}/base SRC_LIST)

if(OS_WINDOWS)
  set(REACTOR_SRC_LIST reactor/SelectReactor.cpp)
elseif(OS_LINUX)
  set(REACTOR_SRC_LIST reactor/EpollReactor.cpp)
endif()

#Сборка билиотеки
add_library (${PROJECT_NAME} STATIC
  ${REACTOR_SRC_LIST}
  ${SRC_LIST}
  ${HEADERS_LIST}
)

if(OS_WINDOWS)
  target_link_libraries(${PROJECT_NAME} ws2_32)
endif()

install(
  TARGETS netmanager
  ARCHIVE DESTINATION lib
)

install(
  FILES ${HEADERS_LIST}
  DESTINATION include/netmanager
)
