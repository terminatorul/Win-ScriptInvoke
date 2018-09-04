# win-shebang-line
Windows file type handler for scripts with `#!` shebang line.

## Linux shebang line
The shebang line (sharp-bang, that is `#` and `!`) is used as the first line of scripts on UNIX/Linux systems to provide the path of the interpreter to run the script.

The [perl](http://www.perl.org/) interpreter on Linux is most likely installed as `/usr/bin/perl`, so the first line of a perl script file looks like:
```
#!/usr/bin/perl --
```
followed by actual perl source code. Lines starting with `#` are comments in Perl, so the interpreter is not bothered by this line, even though the line is not meant for perl itself.

It is meant for the system shell (command interpreter, running for example in a terminal window), to know how to run the script when invoked as an executable command.

## Windows file associations
On Windows the interpreter for a script file is provided by its file association. Each file has an associated type, usually identified by the file extension, and the file type will provide a proper command to run the script (together with other associated "actions" and properties).

## Same extension with different interpreters
Using a file extension as its type means all files of a given extension will be run with the same interpreter. This is rather intuitive for any user and most of the time it works nice.

But it is possible to have the same extension for files with otherwise different types. In my case I wanned to run perl scripts written with either [Perl 6]("https://perl6.org") or [Perl 5](http://www.perl.org/), and use the same file extension `.pl` for any of them. A similar situation exists for [Python 3](https://docs.python.org/3/) and [Python 2](https://docs.python.org/2/) scripts. The two versions are not compatible with each other and they must use different interpreters.

In this case the shebang line can still select the appropriate interpreter, as the file extension is no longer sufficient.

## Install
To install the file type handler you should:
- choose a system-wide installation directory and clone or copy the script there. I used `C:\Local\invoke_perl_shebang.cmd`. You can add the location to your PATH, and you no longer need the absolute file name.
- choose the file extension, for example .pl for perl scripts, and a name for the file type, like PerlScriptFile.
- remember to check that the file extension appears in %PATHEXT% environment variable. Append the extension, separated with `;`, if needed, like: `setx PATHEXT=%PATHEXT%;.pl`.
- start a `cmd` window as administrator and run the below commands:
```
    Assoc .pl=PerlScriptFile
    FType PerlScriptFile="C:\Local\invoke_perl_shebang.cmd" "%1" %*
```
That's it. Now open a non-admin console and run a script:
```
scriptName.pl
```

The file type handler (`invoke_perl_shebang.cmd`) will read the shebang line from the script and use the interprter given to run it.

## Usage
You can use the file type handler with any scripts that provide the shebang line, but if the line is not found, the handler will assume a perl script by default, so it will:
- check to see if the interprter name ends with `perl6`, `perl6.exe`, `perl6.bat`, etc, and use the plain `perl6` command to run the script file
- failing that, it will use the plain `perl` command instead.
This fallback will not help if you use other languages (TODO: fix script to make it more generic).

The way the file type handler checks the interpreter from shebang line is:
- separate the line into the interpreter pathname and an optional (additional) argument. There can be no more the one such argument on a shebang line for Linux systems, but the file type handler does not have a specific limit for it here.
- the interpreter name can be quoted to include file names with spaces. Same for the optional argument
- if the given interpreter is `/bin/env` or `/usr/bin/env`, it is ignored and the following argument is used instead as the actual interpreter. This use case was usefull at some time on Linux in order to pick an interpreter from anywhere on the system PATH, while providing an absolute path name on the shebang line. See the [env](http://pubs.opengroup.org/onlinepubs/9699919799/utilities/env.html) command in the POSIX specification.
- check if the given interpreter exists as a file name, and use it if so
- check if any extesions from %PATHEXT% list can be appended to the interpreter name, so it can be found as a file name, and use it if so
- search the given interpreter on system PATH
- try to append every extension from %PATHEXT% in turn and see if the resulting name can be found on PATH. These checks allow you to use a shebang line like`#!perl`, and then perl.exe will be found anywhere on your PATH (if not already in the current directory).
- if all above steps fail then keep the base name of the interpreter and remove (ignore) the path. This is usefull for Linux paths like `/usr/bin/perl6` that will not normally be found when on Windows. But the base name `perl6` may still be found.
- check if the basename is `perl6` and use `perl6` command if so. TODO: no need to stick to `perl6` here, any base name can be further searched on path.
- if all checks fail then use `perl` command to run the script. On Linux systems `perl` will check the shebang line itself, but not on Windows (except to extract arguments to the perl command if any are found there).

As it is a `.cmd` script, the file type handler will open a console window in order to run the script. This is an issue if the script is a graphical (GUI) application. For this case a non-console version of the file type handler will be needed, that is _not_ provided here (maybe later).
