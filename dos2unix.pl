#!/usr/local/bin/perl
# Perl script to traverse a directory tree and translate all Windows
# source files to UNIX format..

# Arguments are: <source> <target>
#   <source>  Root specification of the source directory tree
#   <target>  Root of the target directory where output is to be written
#             (note: this can not exist prior to executing the script).
#
###########################################################################

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

$g_debug = 1;

# custom DOS to UNIX translation program can be defined here
$dos2unix = $ENV{'DOS2UNIX'};
&pdiag ("dos2unix is $dos2unix\n");

if (@ARGV != 2) {
    print "usage: mkunixsrc <source> <target>\n";
    print "  <source>  Root specification of the source directory tree.\n";
    print "  <target>  Root of the target directory where output is to be \n";
    print "            written.  (note: this can not exist prior to \n";
    print "            executing the script).\n";
    exit 1;
}

# make sure <source> exists and <target> does not

$source = $ARGV[0];
$target = $ARGV[1];

&pdiag ("source is $source, target is $target\n");

if (!(-e $source)) {
    print "$source does not exist.\n";
    exit 1;
}

# if source is a file, convert it and write it directly to target;
# otherwise, do a recursive conversion..

if (!(-d $source)) {
    if (defined ($dos2unix)) {
        print "$dos2unix $source\n";
        print `$dos2unix $source`;
    }
    else {
        &cvtDosFileToUnixFile ($source, $target);
    }
    exit 0;
}

if (-e $target) {
    print "$target exists (delete and start over).\n";
    exit 1;
}

mkdir ($target, 0777);

# call recursive subroutine to traverse all dir paths and translate files

&translate_files ($source, $target, "");

###########################################################################

# translate_files
# traverse tree and translate DOS text files to UNIX
# args:
#   $source   source directory root
#   $target   target directory root
#   $relpath  relative path within directory tree structure

sub translate_files {
    local ($source_root, $target_root, $relpath) = @_;
    local ($source_path) = length ($relpath) > 0 ?
        $source_root . "/" . $relpath : $source_root;
    local ($filename, $filepath, $new_relpath, $outpath);

    &pdiag ("translate_files:\n");
    &pdiag ("  source_root is $source_root\n");
    &pdiag ("  target_root is $target_root\n");
    &pdiag ("  relpath is $relpath\n");

    &pdiag ("  source_path is $source_path\n");
    local (@filelist) = `ls $source_path`;

    # loop through all files in the source directory

    foreach $filename (@filelist) {
        $filename =~ s/\n$//;
        &pdiag ("  filename is $filename\n");

        $filepath = $source_path . "/" . $filename;
        &pdiag ("  filepath is $filepath\n");

        # if file is a directory, make recursive call
        if (-d $filepath) {
            &pdiag ("  $filepath is a directory\n");
            # first, create target directory
            $new_relpath = length ($relpath) > 0 ?
                $relpath . "/" . $filename : $filename;
            mkdir ($target_root . "/" . $new_relpath, 0777);
            # now make recursive call
            &translate_files ($source_root, $target_root, $new_relpath);
        }
        # note: if binary file, ext must be specified here or file
        # will not be copied to target directory..
        elsif ($filename =~ /\.pdf$/ ||
               $filename =~ /\.bin$/ ||
               $filename =~ /\.bir$/ ||
               $filename =~ /\.dat$/ ||
               $filename =~ /\.jar$/ ||
               $filename =~ /config/ ||
               $filename =~ /\.sh$/  ||
               $filename =~ /libtool/ ||
               $filename =~ /mkinstalldirs/ ||
               $filename =~ /\.PDF$/ ||
               $filename =~ /\.wav$/ ||
               $filename =~ /\.dsw$/ ||
               $filename =~ /\.dsp$/ ||
               $filename =~ /\.raw$/ ||
               $filename =~ /\.a$/      
               ) {
            &pdiag ("  $filepath is a binary file, copying..\n");
            # copy it
            $outpath = $target_root . "/" . $relpath . "/" . $filename;
            `cp $filepath $outpath`;
        }
        elsif (-T $filepath) {
            &pdiag ("  $filepath is a text file, translating..\n");
            # translate it
            $outpath = $target_root . "/" . $relpath . "/" . $filename;
            if (defined ($dos2unix)) {
                `cp $filepath $outpath`;
                if ($g_debug) {
                    print "  $dos2unix $outpath\n";
                }
                `$dos2unix $outpath`;
            }
            else {
                &cvtDosFileToUnixFile ($filepath, $outpath);
            }
        }
    }
}

sub cvtDosFileToUnixFile {
    local ($sourceFile, $targetFile) = @_;
    open (INFILE, $sourceFile) || die ("could not open $sourceFile");
    open (OUTFILE, ">$targetFile")|| die ("could not open $targetFile");
    while (<INFILE>) {
        s/\r\n/\n/;
        print OUTFILE $_;
    }
    close (INFILE);
    close (OUTFILE);
}

sub pdiag {
    if ($g_debug) {
        print @_;
    }
}
