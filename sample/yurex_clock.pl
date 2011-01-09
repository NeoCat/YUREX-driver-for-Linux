#!/usr/bin/perl -l
use strict;
use Time::HiRes qw(usleep gettimeofday);

for(;;) {
	my @d = localtime;
	my $d = sprintf "%02d%02d%02d%02d%02d",$d[4]+1,$d[3],$d[2],$d[1],$d[0];
	print $d;
	yurex_cmd(int($d));    # display current date&time
	yurex_cmd("L1");  # turn LED on
	yurex_cmd("L0");  # turn LED off
	my ($s, $us) = gettimeofday;
	usleep 1000000-$us;
}

sub yurex_cmd {
	open my $yurex, ">", "/dev/yurex0" or die "cannot open yurex";
	print $yurex shift;
	close $yurex;
}
