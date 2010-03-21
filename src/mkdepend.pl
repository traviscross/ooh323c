# generate dependency rules file (rules.mk)

$g_debug = 0;

$numargs = @ARGV;

if (-e "rules.mk") {
    `rm -f rules.mk~`;
    `mv rules.mk rules.mk~`;
}

open (OUTFILE, ">rules.mk") ||
    die ("could not open rules.mk for output");

print OUTFILE "# C/C++ to object file compilation rules\n";

&procObjectsFile ("../src");
&procObjectsFile ("../src/h323_v6");
&procObjectsFile ("../src/h323");

close (OUTFILE);

# processobjects.mk file

sub procObjectsFile {
    local $srcdir = shift;
    local $objects_fname = "$srcdir/objects.mk";
    open (INFILE, $objects_fname) || die ("could not open $objects_fname: $!");

    while (<INFILE>) {
        if (/([\w\-]+)\$\(OBJ\)/) {
            $filebase = $1;
            # print "$objdir $filebase\n";

            # determine source file
            if ( -e "$srcdir/$filebase.c" ) {
                &addRules ("$srcdir/$filebase.c");
            }
            elsif ( -e "$srcdir/$filebase.cpp" ) {
                &addRules ("$srcdir/$filebase.cpp");
            }
            else {
                print "source file for $filebase not found\n";
            }
        }
    }

    close (INFILE);
}

sub addRules {
    local $sourcefile = shift;
    local $cfilespec;
    local $cppfile = ($sourcefile =~ /\.cpp$/);
    local @rules = (`gcc $sourcefile -E -MM -I../src -I../src/h323_v6 -I../src/h323`);

    foreach $line (@rules) {
        # if line starts with a non-whitespace character, it is the
        # start of a new rule ..
        if ($line =~ /^\w/) {
            # change .o to $(OBJ)
            $line =~ s/\.o/\$(OBJ)/;

            # add a newline
            print OUTFILE "\n";

            # get C source file from rule
            $cfilespec = $sourcefile;
        }

        print OUTFILE $line;
    }

    # add compile command
    if ($cfilespec) {
        if ($cppfile) {
            print OUTFILE ("\t\$(CCC) ");
        }
        else {
            print OUTFILE ("\t\$(CC) ");
        }
        print OUTFILE ("-c \$(CFLAGS) \$(IPATHS) $cfilespec\n");
        print OUTFILE ("\n");
    }
}
