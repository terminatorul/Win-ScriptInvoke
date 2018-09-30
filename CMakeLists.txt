cmake_minimum_required(VERSION 3.10);

set(WIN_SCRIPT_INVOKE_SRCS
	src/HandlerInfo.cc
	src/ShellExtInit.cc
	src/ContextMenu.cc
	src/ContextMenuHandler.hh
	src/ContextMenuHandler.cc
	src/ClassFactory.hh
	src/ClassFactory.cc
	src/Win-ScriptInvoke.cc)

add_library(Win-ScriptInvoke SHARED ${WIN_SCRIPT_INVOKE_SRCS})
