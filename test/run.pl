#!/usr/bin/perl --
use warnings;
use strict;
use feature 'say';
use English;
use File::Spec;
use constant SCRIPT_DIR => File::Spec->catpath((File::Spec->splitpath($PROGRAM_NAME))[0..1]);
use constant TEST_SCRIPT_NAME => 'Win-ScriptInvoke.cmd';
use constant TEST_CMD => (File::Spec->catfile(SCRIPT_DIR, File::Spec->updir(), TEST_SCRIPT_NAME));

# test files
use constant tests =>
(
    { script => 'shell_name_only.ruby', name => 'Shell basename without extension' },
    { script => 'shell_name_and_ext.ruby', name => 'Shell basename with extension' },
    { script => 'shell_name_and_wrong_ext.ruby', name => 'Shell name only with wrong extension' },
    { script => 'shell_name_and_wrong_ext.ruby', name => 'Shell name only with wrong extension no strip',
	opts => { strip_extension => undef }, expect_error => !undef },
    { script => 'full_path_with_ext.sh', name => 'Invoke full path with extension' },
    { script => 'full_path_no_ext.sh', name => 'Invoke full path without extension' },
    { script => 'invoke_with_arg_separator.sh', name => 'Invoke shell with arguments separator' },
    { script => 'invoke_with_multiple_args.sh', name => 'Invoke shell with multiple arguments' },
    { script => 'default_fallback_with_wrong_shell.pl', name => 'Fallback to default if wrong shell',
	opts => { default_cmd => 'perl' } },
    { script => 'default_fallback_with_wrong_shell.pl', name => 'Default shell without fallback',
	opts => { default_cmd => 'perl', fallback => undef }, expect_error => !undef },
    { script => 'default_fallback_with_no_shell.pl', name => 'No shell with no default', expect_error => !undef },
    { script => 'default_fallback_with_no_shell.pl', name => 'Default shell',
	opts => { default_cmd => 'perl', default_cmd_args => ['-MEnglish', '-w'] } },
    { script => 'shell_cmd_with_spaces.cmake', name => 'Shell name and arguments with spaces' },
    { script => 'ignore_bin_env.cmake', name => 'Ignore /bin/env interpreter' },
    { script => 'ignore_usr_bin_env.cmake', name => 'Ignore /usr/bin/env interpreter' },
    { script => 'shell_wrong_path.ruby', name => 'Wrong shell path' },
    { script => 'shell_wrong_path.ruby', name => 'Wrong shell path without strip path',
	opts => { strip_location => undef }, expect_error => !undef },
    { script => 'shell_wrong_path_and_ext.ruby', name => 'Wrong shell path and extension' },
    { script => 'shell_wrong_path_and_ext.ruby', name => 'Wrong shell path and extension without strip path',
	opts => { strip_location => undef }, expect_error => !undef },
    { script => 'shell_wrong_path_and_ext.ruby', name => 'Wrong shell path and extension without strip exe',
	opts => { strip_extension => undef }, expect_error => !undef },
    { script => 'shell_wrong_path_and_ext.ruby', name => 'Wrong shell path and extension without strip path and ext',
	opts => { strip_location => undef, strip_extension => undef }, expect_error => !undef }
);

use Test::More tests => scalar(@{[tests]});

for my $test_case (tests)
{
    my @cmd_line = TEST_CMD;

    if ($test_case->{'opts'} and $test_case->{'opts'}->{'default_cmd'})
    {
	push @cmd_line, '--default-cmd', $test_case->{'opts'}->{'default_cmd'};
	if ($test_case->{'opts'}->{'default_cmd_args'})
	{
	    push @cmd_line, '--default-cmd-arg', $ARG for @{$test_case->{'opts'}->{'default_cmd_args'}}
	}
	push @cmd_line, '--no-fallback-to-default' if exists $test_case->{'opts'}->{'fallback'} and not $test_case->{'opts'}->{'fallback'}
    }

    push @cmd_line, '--no-strip-extension' if exists $test_case->{'opts'}->{'strip_extension'}  and not $test_case->{'opts'}->{'strip_extension'};
    push @cmd_line, '--no-strip-location' if exists $test_case->{'opts'}->{'strip_location'} and not $test_case->{'opts'}->{'strip_location'};

    system {+ $ENV{'COMSPEC'} } '/C', '(', @cmd_line, '--', File::Spec->catfile(SCRIPT_DIR, $test_case->{'script'}), ')';

    if (defined $test_case->{'expect_error'})
    {
	ok($CHILD_ERROR, $test_case->{'name'});
    }
    else
    {
	ok(!$CHILD_ERROR, $test_case->{'name'});
    }
}
