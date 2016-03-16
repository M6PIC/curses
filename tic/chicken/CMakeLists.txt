cmake_minimum_required(VERSION 3.0)

add_executable(tic
	../tic.c
	)
target_link_libraries(tic terminfo)
