cmake_minimum_required(VERSION 3.0)

add_executable(tic_egg
	../tic.c
	)

target_link_libraries(tic_egg terminfo_egg)
