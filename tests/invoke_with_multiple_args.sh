#!C:\msys64\usr\bin\sh -u -e --

if test "$(/usr/bin/basename "$0")" = "invoke_with_multiple_args.sh" -a \( "$SHELL" = "/bin/sh" -o "$SHELL" = "/usr/bin/bash" \)
then
    echo "MinGW shell invoked with multiple args -u -x."
fi
