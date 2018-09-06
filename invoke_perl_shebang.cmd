@Echo Off

SetLocal EnableDelayedExpansion

Set PERL5_CMD=perl
Set PERL6_CMD=perl6

For /F "usebackq tokens=1,* delims=!" %%i In ("%~1") Do (
    Rem Echo %%i %%j

    Rem Check the first line begins with #!
    If x"%%i" == x"#" (
	If Not x"%%j" == x"" (
	    Rem Echo "Found shebang line: %%i^!%%j"

	    Call :ParseShebangLine %%j
	    Rem Echo Interpreter: !INTERPRETER!
	    Rem Echo Args:        !INTERPRETER_ARGS!

	    for %%n In ("/bin/env" "/usr/bin/env") Do (
		If "%%~n" == "!INTERPRETER!" Call :ParseShebangLine !INTERPRETER_ARGS!
	    )

	    Rem Check full path (with extension) is given for the interpreter
	    If Exist "!INTERPRETER!" (
		Rem Echo "Found shebang line explicit interpreter !INTERPRETER!"
		GoTo :RunInterpreterWithArgs
	    )

	    For %%m In ("!INTERPRETER!") Do (
		If Exist "%%~$PATH:m" GoTo RunInterpreterWithArgs
	    )

	    For %%l In (%PATHEXT:;= %) Do (
		Rem check absolute path without extenstion is given for the interpreter
		If Exist "!INTERPRETER!%%l" (
		    Rem Echo "Found shebang line interpretter !INTERPRETER!%%l"
		    GoTo :RunInterpreterWithArgs
		)

		Rem check basename is given for the interpreter on PATH
		For %%m In ("!INTERPRETER!%%l") Do (
		    Set INTERPRETER_PATH=%%~$PATH:m
		    If Exist "!INTERPRETER_PATH!" (
			Rem Echo "Found shebang line interpreter on PATH: !INTERPRETER_PATH!"
			GoTo :RunInterpreterWithArgs
		    )
		)
	    )

	    Rem check interpreter basename is perl6
	    If /I "!INTERPRETER!" == "perl6" GoTo :RunPerl6CmdWithArgs

	    For %%l In (%PATHEXT:;= %) Do (
		If /I "!INTERPRETER!" == "perl6%%l" GoTo :RunPerl6CmdWithArgs
	    )

	    Call :GetBaseName "!INTERPRETER!"

	    Rem Echo Interpreter base name: !BASENAME!

	    For %%k In ("perl6") Do (
		If /I "!BASENAME!" == "%%~k" GoTo :RunPerl6CmdWithArgs

		For %%l In (%PATHEXT:;= %) Do (
		    If /I "!BASENAME!" == "%%~k%%l" GoTo :RunPerl6CmdWithArgs
		)
	    )
	)
    )

    Rem No shebang line or interpreter not understood
    GoTo :RunPlainPerl5Cmd
)

:ParseShebangLine
Set INTERPRETER=%~1
Call :ParseShebangArguments %2 %3 %4 %5 %6 %7 %8 %9
GoTo :EOF

:ParseShebangArguments
Set INTERPRETER_ARGS=%*
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
GoTo :EOF

:RunInterpreterWithArgs
If Defined DEBUG_SHEBANG_INVOKER (
    Echo Running shebang line "!INTERPRETER!" !INTERPRETER_ARGS! %*
)
"!INTERPRETER!" !INTERPRETER_ARGS! %*
Exit /B %ErrorLevel%
GoTo :EOF

:RunPerl6CmdWithArgs
If Defined DEBUG_SHEBANG_INVOKER (
    Echo Running perl6 command %PERL6_CMD% !INTERPRETER_ARGS! %*
)
%PERL6_CMD% !INTERPRETER_ARGS! %*
Exit /B %ErrorLevel%
GoTo :EOF

:RunPlainPerl5Cmd
If Defined DEBUG_SHEBANG_INVOKER (
    Echo Running plain perl5 command %PERL5_CMD% %*
)
%PERL5_CMD% %*
Exit /B %ErrorLevel%
GoTo :EOF

:RunPlainPerl6Cmd
If Defined DEBUG_SHEBANG_INVOKER (
    Echo Running plain perl6 command %PERL6_CMD% %*
)
%PERL6_CMD% %*
Exit /B %ErrorLevel%
GoTo :EOF
