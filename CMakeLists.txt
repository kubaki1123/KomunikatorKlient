cmake_minimum_required(VERSION 3.13)

project(komunikator_server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

#setup asio

add_library(asio INTERFACE)

target_include_directories(asio INTERFACE asio/include)

#setup komunikator

add_executable(komunikator_klient 
	Komunikator_klient/Client.cpp
	Komunikator_klient/Main.cpp
)

target_include_directories(komunikator_klient PRIVATE Komunikator_klient/)

target_link_libraries(komunikator_klient PRIVATE asio)