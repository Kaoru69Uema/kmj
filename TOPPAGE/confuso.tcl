#!/usr/bin/wish

. configure -width 250 -height 250

wm title . "Configure source code with USO"

# initialize file name
set theFileName "unknown"
set tFileName   $theFileName

# initialize acount of tasks
set theAcountOfTasks 7

# define labels
label   .lblAcountOfTasks -width 16 -text "Acount of tasks"
spinbox .sbxSpan          -width 10 -state readonly -from 2 -to 16 -increment 1
label   .lblAcount        -width 10 -text "7"               -bg lightblue
label   .lblFileName      -width 10 -text "File Name"
entry   .edtFileName      -width 10 -bg green -textvariable tFileName

# sub procedure
proc updateAcount { s } {
  global theAcountOfTasks
  # get acount
  set theAcountOfTasks $s
  # puts digit to label
  .lblAcount configure -text "$theAcountOfTasks"
}

# define buttons
button .btnEXIT -bg gray -text "EXIT" -command "exit" -width 10

button .btnMAKE -width 10 -bg yellow -text "make" -command {
  # create file name
  set theAcountOfTasks [.sbxSpan get]
  # create file name
  set theFileName [format "%s.c" $tFileName]
  # make
  exec xconfig $theAcountOfTasks $theFileName
}

# window area placing
grid .lblAcountOfTasks -column 0 -row 0
grid .sbxSpan          -column 1 -row 0

grid .lblFileName -column 0 -row 2
grid .edtFileName -column 1 -row 2

grid .btnMAKE -column 0 -row 3
grid .btnEXIT -column 1 -row 3

