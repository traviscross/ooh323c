if(@ARGV < 2) {
   print "Usage - perl mkfwkit.pl [compact|standard] [version]";
   exit(0);
}
print "Preparing ", $ARGV[0], " kit";
if ($ARGV[0] eq "compact") {
   print " Compact\n";
   $suff = "-compact";
}
print "\n";

$version = $ARGV[1];


print `rm -f ooh323c-${version}.tar.gz`;
print `rm -rf ooh323c-${version}`;
mkdir ("ooh323c-${version}", 0777);
print `cp -f ./AUTHORS ./ooh323c-${version}/AUTHORS`;
print `cp -f ./COPYING ./ooh323c-${version}/COPYING`;
print `cp -f ./ChangeLog ./ooh323c-${version}/ChangeLog`;
print `cp -f ./INSTALL ./ooh323c-${version}/INSTALL`;
print `cp -f ./Makefile.am ./ooh323c-${version}/Makefile.am`;
print `cp -f ./NEWS ./ooh323c-${version}/NEWS`;
print `cp -f ./README ./ooh323c-${version}/README`;

print `cp -f ./configure ./ooh323c-${version}/configure`;
print `cp -f ./configure.in ./ooh323c-${version}/configure.in`;
print `cp -f ./ooh323c.dsw ./ooh323c-${version}/ooh323c.dsw`;

mkdir ("ooh323c-${version}/doc", 0777);
mkdir ("ooh323c-${version}/doc/H323StackLibrary", 0777);
mkdir ("ooh323c-${version}/doc/MediaLibrary", 0777);
print `cp -f -r ./doc/H323Introduction.PDF ./ooh323c-${version}/doc/.`;
print `cp -f -r ./doc/MediaLibRef0.2.PDF ./ooh323c-${version}/doc/.`;
print `cp -f -r ./doc/H323StkUsersGuidev${version}.PDF ./ooh323c-${version}/doc/.`;
print `cp -f -r ./doc/H323StkAPIRefMan.PDF ./ooh323c-${version}/doc/.`;
print `cp -f -r ./doc/H323StackLibrary/* ./ooh323c-${version}/doc/H323StackLibrary`;
print `cp -f -r ./doc/MediaLibrary/* ./ooh323c-${version}/doc/MediaLibrary`;
   
mkdir ("ooh323c-${version}/specs", 0777);
print `cp -f ./specs/*.asn ./ooh323c-${version}/specs`;

  
mkdir ("ooh323c-${version}/src", 0777);
mkdir ("ooh323c-${version}/src/h323", 0777);
#mkdir ("ooh323c-${version}/src/h450", 0777);

print `cp -f ./src/Makefile.am ./ooh323c-${version}/src/Makefile.am`;
print `cp -f ./src/oostk.dsp ./ooh323c-${version}/src/oostk.dsp`;
print `cp -f ./src/*.h ./ooh323c-${version}/src`;
print `cp -f ./src/*.c ./ooh323c-${version}/src`;
print `cp -f ./src/h323${suff}/*.h ./ooh323c-${version}/src/h323`;
print `cp -f ./src/h323${suff}/*.c ./ooh323c-${version}/src/h323`;

#print `cp -f ./src/h450${suff}/* ./ooh323c-${version}/src/h450` ;






mkdir ("ooh323c-${version}/tests", 0777);
print `cp -f ./tests/Makefile.am ./ooh323c-${version}/tests/Makefile.am`;

mkdir ("ooh323c-${version}/tests/simple", 0777);
print `cp -f ./tests/simple/*.c ./ooh323c-${version}/tests/simple`;
print `cp -f ./tests/simple/*.h ./ooh323c-${version}/tests/simple`;
print `cp -f ./tests/simple/Makefile.am ./ooh323c-${version}/tests/simple/Makefile.am`;
print `cp -f ./tests/simple/simple.dsp ./ooh323c-${version}/tests/simple/simple.dsp`;
  

mkdir ("ooh323c-${version}/tests/player", 0777);
print `cp -f ./tests/player/*.c ./ooh323c-${version}/tests/player`;
print `cp -f ./tests/player/*.h ./ooh323c-${version}/tests/player`;
print `cp -f ./tests/player/Makefile.am ./ooh323c-${version}/tests/player/Makefile.am`;
print `cp -f ./tests/player/space.raw ./ooh323c-${version}/tests/player/space.raw`;
print `cp -f ./tests/player/ooPlayer.dsp ./ooh323c-${version}/tests/player/ooPlayer.dsp`;
print `cp -f ./tests/player/space.wav ./ooh323c-${version}/tests/player`;
print `cp -f ./tests/player/states.wav ./ooh323c-${version}/tests/player`;

mkdir ("ooh323c-${version}/tests/receiver", 0777);
print `cp -f ./tests/receiver/*.c ./ooh323c-${version}/tests/receiver`;
print `cp -f ./tests/receiver/*.h ./ooh323c-${version}/tests/receiver`;
print `cp -f ./tests/receiver/Makefile.am ./ooh323c-${version}/tests/receiver/Makefile.am`;
print `cp -f ./tests/receiver/ooReceiver.dsp ./ooh323c-${version}/tests/receiver/ooReceiver.dsp`;
  

mkdir ("ooh323c-${version}/media", 0777);
print `cp -r -f ./media/*.c ./ooh323c-${version}/media`;
print `cp -r -f ./media/*.h ./ooh323c-${version}/media`;
print `cp -r -f ./media/Makefile.am ./ooh323c-${version}/media/Makefile.am`;
print `cp -r -f ./media/oomedia.dsp ./ooh323c-${version}/media/oomedia.dsp`;


 
`tar -cvf ooh323c-${version}.tar ooh323c-${version}`;
`gzip ooh323c-${version}.tar`;
#`rm -rf ooh323c`;


