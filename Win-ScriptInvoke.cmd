@Echo Off

SetLocal EnableDelayedExpansion

GoTo :ParseCommandLine

:ShowSyntax
Echo.
Echo Syntax:>&2
Echo     "%~0" [--options]... -- ScriptFile.ext [script-args]...>&2
Echo.>&2
Echo Read first line from ScriptFile.ext and use it as the command to run the script.>&2
Echo.>&2
Echo The first line should begin with the chrarcters #^^!, which are not considered part of>&2
Echo the command, and should be followed immediately by a command name and an optional argument.>&2
Echo.>&2
Echo Options are:>&2
Echo.>&2
Echo    --no-strip-location>&2
Echo    --strip-location>&2
Echo            the directory part of the #^^!-command is ignored if given but not found, and the>&2
Echo            command basename alone is searched on PATH instead. The default is --strip-location.>&2
Echo.>&2
Echo    --no-strip-extension>&2
Echo    --strip-extension>&2
Echo            The extension part of the #^^!-command is ignored if given but not found, and any>&2
Echo            executable found with the same name is invoked regardless of its extension. The>&2
Echo            default is --strip-extension, although it is recommended that your script files>&2
Echo            not include the .exe/.bat file extension on the shebang line.>&2
Echo.>&2
Echo    --default-cmd "cmdName.exe">&2
Echo            Use cmdName.exe to run the script if the #^^! line is not found at the begining of>&2
Echo            the scripit file.>&2
Echo.>&2
Echo    --default-cmd-arg "arg">&2
Echo            Include given "arg" on the command line when running "cmdName.exe". Use this option>&2
Echo            multiple times to pass multiple arguments to cmdName.>&2
Echo.>&2
Echo    --no-fallback-to-default>&2
Echo    --fallback-to-default>&2
Echo            If the #^^!-command is not found at all and a --default-cmd is given, invoke that>&2
Echo            command to run the script, that is ignore the #^^!-command entirely if it is not>&2
Echo            usable. The default is --fallback-to-default.>&2

Exit /B 254

:ParseCommandLine
Rem Parse command line
If x"%1" == x"" (
    GoTo :ShowSyntax
)

Set SCRIPT_INVOKER_DEBUG=

Set SCRIPT_INVOKE_STRIP_LOCATION=1
Set SCRIPT_INVOKE_STRIP_EXTENSION=1
Set SCRIPT_INVOKE_DEFAULT_CMD=
Set SCRIPT_INVOKE_DEFAULT_CMD_ARGS=
Set SCRIPT_INVOKE_FALLBACK_TO_DEFAULT=1

:ParseCommandLineOption
If /I "%~1" == "/?"	 GoTo :ShowSyntax
If /I "%~1" == "-h"	 GoTo :ShowSyntax
If /I "%~1" == "--help"	 GoTo :ShowSyntax
If /I "%~1" == "--usage" GoTo :ShowSyntax

If /I "%~1" == "--debug" (
    Set SCRIPT_INVOKER_DEBUG=1
    GoTo :NextCommandArg
)

If /I "%~1" == "--strip-location" (
    Set SCRIPT_INVOKE_STRIP_LOCATION=1
    GoTo :NextCommandArg
)

If /I "%~1" == "--no-strip-location" (
    Set SCRIPT_INVOKE_STRIP_LOCATION=
    GoTo :NextCommandArg
)

If /I "%~1" == "--strip-extension" (
    Set SCRIPT_INVOKE_STRIP_EXTENSION=1
    GoTo :NextCommandArg
)

If /I "%~1" == "--no-strip-extension" (
    Set SCRIPT_INVOKE_STRIP_EXTENSION=
    GoTo :NextCommandArg
)

If /I "%~1" == "--default-cmd" (
    Set SCRIPT_INVOKE_DEFAULT_CMD=%2
    Shift
    GoTo :NextCommandArg
)

If /I "%~1" == "--default-cmd-arg" (
    Set SCRIPT_INVOKE_DEFAULT_CMD_ARGS=!SCRIPT_INVOKE_DEFAULT_CMD_ARGS! %2
    Shift
    GoTo :NextCommandArg
)

If /I "%~1" == "--fallback-to-default" (
    Set SCRIPT_INVOKE_FALLBACK_TO_DEFAULT=1
    GoTo :NextCommandArg
)

If /I "%~1" == "--no-fallback-to-default" (
    Set SCRIPT_INVOKE_FALLBACK_TO_DEFAULT=
    GoTo :NextCommandArg
)

If "%~1" == "--" (
    Shift
    GoTo :ParseScriptCommand
)

Echo Unknown argument %1 >&2
GoTo :ShowSyntax

:NextCommandArg
Shift
GoTo :ParseCommandLineOption

:ParseScriptCommand
Set SCRIPT_FILE_NAME=%1
Set SCRIPT_COMMAND_LINE=!SCRIPT_FILE_NAME!
Shift

:ParseScriptOption
If Not x%1 == x (
    Set SCRIPT_COMMAND_LINE=!SCRIPT_COMMAND_LINE! %1
    Shift
    GoTo :ParseScriptOption
)

If Defined SCRIPT_INVOKER_DEBUG (
    Echo Strip location: !SCRIPT_INVOKE_STRIP_LOCATION!
    Echo Strip extension: !SCRIPT_INVOKE_STRIP_EXTENSION!
    Echo Default command: !SCRIPT_INVOKE_DEFAULT_CMD! !SCRIPT_INVOKE_DEFAULT_CMD_ARGS!
    Echo Fallback to cmd: !SCRIPT_INVOKE_FALLBACK_TO_DEFAULT!
    Echo Script command line: !SCRIPT_COMMAND_LINE!
)

For /F "usebackq tokens=1,* delims=!" %%i In (!SCRIPT_FILE_NAME!) Do (
    Rem Echo %%i %%j

    Rem Check the first line begins with #!
    If x"%%i" == x"#" (
	If Not x"%%j" == x"" (
	    Rem Echo "Found shebang line: %%i^!%%j"

	    Call :ParseShebangLine %%j
	    Rem Echo Interpreter: !INTERPRETER!
	    Rem Echo Args:        !INTERPRETER_ARGS!

	    for %%n In ("/bin/env" "/usr/bin/env") Do (
		If "%%~n" == "!INTERPRETER_FILE_NAME!" Call :ParseShebangLine !INTERPRETER_ARGS!
	    )

	    Set INTERPR=!INTERPRETER!

	    If Defined SCRIPT_INVOKE_STRIP_EXTENSION (
		For %%k In (!INTERPRETER!) Do (
		    If Not x"%%~xk" == x"" (
			Call :TrimFileExtension !INTERPRETER!
			Set INTERPR=!INTERPR! "!TRIMMED!"
		    )
		)
	    )

	    If Defined SCRIPT_INVOKER_DEBUG (
		Echo INTERPR: !INTERPR!
	    )

	    If "!INTERPRETER_FILE_NAME:\=;!" == "!INTERPRETER_FILE_NAME!" (
		If "!INTERPRETER_FILE_NAME::=;!" == "!INTERPRETER_FILE_NAME!" (
		    Rem No path to interpreter, only name given, like: #!cmake
		    For %%w In (!INTERPR!) Do (
			For %%y In ("." "%PATH:;=" "%") Do (
			    For %%x In ("" "%PATHEXT:;=" "%") Do (
				If Exist "%%~y\%%~w%%~x" (
				    Set INTERPRETER="%%~y\%%~w%%~x"
				    GoTo :RunInterpreterWithArgs
				)
			    )
			)
		    )
		)

		GoTo :StripLocation
	    )
	    
	    Rem Some path to interpreter given, like #!\Local\bin\sed or #!C:sed.exe
	    Rem Skip PATH search
	    For %%w In (!INTERPR!) Do (
		For %%x In ("" "%PATHEXT:;=" "%") Do (
		    If Exist "%%~w%%~x" (
			Set INTERPRETER="%%~w%%~x"
			GoTo :RunInterpreterWithArgs
		    )
		)
	    )

:StripLocation
	    If Defined SCRIPT_INVOKE_STRIP_LOCATION (
		Call :GetBaseName !INTERPRETER!

		For %%y In (!INTERPRETER!) Do (
		    If /I Not "!BASENAME!" == "%%~y" (
			Set INTERPR="!BASENAME!"

			If Defined SCRIPT_INVOKE_STRIP_EXTENSION (
			    For %%k In ("!BASENAME!") Do (
				If Not x"%%~xk" == x"" (
				    Call :TrimFileExtension "!BASENAME!"
				    Set INTERPR=!INTERPR! "!TRIMMED!"
				)
			    )
			)

			For %%w In (!INTERPR!) Do (
			    For %%k In ("." "%PATH:;=" "%") Do (
				For %%l In ("" "%PATHEXT:;=" "%") Do (
				    If Exist "%%~k\%%~w%%~l" (
					Set INTERPRETER="%%~k\%%~w%%~l"
					GoTo :RunInterpreterWithArgs
				    )
				)
			    )
			)
		    )
		)
	    )

	    Rem Check for a built-in cmd command
	    For /F "usebackq delims=`" %%l In (`help`) Do (
		Set LINE=%%l
		If Not "!LINE:~0,1!" == " " (
		    For %%y in (!INTERPRETER!) Do (
			For /F %%c In ("!LINE!") Do (
			    If /I "%%c" == "%%~y" (
				Set INTERPRETER=%%~y
				GoTo :RunInterpreterWithArgs
			    )
			)
		    )
		)
	    )

	    Rem #!-command was not found in any way
	    If Defined SCRIPT_INVOKE_FALLBACK_TO_DEFAULT (
		If Defined SCRIPT_INVOKE_DEFAULT_CMD (
		    Set INTERPRETER=!SCRIPT_INVOKE_DEFAULT_CMD!
		    Set INTERPRETER_ARGS=!SCRIPT_INVOKE_DEFAULT_CMD_ARGS!
		    GoTo :RunInterpreterWithArgs
		)
	    )

	    Rem Invalid #!-command interpreter, will trigger an error on invocation
	    Echo !SCRIPT_FILE_NAME!: Bad interpreter !INTERPRETER! >&2
	    GoTo :RunInterpreterWithArgs
	)
    )

    Rem No shebang line
    If Defined SCRIPT_INVOKE_DEFAULT_CMD (
	Set INTERPRETER=!SCRIPT_INVOKE_DEFAULT_CMD!
	Set INTERPRETER_ARGS=!SCRIPT_INVOKE_DEFAULT_CMD_ARGS!
	GoTo :RunInterpreterWithArgs
    )

    Echo Expected initial #^^! line with script interpreter name for !SCRIPT_FILE_NAME! >&2
    Exit /B 255
)

:ParseShebangLine
Set INTERPRETER=
Set INTERPRETER_FILE_NAME=
Set INTERPRETER_ARGS=

For %%i in (%*) Do (
    If Defined INTERPRETER (
	Set INTERPRETER_ARGS=!INTERPRETER_ARGS! %%i
    ) Else (
	Set INTERPRETER=%%i
	Set INTERPRETER=!INTERPRETER:/=\!
	Set INTERPRETER_FILE_NAME=%%~i
    )
)
GoTo :EOF

:GetBaseName
Set BASENAME=%~1
:StripDirectoryComponent
If Not "!BASENAME!" == "!BASENAME:*/=!" (
    Set BASENAME=!BASENAME:*/=!
    GoTo :StripDirectoryComponent
)
If Not "!BASENAME!" == "!BASENAME:*\=!" (
    Set BASENAME=!BASENAME:*\=!
    GoTo :StripDirectoryComponent
)

If Defined SCRIPT_INVOKER_DEBUG (
    Echo Script basename: !BASENAME!
)

GoTo :EOF

:TrimFileExtension
Set TRIMMED=%~1
:TrimLastChar
If "!TRIMMED:~-1!" == "." (
    Set TRIMMED=!TRIMMED:~0,-1!
) Else (
    Set TRIMMED=!TRIMMED:~0,-1!
    GoTo :TrimLastChar
)
If Defined SCRIPT_INVOKER_DEBUG (
    Echo Trimmed name: !TRIMMED!
)
GoTo :EOF

:RunInterpreterWithArgs
If Defined SCRIPT_INVOKER_DEBUG (
    Echo Running shebang line !INTERPRETER! !INTERPRETER_ARGS! !SCRIPT_COMMAND_LINE!
)
!INTERPRETER! !INTERPRETER_ARGS! !SCRIPT_COMMAND_LINE!
Exit /B %ErrorLevel%
GoTo :EOF
