#! /usr/bin/perl
use POSIX;

my $a;

print "uint8_t wheel_r[360]={";

for($a = 0;$a < 360; $a++){
    $t = floor($a/60) % 6;
    
    if( $t == 4  ) {
	print int((255/60) * ($a%60)).",";
    }
    if( ($t == 0)  || ($t == 5)) {
	print "255,";
    }
    if( ($t == 1) ) {
	print int(255 - (((255/60) * ($a%60)))).",";
    }
    if( ($t == 2) || ($t == 3)) {
	print "0,";
    }
}

print "};\nuint8_t wheel_g[360]={";
for($a = 0;$a < 360; $a++){
    $t = floor($a/60) % 6;
    if( $t == 0  ) {
	print int((255/60) * $a).",";
    }
    if( ($t == 1)  || ($t == 2)) {
	print "255,";
    }
    if( ($t == 3) ) {
	print int(255 - (((255/60) * ($a%60)))).",";
    }
    if( ($t == 4) || ($t == 5)) {
    print "0,";
    }
}

print "};\nuint8_t wheel_b[360]={";
for($a = 0;$a < 360; $a++){
    $t = floor($a/60) % 6;
    
    if( $t == 2  ) {
	print int((255/60) * ($a%60)).",";
    }
    if( ($t == 3)  || ($t == 4)) {
	print "255,";
    }
    if( ($t == 5) ) {
	print int(255 - (((255/60) * ($a%60)))).",";
    }
    if( ($t == 0) || ($t == 1)) {
	print "0,";
    }
}

