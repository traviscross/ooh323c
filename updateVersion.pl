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
   print "Usage - perl updateVersion.pl <ooh323c-dir> <version>";
   exit(0);
}
($ooh323cdir, $version) = @ARGV;

print "Updating version number in ", $ooh323cdir, " to ", $version, "\n";

updateVersion("${ooh323cdir}/configure.in", $version);
updateVersion("${ooh323cdir}/README", $version);
updateVersion("${ooh323cdir}/README.win32.txt", $version);
updateVersion("${ooh323cdir}/README.unix", $version);
updateVersion("${ooh323cdir}/ChangeLog", $version);
updateVersion("${ooh323cdir}/src/ootypes.h", $version);


sub updateVersion{
   print @_, "\n";
   ($filename, $version) = @_;
   open (INFILE, $filename) || die ("could not open $filename");
   open (OUTFILE, ">temp.txt") || die ("could not open temp.txt");
   while(<INFILE>) {
      s/xx.aa/$version/;
      s/xx.aa/$version/;
      s/xx.aa/$version/;
      s/xx.aa/$version/;
      print OUTFILE $_;
   }
   close (INFILE);
   close (OUTFILE);
   `rm -f $filename`;
   `mv temp.txt $filename`;
}
