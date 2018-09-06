#!C:\msys64\usr\bin\sh --

if test "$(/usr/bin/basename "$0")" = "invoke_with_arg_separator.sh" -a \( "$SHELL" = "/bin/sh" -o "$SHELL" = "/usr/bin/bash" \)
then
    echo "MinGW shell invoked with arguments separator."
fi
