[![Build status](https://ci.appveyor.com/api/projects/status/q2s48eeh295xnksm/branch/master?svg=true)](https://ci.appveyor.com/project/terminatorul/win-shebang-line/branch/master)

# Win-ScriptInvoke
Windows file type handler for scripts with `#!` shebang line or other content marker line at the begining of the file.

## Linux shebang line
The shebang line (sharp-bang, that is `#` and `!`) is used as the first line of scripts on UNIX/Linux systems, to provide the path of the interpreter to run the script.

The [perl](http://www.perl.org/) interpreter on Linux is most likely installed as `/usr/bin/perl`, so the first line of a perl script file looks like:
```
#!/usr/bin/perl --
```
followed by actual perl source code. Lines starting with `#` are comments in Perl, so the interpreter is not bothered by this line, even though it is not meant for perl.

It is meant for the system shell (command interpreter, running for example in a terminal window), to know how to run the script when invoked as an executable command.

There are languages (like [php](http://php.net/)) with different rules for comments, that still ignore the first line if it starts with `#!`, just to support this mechanism.

## Windows file associations
On Windows the interpreter for a script file is provided by its file association. Each file has an associated type, usually identified by the file extension, and the file type will provide a proper command to run the script (together with more associated "actions" and properties).

## Same extension with different interpreters
Using a file extension as its type means all files of a given extension will be run with the same interpreter. This is rather intuitive for any user and most of the time it works nice.

But it is possible to have the same extension for files with different types. In my case I wanned to run perl scripts written with either [Perl 6](https://www.raku.org) (now called Raku) or [Perl 5](http://www.perl.org/), and use the same file extension `.pl` for both of them. A similar situation exists for [Python 3](https://docs.python.org/3/) and [Python 2](https://docs.python.org/2/) scripts. The two versions are not compatible with each other and they must use different interpreters.

In this case the shebang line can select the appropriate interpreter, while the file extension is no longer sufficient.

## Content marker line
Scripts like [Perl 6](https://www.raku.org) can also be identified by a specific line, reading for example `use v6;`, somewhere in the begining of the script file. So this file type handler can scan the first lines of the script for this purpose.

## Installation
To install the file type handler you should:
- choose a system-wide installation directory and clone or copy the script there. I used `C:\Local\bin\Win-ScriptInvoke.cmd`.
- choose the file extension, for example `.pl` for perl scripts, and a name for the file type, like `PerlFamilyScript`.
- remember to check that the file extension appears in `PATHEXT` environment variable. Append the extension, separated with `;`, if needed, like: `SetX PATHEXT "%PATHEXT%;.pl"`.
- start a `cmd` window as Administrator and run the below commands:
```
    Assoc .pl=PerlFamilyScript
    FType PerlFamilyScript="C:\Local\bin\Win-ScriptInvoke.cmd" --default-cmd perl -- "%1" %*
```
That's it. Open a non-admin console and run a script:
```
scriptName.pl
```
or just
```
scriptName
```

in case you have the `PATHEXT` variable configured. Windows may prompt you on the first use of the modified file type, to confirm file type change, so select the `Win-ScriptInvoke` handler then.

The file type handler will read the shebang line from the script and use the given interpreter to run it. A number of command-line options are available _for the file type handler_:
```
F:\Local\bin>Win-ScriptInvoke.cmd

Syntax:
    "Win-ScriptInvoke.cmd" [--options]... -- ScriptFile.ext [script-args]...

Read first line from ScriptFile.ext and use it as the command to run the script.

The first line should begin with the chrarcters #!, which are not considered part of
the command, and should be followed immediately by a command name and an optional argument.

Options are:

   --no-strip-location
   --strip-location
           The directory part of the #!-command is ignored if given but not found, and the
           command basename alone is searched on PATH instead. The default is --strip-location.

   --no-strip-extension
   --strip-extension
           The extension part of the #!-command is ignored if given but not found, and any
           executable found with the same name is invoked regardless of extension. The default
           is --strip-extension, although it is recommended that your script files not include
           the .exe/.bat/... file extension on the shebang line.

   --scan-line "line text"
           Scan the first 10 lines of the script for a line that reads "line text", and if found
           use the interpreter given with --spcific-cmd and --specific-cmd-arg

   --scan-line-count "25"
           Gives the number of initial lines in the script that will be scanned to look for the
           "line text" given with --scan-line. Default is to scan initial 50 lines, to account
           for licensing headers that sometimes clutter source code files.

   --specific-cmd "specificCmd.exe"
           Use "specificCmd.exe" to runt the script, if a line was found in the beginning of the
           script that reads like the text given with --scan-line

   --specific-cmd-arg "arg"
           multiple times to pass multiple arguments to specificCmd.

   --default-cmd "cmdName.exe"
           Use cmdName.exe to run the script if the #! line is not found at the begining of
           the scripit file.

   --default-cmd-arg "arg"
           Include given "arg" first on the command line when running "cmdName.exe". Use this
           option multiple times to pass multiple arguments.

   --no-fallback-to-default
   --fallback-to-default
           If the #!-command is not found at all and a --default-cmd is given, invoke that
           command to run the script, that is ignore the #!-command entirely if it is not
           usable. The default is --fallback-to-default.
```

## Usage
You can use the file type handler with any scripts that provide a shebang line or optionally a content marker line. If neither is not found at the begining of the script, then:
- if `--default-cmd` option is given, that command is invoked as the interpreter. Interpreter arguments given with `--default-cmd-arg` are included on the new command line.
- if there was no `--default-cmd` option given, then an error is show like the following:
```
Expected initial #! line with script interpreter name for <scriptFileName.ext>
```

The way the file type handler checks the interpreter from shebang line is:
- separate the line into interpreter pathname and an optional (additional) argument. There can be no more the one such argument on a shebang line for Linux systems, so your scripts should follow this limit.
- the interpreter name can be quoted to include file names with spaces. Same for optional arguments
- check for a cmd built-in command as the interpreter (they are listed with built-in `help` command) and run it if found
- if the interpreter is `/bin/env` or `/usr/bin/env`, it is ignored and the following argument is used instead. This use case allows UNIX systems to pick an interpreter from anywhere on  `PATH`, while still starting the shebang line with an absolute file name. See the [env](http://pubs.opengroup.org/onlinepubs/9699919799/utilities/env.html) command in The Open Group specification.
- check if the given interpreter exists as a file name, and invoke it if so
- check if any extensions from `PATHEXT` list can be appended to the interpreter name, so it can be found as a file name
- if no path component is given for the interpreter name, search the base file name on `PATH`. For each of the paths:
    * append every extension from `PATHEXT` in turn and see if the resulting name can be found. These checks allow you to use a shebang line like `#!perl`, and then `perl.exe` can be found (same process used by `cmd.exe`).
- if above steps fail then keep the base name of the interpreter and remove (ignore) the path. This is meant to support Linux paths like `/usr/bin/perl6` that will not be found on Windows, but the base name `perl6` may still be found (on `PATH`). Also remove the extension from the base name. That is, an interpreter given as `perl6.exe` can be replaced with `perl6.bat` if the later is found on `PATH`. This behavior can be disabled with `--no-strip-location` and `--no-strip-extension` options.
- if there is no shebang line at the start of the file, the first lines are then searched for a content marker line, if one was provided. If a content marker like is found, the associated interpreter and interpreter args are invoked, to run the script with the script args.
- if all checks fail then use the default command if it was given with `--default-cmd` option
- without `--default-cmd`, display a message like 
  `scriptName.cmd: Bad interpreter: <interpreter name>`
  and attempt to run the (_missing_) interpreter as given, which will then produce an error from `cmd.exe`.

As it is a `.cmd` script, the file type handler will open a console window in order to run the script. This is an issue if the script is a graphical (GUI) application. For this case a non-console version of the file type handler will be needed, that is _not_ provided here (maybe later).

Also note that file type handlers on Windows can be more complex, based on registering shell-specific .dll functions, so this script is a simple approach to the issue.
