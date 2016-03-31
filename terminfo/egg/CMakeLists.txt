cmake_minimum_required(VERSION 3.0)

include_directories("../include")
include_directories(${PROJECT_BINARY_DIR}/egg)
include_directories(${PROJECT_SOURCE_DIR}/egg)

add_custom_command(
	OUTPUT termcap_hash.c
	COMMAND TOOL_NBPERF=${CMAKE_BINARY_DIR}/nbperf/nbperf ${PROJECT_SOURCE_DIR}/genthash ${PROJECT_SOURCE_DIR}/src/termcap_map.c > termcap_hash.c
	)

add_custom_command(
	OUTPUT ${PROJECT_BINARY_DIR}/egg/hash.c
	COMMAND TOOL_NBPERF=${CMAKE_BINARY_DIR}/nbperf/nbperf ${PROJECT_SOURCE_DIR}/genhash ${CMAKE_SOURCE_DIR}/include/term.h > hash.c
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
	../src/ti.c
	../src/tparm.c
	../src/tputs.c
	../src/setupterm.c
	${PROJECT_BINARY_DIR}/egg/hash.c
	../../general/cdbr.c
	../../general/mi_vector_hash.c
	)
