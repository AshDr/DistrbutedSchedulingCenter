cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(DistrbutedSchedulingCenter)

set(SRC_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

# include_directories(${SRC_INCLUDE_DIR})

file(GLOB SERVER_SRC_FILES "${CMAKE_SOURCE_DIR}/src/plugin.cc" 
                           "${CMAKE_SOURCE_DIR}/src/tcpserver.cc" 
                           "${CMAKE_SOURCE_DIR}/src/task.cc"
                           "${CMAKE_SOURCE_DIR}/src/schedulingcenter.cc"
                           "${CMAKE_SOURCE_DIR}/src/pluginmanager.cc")

file(GLOB CLIENT_SRC_FILES "${CMAKE_SOURCE_DIR}/src/plugin.cc" 
                           "${CMAKE_SOURCE_DIR}/src/tcpclient.cc" 
                           "${CMAKE_SOURCE_DIR}/src/task.cc"
                           "${CMAKE_SOURCE_DIR}/src/branchoffice.cc"
                           "${CMAKE_SOURCE_DIR}/src/vehicle.cc"
                           "${CMAKE_SOURCE_DIR}/src/pluginmanager.cc")

add_library(server_all
            OBJECT 
            ${SERVER_SRC_FILES}
            )
target_include_directories(server_all PUBLIC ${SRC_INCLUDE_DIR})

add_library(client_all
            OBJECT 
            ${CLIENT_SRC_FILES}
            )
target_include_directories(client_all PUBLIC ${SRC_INCLUDE_DIR})

add_executable(server
               ${CMAKE_SOURCE_DIR}/src/server_main.cc
               $<TARGET_OBJECTS:server_all>
               )
target_link_libraries(server PRIVATE pthread dl)
add_executable(client
               ${CMAKE_SOURCE_DIR}/src/client_main.cc
               $<TARGET_OBJECTS:client_all>
               )
target_link_libraries(client PRIVATE pthread dl)

target_include_directories(server PUBLIC ${SRC_INCLUDE_DIR})
target_include_directories(client PUBLIC ${SRC_INCLUDE_DIR})

add_library(funcv1 SHARED "src/funcv1.cc")
add_library(funcv2 SHARED "src/funcv2.cc")
set_target_properties(funcv1 PROPERTIES OUTPUT_NAME "funcv1")
set_target_properties(funcv2 PROPERTIES OUTPUT_NAME "funcv2")

set_target_properties(funcv1 PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins)
set_target_properties(funcv2 PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins)

target_compile_options(funcv1 PRIVATE -fPIC)
target_compile_options(funcv2 PRIVATE -fPIC)
