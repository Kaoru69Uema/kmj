#!/usr/bin/gawk

/^$/           { print $0 }
/^include/     { print ".include " $2 }
/^org/         { print ".org " $2}
/.*:/          { print $1 }
/^add/         { print "\t" $1 " " $2 ", " $3 }
