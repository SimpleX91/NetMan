project(NetworkManager)

cmake_minimum_required(VERSION 2.8)

aux_source_directory(. SRC_LIST)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#Что бы в дереве проекта отображались заголовочники
set(HEAD_LIST ${CMAKE_SOURCE_DIR}/Include/NetworkManager/HostAddress.h
              ${CMAKE_SOURCE_DIR}/Include/NonCopyable.h
              ${CMAKE_SOURCE_DIR}/Include/NetworkManager/BSDWinDiff.h
              ${CMAKE_SOURCE_DIR}/Include/Misc/UThread.h
              ${CMAKE_SOURCE_DIR}/Include/NetworkManager/ByteArray.h
              ${CMAKE_SOURCE_DIR}/Include/NetworkManager/Connection.h
              ${CMAKE_SOURCE_DIR}/Include/NetworkManager/NetworkManager.h
              ${CMAKE_SOURCE_DIR}/Include/NetworkManager/Server.h
              ${CMAKE_SOURCE_DIR}/Include/NetworkManager/ExternalAbstract.h
              ${CMAKE_SOURCE_DIR}/NetworkManager/Base/NetworkManagerInt.h
              ${CMAKE_SOURCE_DIR}/NetworkManager/Base/InternalAbstract.h
    )

#Сборка билиотеки
add_library (${PROJECT_NAME} STATIC ${SRC_LIST} ${HEAD_LIST})
target_link_libraries(${PROJECT_NAME} UThread Reactor)

if(OS_WINDOWS)
  target_link_libraries(${PROJECT_NAME} ws2_32)
endif()
