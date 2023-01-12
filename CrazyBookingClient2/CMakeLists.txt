cmake_minimum_required(VERSION 3.5)

project(CrazyBookingClient2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(CrazyBookingClient2 main.cpp network.cpp network.h)
TARGET_LINK_LIBRARIES( CrazyBookingClient2 pthread)
