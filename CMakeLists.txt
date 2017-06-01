cmake_minimum_required(VERSION 2.8)

set(CLI_NAME client)
set(SRV_NAME server)
set(REC_NAME cycle)
# project ($CLI_NAME)

set(CODEROOT ${CMAKE_SOURCE_DIR}/sources)
set(INPATH ${CMAKE_SOURCE_DIR}/include)

# build flag
set(CMAKE_C_FLAGS_DEBUG "-g -Wall")
set(CMAKE_CXX_FLAGS_RELEASE ${CMAKE_C_FLAGS_DEBUG})
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FALGS} -std=c++11")
if(CLANG)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

set(PLATFORM_SPECIFIC_SRC
  ./tcpclient.cc
  ./tcpserv.cc
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
  ./tcpserv.cc
)

set(MODEL_CLI_SRC
  ./tcpclient.cc
)

set(MODEL_CLI_REC
  ./cycle.cc
)

set(MODEL_HEADERS
  sources/munp.h
)

# add include path
include_directories(${CMAKE_SOURCE_DIR}/include)

# add the executable
add_executable(${SRV_NAME} ${MODEL_SRV_SRC} ${MODEL_SRC} ${MODEL_HEADERS})
add_executable(${CLI_NAME} ${MODEL_CLI_SRC} ${MODEL_SRC} ${MODEL_HEADERS})
add_executable(${REC_NAME} ${MODEL_CLI_REC} ${MODEL_SRC} ${MODEL_HEADERS})
add_executable(tt test.cc ${INPATH}/bcm/bcm2835.h ${INPATH}/bcm/bcm2835.c)
