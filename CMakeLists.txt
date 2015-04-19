project(IpPrefixBitmap)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(IpPrefixBitmap ${SOURCE_FILES} IpPrefixBitmap.cpp IpPrefixBitmap.h Node.h Ip.h estd.h)
