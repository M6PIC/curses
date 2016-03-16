cmake_minimum_required(VERSION 3.0)
project(nbperf)

include_directories(after "include")

add_executable(nbperf
	src/nbperf.c
	src/nbperf-bdz.c
	src/nbperf-chm.c
	src/nbperf-chm3.c
	src/graph3.c
	src/graph2.c
	)
