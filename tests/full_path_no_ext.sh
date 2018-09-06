#!C:\msys64\usr\bin\sh

if test "$(/usr/bin/basename "$0")" = "full_path_no_ext.sh" -a \( "$SHELL" = "/bin/sh" -o "$SHELL" = "/usr/bin/bash" \)
then
    echo "MinGW shell invoked from full path without extension."
fi
