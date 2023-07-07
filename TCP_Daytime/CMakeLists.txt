#--------------------------- MAIN CONFIGURATION ---------------------------#

cmake_minimum_required(VERSION 3.6.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcoroutines")

set(
    CLIENT_EXECUTABLE
    "${PROJECT_NAME}_client"
)

set(
    SERVER_EXECUTABLE
    "${PROJECT_NAME}_server"
)

set(
    SERVER_EXECUTABLE_ASYNC
    "${PROJECT_NAME}_server_async"
)

add_executable(
    ${CLIENT_EXECUTABLE}
    client.cpp
)

add_executable(
    ${SERVER_EXECUTABLE}
    Synchronous/server.cpp
)

add_executable(
    ${SERVER_EXECUTABLE_ASYNC}
    Asynchronous/server.cpp
)

# Adding Boost library;

find_package(Boost COMPONENTS system REQUIRED)

message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
message(STATUS "Boost_VERSION: ${Boost_VERSION}")
message(STATUS "BOOST_SYSTEM: ${BOOST_SYSTEM}")
message(STATUS "BOOST_FILE_SYSTEM: ${BOOST_FILE_SYSTEM}")

target_include_directories(${CLIENT_EXECUTABLE} PUBLIC ${Boost_INCLUDE_DIRS})
target_include_directories(${SERVER_EXECUTABLE} PUBLIC ${Boost_INCLUDE_DIRS})
target_include_directories(${SERVER_EXECUTABLE_ASYNC} PUBLIC ${Boost_INCLUDE_DIRS})

set(
    ALL_LINKED_LIBS
    ${ALL_LINKED_LIBS}
    ${Boost_LIBRARIES}
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcoroutines")
# if windows compiler;
if(MINGW)
    message(STATUS "MinGW compiler defined!")
    SET(
        WIN_LIBS
        ws2_32
        wsock32
    )
    set(
        ALL_LINKED_LIBS
        ${ALL_LINKED_LIBS}
        ${WIN_LIBS}
    )
endif()

target_link_libraries(${CLIENT_EXECUTABLE} LINK_PUBLIC ${ALL_LINKED_LIBS})
target_link_libraries(${SERVER_EXECUTABLE} LINK_PUBLIC ${ALL_LINKED_LIBS})
target_link_libraries(${SERVER_EXECUTABLE_ASYNC} LINK_PUBLIC ${ALL_LINKED_LIBS})