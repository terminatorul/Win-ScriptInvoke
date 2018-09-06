#!/usr/bin/perl --
use warnings;
use strict;
use feature 'say';
use English;
use File::Spec;
use constant SCRIPT_DIR => File::Spec->catpath((File::Spec->splitpath($PROGRAM_NAME))[0..1]);
use constant TEST_SCRIPT_NAME => 'invoke_perl_shebang.cmd';
use constant TEST_CMD => (File::Spec->catfile(SCRIPT_DIR, File::Spec->updir(), TEST_SCRIPT_NAME));

# test files
use constant tests =>
(
    { script => 'full_path_with_ext.sh', name => 'Invoke full path with extension' },
    { script => 'full_path_no_ext.sh', name => 'Invoke full path without extension' },
    { script => 'invoke_with_arg_separator.sh', name => 'Invoke shell with arguments separator' },
    { script => 'invoke_with_multiple_args.sh', name => 'Invoke shell with multiple arguments' },
    { script => 'default_fallback_with_wrong_shell.pl', name => 'Fallback to default with wrong shell' },
);

use Test::More tests => scalar(@{[tests]});

for my $test_case (tests)
{
    ok(system(TEST_CMD, File::Spec->catfile(SCRIPT_DIR, $test_case->{'script'})) == 0, $test_case->{'name'});
}
