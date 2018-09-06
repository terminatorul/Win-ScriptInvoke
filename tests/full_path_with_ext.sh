#!C:\msys64\usr\bin\sh.exe

if test "$(/usr/bin/basename "$0")" = "full_path_with_ext.sh" -a \( "$SHELL" = "/bin/sh" -o "$SHELL" = "/usr/bin/bash" \)
then
    echo "MinGW shell invoked from full path and extension."
fi
