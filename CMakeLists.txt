cmake_minimum_required(VERSION 3.1)

set(CLI_NAME client)
set(SRV_NAME server)

project ($CLI_NAME)

set(CODEROOT ${CMAKE_SOURCE_DIR}/sources)

set(CMAKE_C_FLAGS_DEBUG "-g -Wall")
set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FALGS} -std=c++11")
if(CLANG)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

set(PLATFORM_SPECIFIC_SRC
  ./tcpclient.cpp
  ./tcpserv.cpp
)

set(MODEL_SRC
  sources/error.cpp
  sources/readline.cpp
  sources/readn.cpp
  sources/wraplinux.cpp
  sources/wrapsock.cpp
  sources/writen.cpp
)

set(MODEL_SRV_SRC
  ${MODEL_SRC}
  ./tcpserv.cpp
)

set(MODEL_CLI_SRC
  ${MODEL_SRC}
  ./tcpclient.cpp
)

set(MODEL_HEADERS
  sources/munp.h
)

# add the executable
add_executable(${CLI_NAME} ${MODEL_SRV_SRC} ${MODEL_HEADERS})
add_executable(${SRV_NAME} ${MODEL_CLI_SRC} ${MODEL_HEADERS})

