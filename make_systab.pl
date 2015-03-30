#!/usr/bin/env perl

my $syscall_file = '/usr/include/sys/syscall.h';

my $reg_sys = '\/\* syscall: "([A-Za-z0-9_]*)" ret: "([A-Za-z0-9_\* ]*)" args: (.*)[ ]?\*\/';
my $reg_def = '\#define[ \t]+SYS_[a-z0-9_]*[ \t]+(\d+)';

open(FILE, $syscall_file) || die "Cannot read syscall header file `$syscall_file'";

print << "EOS";
#include    "ftrace.h"

t_systab    systab[] = {
EOS

my $ret;
my $name;
my $args;
my $line;
my $count = 0;

while (<FILE>) {
    if (/$reg_sys/) {
        $ret  = $2;
        $name = $1;
        $args = $3;
        $args =~ s/" "/, /g;
        $args =~ s/"//g;
        $args =~ s/ $//g;
        $line = "$ret $name($args)";
    }
    elsif (/$reg_def/) {
        my $num = $1;
        print << "EOE";
    {"$ret", "$name", "$args", "$line"},
EOE
        $count++;
    }
}
close FILE;

print << "EOF";
    {0, 0, 0, 0}
EOF

print "};\n";
