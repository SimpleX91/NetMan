cmake_minimum_required(VERSION 2.8)

project(netmanager)

set(CMAKE_BUILD_TYPE Debug)

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
include_directories(${CMAKE_SOURCE_DIR}/Include)
file(GLOB_RECURSE HEADERS_LIST "${CMAKE_SOURCE_DIR}/Include/*.h")

aux_source_directory(${CMAKE_SOURCE_DIR}/Base SRC_LIST)

if(OS_WINDOWS)
  set(REACTOR_SRC_LIST Reactor/SelectReactor.cpp)
elseif(OS_LINUX)
  set(REACTOR_SRC_LIST Reactor/EpollReactor.cpp)
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
