cmake_minimum_required(VERSION 3.0)
project(netbsd_curses)

include_directories(after "include")
include_directories(after "general/include")

add_subdirectory(nbperf)
add_subdirectory(terminfo)
add_subdirectory(tic)
add_subdirectory(curses)

add_dependencies(curses terminfo)
add_dependencies(tic terminfo)
add_dependencies(terminfo ti_generate)
add_dependencies(ti_generate nbperf tic_egg)
add_dependencies(tic_egg terminfo_egg)
add_dependencies(terminfo_egg ti_generate_egg)
add_dependencies(ti_generate_egg nbperf)
