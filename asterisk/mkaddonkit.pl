# make Asterisk channel driver add-on source kit
#
# This script creates an archive package from the latest ooh323c source
# code named 'asterisk-ooh323c-<version>.tar.gz'.  This can be unpacked
# in the Asterisk addons root directory to replace the existing channel
# driver with the latest one.  The code can then be built and installed
# using the Asterisk add-ons build procedure.

# $g_debug = 1;

sub usage {
    print "usage: mksrckit <version>\n";
    print "  <version>  Channel driver version x.x (eg: 0.7)\n";
    exit(0);
}

sub pdiag {
    if ($g_debug) {
        print @_;
    }
}

# process args

$numargs = @ARGV;
if ($numargs < 1) {
    &usage ;
}

my $version = $ARGV[0];
my $pkgname = "asterisk-ooh323c-${version}";

# create root directory structure

if (-e "../channels") {
    if (-e "../channels_old") {
        `rm -rf ../channels_old`;
    }
    `mv ../channels ../channels_old`;
}

mkdir ("../channels", 0777);

# copy Asterisk driver files
print `cp -f ./*.[ch] ../channels`;

# copy ooh323c files
chdir ("..");
mkdir ("./channels/ooh323c", 0777);
mkdir ("./channels/ooh323c/src", 0777);
mkdir ("./channels/ooh323c/src/h323", 0777);
print `cp -f ./src/*.h ./channels/ooh323c/src`;
print `cp -f ./src/*.c ./channels/ooh323c/src`;
print `cp -f ./src/h323/*.h ./channels/ooh323c/src/h323`;
print `cp -f ./src/h323/*.c ./channels/ooh323c/src/h323`;
print `cp -f ./src/h323_v6/*.h ./channels/ooh323c/src/h323`;
print `cp -f ./src/h323_v6/*.c ./channels/ooh323c/src/h323`;

# tar and gzip
`tar -cf ${pkgname}.tar channels`;
`gzip ${pkgname}.tar`;
