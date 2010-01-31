open (INFILE, "temp.txt");
while (<INFILE>) {
    chomp;
    s/\r//;
    s/\n//;
    print "$_\n";
}
