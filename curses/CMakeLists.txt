cmake_minimum_required(VERSION 3.0)
project(curses)

include_directories(after "include")
include_directories(after "../terminfo/include")

add_library(curses
	src/acs.c
	src/add_wch.c
	src/add_wchstr.c
	src/addbytes.c
	src/addch.c
	src/addchnstr.c
	src/addnstr.c
	src/addwstr.c
	src/attributes.c
	src/background.c
	src/bell.c
	src/border.c
	src/box.c
	src/cchar.c
	src/chgat.c
	src/clear.c
	src/clearok.c
	src/clrtobot.c
	src/clrtoeol.c
	src/color.c
	src/copywin.c
	src/cr_put.c
	src/cur_hash.c
	src/curs_set.c
	src/curses.c
	src/delch.c
	src/deleteln.c
	src/delwin.c
	src/echo_wchar.c
	src/echochar.c
	src/erase.c
	src/fileio.c
	src/flushok.c
	src/fullname.c
	src/get_wch.c
	src/get_wstr.c
	src/getch.c
	src/getstr.c
	src/getyx.c
	src/id_subwins.c
	src/idcok.c
	src/idlok.c
	src/in_wch.c
	src/in_wchstr.c
	src/inch.c
	src/inchstr.c
	src/initscr.c
	src/ins_wch.c
	src/ins_wstr.c
	src/insch.c
	src/insdelln.c
	src/insertln.c
	src/insstr.c
	src/instr.c
	src/inwstr.c
	src/keypad.c
	src/leaveok.c
	src/line.c
	src/meta.c
	src/move.c
	src/mvwin.c
	src/newwin.c
	src/nodelay.c
	src/notimeout.c
	src/overlay.c
	src/overwrite.c
	src/pause.c
	src/printw.c
	src/putchar.c
	src/refresh.c
	src/resize.c
	src/scanw.c
	src/screen.c
	src/scroll.c
	src/scrollok.c
	src/setterm.c
	src/standout.c
	src/timeout.c
	src/toucholap.c
	src/touchwin.c
	src/tscroll.c
	src/tstp.c
	src/tty.c
	src/unctrl.c
	src/underscore.c
	)
