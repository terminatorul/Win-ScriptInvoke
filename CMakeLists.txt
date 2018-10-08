cmake_minimum_required(VERSION 3.10);

set(WIN_SCRIPT_INVOKE_SRCS
	ScriptFile.hh
	ScriptFile.cc
	src/HandlerInfo.cc
	src/ShellExtInit.cc
	src/InitializeWithStream.cc
	src/ContextMenu.cc
	src/ContextMenuHandler.hh
	src/ContextMenuHandler.cc
	src/ClassFactory.hh
	src/ClassFactory.cc
	src/Win-ScriptInvoke.hh
	src/Win-ScriptInvoke.cc)

add_library(Win-ScriptInvoke SHARED ${WIN_SCRIPT_INVOKE_SRCS})
