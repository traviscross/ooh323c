#!/usr/local/bin/perl
#
# * Copyright (C) 2004-2005 by Objective Systems, Inc.
# *
# * This software is furnished under an open source license and may be
# * used and copied only in accordance with the terms of this license.
# * The text of the license may generally be found in the root
# * directory of this installation in the COPYING file.  It
# * can also be viewed online at the following URL:
# *
# *   http://www.obj-sys.com/open/license.html
# *
# * Any redistributions of this file including modified versions must
# * maintain this copyright notice.
# *
# ***************************************************************************/
if(@ARGV < 2) {
   print "Usage - perl mkfwkit.pl [win|unix|src] [version]";
   exit(0);
}
print "Preparing ", $ARGV[0], " kit\n";


$version = $ARGV[1];


print `rm -rf ooh323c-${version}`;
mkdir ("ooh323c-${version}", 0777);
print "Copying top level ooh323c dir\n";
if ($ARGV[0] eq "unix") {
   print `cp -f ./AUTHORS ./ooh323c-${version}/AUTHORS`;
   print `cp -f ./COPYING ./ooh323c-${version}/COPYING`;
   print `cp -f ./ChangeLog ./ooh323c-${version}/ChangeLog`;
}
if ($ARGV[0] eq "win") {
   print `cp -f ./COPYING ./ooh323c-${version}/COPYING.txt`;
   print `cp -f ./ChangeLog ./ooh323c-${version}/ChangeLog.txt`;
}

#ooh323c directory
if ($ARGV[0] eq "src") {
   print `cp -f ./AUTHORS ./ooh323c-${version}/AUTHORS`;
   print `cp -f ./COPYING ./ooh323c-${version}/COPYING`;
   print `cp -f ./ChangeLog ./ooh323c-${version}/ChangeLog`;
   print `cp -f ./INSTALL ./ooh323c-${version}/INSTALL`;
   print `cp -f ./Makefile.am ./ooh323c-${version}/Makefile.am`;
   print `cp -f ./Makefile.in ./ooh323c-${version}/Makefile.in`;
   print `cp -f ./NEWS ./ooh323c-${version}/NEWS`;



   print `cp -f ./aclocal.m4 ./ooh323c-${version}/aclocal.m4`;
   print `cp -f ./configure ./ooh323c-${version}/configure`;
   print `cp -f ./configure.in ./ooh323c-${version}/configure.in`;
   print `cp -f ./config.h.in ./ooh323c-${version}/config.h.in`;
   print `cp -f ./ooh323c.dsw ./ooh323c-${version}/ooh323c.dsw`;

   mkdir ("ooh323c-${version}/config", 0777);
   print `cp -f -r ./config/* ./ooh323c-${version}/config`;
   print `rm -rf ./ooh323c-${version}/config/CVS`;
}



#ooh323c docs
if($ARGV[0] eq "src") {
   print `cp -f ./README ./ooh323c-${version}/README`;
}

if($ARGV[0] eq "win") {
   print `cp -f ./README.win32.txt ./ooh323c-${version}/README.txt`;
}

if($ARGV[0] eq "unix") {
   print `cp -f ./README.unix ./ooh323c-${version}/README`;
}

print "Copying ooh323c docs\n";
mkdir ("ooh323c-${version}/doc", 0777);
mkdir ("ooh323c-${version}/doc/h323stk/", 0777);
mkdir ("ooh323c-${version}/doc/h323stk/images", 0777);
print `cp -f -r ./doc/H323Introduction.pdf ./ooh323c-${version}/doc/.`;
print `cp -f -r ./doc/ooH323cUsersGuide.pdf ./ooh323c-${version}/doc/.`;
if ($ARGV[0] eq "src") {
   print `cp -f -r ./doc/footer.shtml ./ooh323c-${version}/doc/.`;
   print `cp -f -r ./doc/header.shtml ./ooh323c-${version}/doc/.`;
   print `cp -f -r ./doc/stylesheet.css ./ooh323c-${version}/doc/.`;
}
print `cp -f -r ./doc/h323stk/* ./ooh323c-${version}/doc/h323stk`;
print `rm -rf ./ooh323c-${version}/doc/h323stk/CVS`;
print `cp -f -r ./doc/h323stk/images/* ./ooh323c-${version}/doc/h323stk/images`;
print `rm -rf ./ooh323c-${version}/doc/h323stk/images/CVS`;


#oomedia docs
print "Copying media docs\n";
mkdir ("ooh323c-${version}/doc/media", 0777);
print `cp -f -r ./media/doc/MediaLibRef0.2.pdf ./ooh323c-${version}/doc/.`;
print `cp -f -r ./doc/media/* ./ooh323c-${version}/doc/media/.`;
print `rm -rf ./ooh323c-${version}/doc/media/CVS`;

#specs
print "Copying specs\n";
mkdir ("ooh323c-${version}/specs", 0777);
print `cp -f ./specs/*.asn ./ooh323c-${version}/specs`;

#ooh323c library source files
if ($ARGV[0] eq "src") {
   print "Copying ooh323c sources\n";
   mkdir ("ooh323c-${version}/src", 0777);
   mkdir ("ooh323c-${version}/src/h323", 0777);
   mkdir ("ooh323c-${version}/src/h323_v6", 0777);
   print `cp -f ./src/Makefile.am ./ooh323c-${version}/src/Makefile.am`;
   print `cp -f ./src/Makefile.in ./ooh323c-${version}/src/Makefile.in`;
   print `cp -f ./src/oostk.dsp ./ooh323c-${version}/src/oostk.dsp`;
   print `cp -f ./src/*.h ./ooh323c-${version}/src`;
   print `cp -f ./src/*.c ./ooh323c-${version}/src`;
   print `cp -f ./src/h323/*.h ./ooh323c-${version}/src/h323`;
   print `cp -f ./src/h323/*.c ./ooh323c-${version}/src/h323`;
   print `cp -f ./src/h323_v6/*.h ./ooh323c-${version}/src/h323_v6`;
   print `cp -f ./src/h323_v6/*.c ./ooh323c-${version}/src/h323_v6`;
}


#create example directories
mkdir ("ooh323c-${version}/tests", 0777);
mkdir ("ooh323c-${version}/tests/simple", 0777);
mkdir ("ooh323c-${version}/tests/player", 0777);
mkdir ("ooh323c-${version}/tests/receiver", 0777);
mkdir ("ooh323c-${version}/tests/chansetup", 0777);

#copy example sources
if ($ARGV[0] eq "src") {
    print "Copying examples sources\n";
   #test dir
   print `cp -f ./tests/Makefile.am ./ooh323c-${version}/tests/Makefile.am`;
   print `cp -f ./tests/Makefile.in ./ooh323c-${version}/tests/Makefile.in`;

   #simple
   print "Copying simple sources\n";
   print `cp -f ./tests/simple/*.c ./ooh323c-${version}/tests/simple/`;
   print `cp -f ./tests/simple/*.h ./ooh323c-${version}/tests/simple/`;
   print `cp -f ./tests/simple/Makefile.am ./ooh323c-${version}/tests/simple/Makefile.am`;
   print `cp -f ./tests/simple/Makefile.in ./ooh323c-${version}/tests/simple/Makefile.in`;
   print `cp -f ./tests/simple/simple.dsp ./ooh323c-${version}/tests/simple/simple.dsp`;

   #player
    print "Copying player sources\n";
   print `cp -f ./tests/player/*.c ./ooh323c-${version}/tests/player`;
   print `cp -f ./tests/player/*.h ./ooh323c-${version}/tests/player`;
   print `cp -f ./tests/player/Makefile.am ./ooh323c-${version}/tests/player/Makefile.am`;
   print `cp -f ./tests/player/Makefile.in ./ooh323c-${version}/tests/player/Makefile.in`;
   print `cp -f ./tests/player/ooPlayer.dsp ./ooh323c-${version}/tests/player/ooPlayer.dsp`;
   print `cp -f ./tests/player/space.wav ./ooh323c-${version}/tests/player/.`;
   print `cp -f ./tests/player/states.wav ./ooh323c-${version}/tests/player/.`;
   print `cp -f ./tests/player/space.raw ./ooh323c-${version}/tests/player/space.raw`;

   #receiver
   print "Copying receiver sources\n";
   print `cp -f ./tests/receiver/*.c ./ooh323c-${version}/tests/receiver/`;
   print `cp -f ./tests/receiver/*.h ./ooh323c-${version}/tests/receiver/`;
   print `cp -f ./tests/receiver/Makefile.am ./ooh323c-${version}/tests/receiver/Makefile.am`;
   print `cp -f ./tests/receiver/Makefile.in ./ooh323c-${version}/tests/receiver/Makefile.in`;
   print `cp -f ./tests/receiver/ooReceiver.dsp ./ooh323c-${version}/tests/receiver/ooReceiver.dsp`;

   #h323peer
   print "Copying h323peer sources\n";
   print `cp -f ./tests/chansetup/*.c ./ooh323c-${version}/tests/chansetup`;
   print `cp -f ./tests/chansetup/Makefile.am ./ooh323c-${version}/tests/chansetup/Makefile.am`;
   print `cp -f ./tests/chansetup/Makefile.in ./ooh323c-${version}/tests/chansetup/Makefile.in`;
   print `cp -f ./tests/chansetup/chansetup.dsp ./ooh323c-${version}/tests/chansetup/chansetup.dsp`;

   #media library
   mkdir ("ooh323c-${version}/media", 0777);
   print `cp -r -f ./media/*.c ./ooh323c-${version}/media`;
   print `cp -r -f ./media/*.h ./ooh323c-${version}/media`;
   print `cp -r -f ./media/Makefile.am ./ooh323c-${version}/media/Makefile.am`;
   print `cp -r -f ./media/Makefile.in ./ooh323c-${version}/media/Makefile.in`;
   print `cp -r -f ./media/oomedia.dsp ./ooh323c-${version}/media/oomedia.dsp`;
}

#unix binary build
if ($ARGV[0] eq "unix") {
   print "Copying linux binaries\n";
   mkdir ("ooh323c-${version}/lib", 0777);
   print `cp -f ./src/libooh323c.a ./ooh323c-${version}/lib/.`;
   print `cp -f ./media/.libs/liboomedia.so.1.0.1 ./ooh323c-${version}/lib/liboomedia.so`;
   print `cp -f ./tests/simple/simple ./ooh323c-${version}/tests/simple/.`;
   print `cp -f ./tests/player/ooPlayer ./ooh323c-${version}/tests/player/.`;
   print `cp -f ./tests/player/space.raw ./ooh323c-${version}/tests/player/space.raw`;
   print `cp -f ./tests/receiver/ooReceiver ./ooh323c-${version}/tests/receiver/.`;
   print `cp -f ./tests/chansetup/h323peer ./ooh323c-${version}/tests/chansetup/.`;
}

# windows binary build
if ($ARGV[0] eq "win") {
   print "Copying windows binaries\n";
   mkdir ("ooh323c-${version}/lib", 0777);
   #print `cp -f ./lib/release/ooh323c_a.lib ./ooh323c-${version}/lib/.`;
   #print `cp -f ./lib/release/ooh323c.dll ./ooh323c-${version}/lib/.`;
   print `cp -f ./lib/release/ooh323c.lib ./ooh323c-${version}/lib/.`;
   print `cp -f ./lib/release/oomedia.dll ./ooh323c-${version}/lib/.`;
   print `cp -f ./lib/release/oomedia.lib ./ooh323c-${version}/lib/.`;
   print `cp -f ./tests/simple/release/simple.exe ./ooh323c-${version}/tests/simple/.`;
   print `cp -f ./tests/player/release/ooPlayer.exe ./ooh323c-${version}/tests/player/.`;
   print `cp -f ./tests/player/space.wav ./ooh323c-${version}/tests/player`;
   print `cp -f ./tests/player/states.wav ./ooh323c-${version}/tests/player`;
   print `cp -f ./tests/receiver/release/ooReceiver.exe ./ooh323c-${version}/tests/receiver/.`;
   print `cp -f ./tests/chansetup/release/h323peer.exe ./ooh323c-${version}/tests/chansetup/.`;
}

if($ARGV[0] eq "src") {
   `perl dos2unix.pl  ooh323c-${version} ooh323c-${version}-temp`;
   `rm -rf ooh323c-${version}`;
   `mv ooh323c-${version}-temp ooh323c-${version}`;
 }

if ($ARGV[0] eq "win") {
   print `rm -f ooh323c-${version}.zip`;
   `zip ooh323c-$version.zip -r ooh323c-$version`;
   #`mv ooh323c-$version.zip ooh323c-${version}w32.zip`;
}
if ($ARGV[0] eq "unix") {
   print `rm -f ooh323c-${version}.tar.gz`;
   print `rm -f ooh323c-${version}lnx.tar.gz`;
   `tar -cvf ooh323c-${version}.tar ooh323c-${version}`;
   `gzip ooh323c-${version}.tar`;
   `mv ooh323c-${version}.tar.gz ooh323c-${version}lnx.tar.gz`;
}
if ($ARGV[0] eq "src") {
   print `rm -f ooh323c-${version}.tar.gz`;
   `tar -cvf ooh323c-${version}.tar ooh323c-${version}`;
   `gzip ooh323c-${version}.tar`;
}
#`rm -rf ooh323c`;





