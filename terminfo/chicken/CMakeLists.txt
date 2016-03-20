cmake_minimum_required(VERSION 3.0)

include_directories(after "../include")
include_directories(after ${PROJECT_BINARY_DIR}/chicken)

add_custom_command(
	OUTPUT termcap_hash.c
	COMMAND TOOL_NBPERF=${CMAKE_BINARY_DIR}/nbperf/nbperf ${PROJECT_SOURCE_DIR}/genthash ${PROJECT_SOURCE_DIR}/termcap_map.c > termcap_hash.c
	)

add_custom_command(
	OUTPUT compiled_terms.c
	COMMAND TOOL_TIC=${CMAKE_BINARY_DIR}/tic/egg/tic_egg TERMINFO=${PROJECT_SOURCE_DIR}/termtypes.ti ${PROJECT_SOURCE_DIR}/genterms > compiled_terms.c
	)

add_custom_command(
	OUTPUT ${PROJECT_BINARY_DIR}/chicken/hash.c
	COMMAND TOOL_NBPERF=${CMAKE_BINARY_DIR}/nbperf/nbperf ${PROJECT_SOURCE_DIR}/genhash ${CMAKE_SOURCE_DIR}/include/term.h > hash.c
	)

add_custom_target(
	ti_generate ALL 
	DEPENDS compiled_terms.c termcap_hash.c ${PROJECT_BINARY_DIR}/chicken/hash.c
	)

add_library(terminfo
	../src/compile.c
	../src/curterm.c
	../src/term.c
	../src/termcap.c
	../src/termcap_map.c
	../src/ti.c
	../src/tparm.c
	../src/tputs.c
	${PROJECT_BINARY_DIR}/chicken/hash.c
	)
