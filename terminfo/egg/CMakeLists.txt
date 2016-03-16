cmake_minimum_required(VERSION 3.0)

include_directories(after "../include")
include_directories(after ${PROJECT_BINARY_DIR}/egg)
include_directories(after ${PROJECT_SRC_DIR}/egg)

add_custom_command(
	OUTPUT termcap_hash.c
	COMMAND TOOL_NBPERF=${CMAKE_BINARY_DIR}/nbperf/nbperf ${PROJECT_SOURCE_DIR}/genthash ${PROJECT_SOURCE_DIR}/termcap_map.c > termcap_hash.c
	)

add_custom_command(
	OUTPUT ${PROJECT_BINARY_DIR}/egg/hash.c
	COMMAND TOOL_NBPERF=${CMAKE_BINARY_DIR}/nbperf/nbperf ${PROJECT_SOURCE_DIR}/genhash ${PROJECT_SOURCE_DIR}/include/term.h > hash.c
	)

add_custom_target(
	ti_generate_egg ALL 
	DEPENDS termcap_hash.c ${PROJECT_BINARY_DIR}/egg/hash.c
	)

add_library(terminfo_egg
	../src/compile.c
	../src/curterm.c
	../src/term.c
	../src/termcap.c
	../src/termcap_map.c
	../src/ti.c
	../src/tparm.c
	../src/tputs.c
	${PROJECT_BINARY_DIR}/egg/hash.c
	)
