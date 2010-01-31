open (INFILE, "ooq931.c");
while (<INFILE>) {
    chomp;
    print "$_\n";
}
