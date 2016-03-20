cmake_minimum_required(VERSION 3.0)

add_executable(tic
	../tic.c
	../../general/cdbw.c
	../../general/mi_vector_hash.c
	../../general/arc4random.c
	../../general/getentropy.c
	../../general/sha512c.c
	)

target_link_libraries(tic terminfo)
