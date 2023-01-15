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

proc genlist {x} {
  set last $x
  set result ""
  for {set i 0} {$i < $last} {incr i} {
    set result "$result $i"
  }
  return $result 
}

proc gencfile { x } {
  global tFileName
  # open file
  set f [open $tFileName "w"]
  # store strings
  puts $f "/*"
  puts $f "  FileName $tFileName "
  puts $f ""
  puts $f "    task $x "
  puts $f ""
  puts $f "*/"
  # store strings
  puts $f "#define OFF 0"
  puts $f "#define ON  OFF+1"
  puts $f ""
  puts $f "#define NO  0"
  puts $f "#define YES NO+1"
  puts $f ""
  set result [genlist $x]
  set last [llength $result]
  puts $f "#define TSK_ID_MAX $last"
  puts $f ""
  foreach n $result {
    puts $f "#define TSK_ID$n $n"
  }
  puts $f "/* redefine data types"
  puts $f "typedef unsigned char  UBYTE ;"
  puts $f "typedef   signed char  SBYTE ;"
  puts $f "typedef unsigned short UWORD ;"
  puts $f "typedef   signed short SWORD ;"
  puts $f "typedef unsigned long  ULONG ;"
  puts $f ""
  puts $f "/* define TCB */"
  puts $f "typedef struct \{"
  puts $f "  void  (*tsk)(void);"
  puts $f "  UWORD wcount ;"
  puts $f "\} TCBP ;"
  puts $f ""
  puts $f "TCBP tcb\[TSK_ID_MAX\];"
  puts $f ""
  puts $f "/* define TASK state */"
  puts $f "#define TTS_SUSPEND 0"
  puts $f "#define TTS_WAIT    TTS_SUSPEND+1"
  puts $f "#define TTS_READY   TTS_SUSPEND+2"
  puts $f ""
  puts $f "volatile UWORD ready  ;"
  puts $f "volatile UWORD suspend;"
  puts $f "volatile UWORD waitq ;"
  puts $f "volatile UWORD run_tsk;"
  puts $f "volatile UBYTE tflag;"
  puts $f ""
  puts $f "/*-----------------------*/"
  puts $f "/* system call prototype */"
  puts $f "/*-----------------------*/"
  puts $f "void  init_os(void);"
  puts $f "void  cre_tsk(UBYTE tid,void (*tsk)(void));"
  puts $f "void  sta_tsk(UBYTE tid,UBYTE sta);"
  puts $f "void  rsm_tsk(UBYTE tid);"
  puts $f "void  sus_tsk(UBYTE tid);"
  puts $f "void  slp_tsk(void);"
  puts $f "void  wai_tsk(UWORD x);"
  puts $f "UBYTE is_tsk_ready(UBYTE tid);"
  puts $f "void  timer_handler(void);"
  puts $f ""
  puts $f "/*--------------------------------*/"
  puts $f "/* Insert user functions protoype */"
  puts $f "/*--------------------------------*/"
  puts $f "void  user_initialize(void);"
  puts $f ""
  puts $f "/*------*/"
  puts $f "/* main */"
  puts $f "/*------*/"
  puts $f "int main(void)"
  puts $f "\{"
  puts $f "  TCBP  pcur_tsk ;"
  puts $f ""
  puts $f "  user_initialize();"
  puts $f ""
  puts $f "  cli() ;"
  puts $f "  /* initialize monitor */"
  puts $f "  init_os();"
  puts $f ""
  foreach n $result {
    puts $f "  cre_tsk(TSK_ID$n,tsk$n\_proc);"
  }
  puts $f ""
  foreach n $result {
    puts $f "  sta_tsk(TSK_ID$n,TTS_SUSPEND);"
  }
  puts $f ""
  puts $f "  sei() ;"
  puts $f "  /* loop */"
  puts $f "  run_tsk = TSK_ID0 ;"
  puts $f "  while ( ON ) \{"
  puts $f "    /* dispatcher */"
  puts $f "    pcur_tsk = tcb\[run_tsk\] ;"
  puts $f "    if ( is_tsk_ready( run_tsk ) == YES ) \{ (*(pcur_tsk.tsk))(); \}"
  puts $f "    run_tsk++;"
  puts $f "    if ( run_tsk == TSK_ID_MAX ) \{ run_tsk = TSK_ID0 ; \}"
  puts $f "    /* timer */"
  puts $f "    if ( tflag == ON ) \{"
  puts $f "      tflag = OFF ;"
  puts $f "      timer_handler();"
  puts $f "    \}"
  puts $f "  \}"
  puts $f "  /* dummy */"
  puts $f "  return 0 ;"
  puts $f "\}"
  puts $f ""
  puts $f "/*-----------------------*/"
  puts $f "/* Insert user functions */"
  puts $f "/*-----------------------*/"
  puts $f "void user_initialize(void)"
  puts $f "\{"
  puts $f ""
  puts $f "\}"
  puts $f ""
  puts $f "/*----------------*/"
  puts $f "/* task functions */"
  puts $f "/*----------------*/"
  foreach n $result {
    puts $f "void tsk$n\_proc(void)"
    puts $f "\{"
    puts $f ""
    puts $f "\}"
    puts $f ""
  }
  puts $f "/*------------------*/"
  puts $f "/* system call body */"
  puts $f "/*------------------*/"
  puts $f "void init_os(void)"
  puts $f "\{"
  puts $f "  ready   = 0 ;"
  puts $f "  suspend = 0 ;"
  puts $f "  waitq   = 0 ;"
  puts $f "  tflag   = OFF ;"
  puts $f "\}"
  puts $f ""
  puts $f "void cre_tsk(UBYTE tid,void (*tsk)(void))"
  puts $f "\{"
  puts $f "  tcb\[tid\].tsk    = tsk;"
  puts $f "  tcb\[tid\].wcount = 0;"
  puts $f "\}"
  puts $f ""
  puts $f "void sta_tsk(UBYTE tid,UBYTE sta)"
  puts $f "\{"
  puts $f "  UWORD tmp ;"
  puts $f "  tmp = (1 << tid);"
  puts $f "  if ( sta == TTS_READY   ) \{ ready   |= tmp; \}"
  puts $f "  if ( sta == TTS_SUSPEND ) \{ suspend |= tmp; \}"
  puts $f "  if ( sta == TTS_WAIT    ) \{ waitq   |= tmp; \}"
  puts $f "\}"
  puts $f ""
  puts $f "void rsm_tsk(UBYTE tid)"
  puts $f "\{"
  puts $f "  UWORD tmp ;"
  puts $f "  tmp = (1 << tid);"
  puts $f "  ready   |=  tmp;"
  puts $f "  suspend &= ~tmp;"
  puts $f "  waitq   &= ~tmp;"
  puts $f "\}"
  puts $f ""
  puts $f "void sus_tsk(UBYTE tid)"
  puts $f "\{"
  puts $f "  UWORD tmp ;"
  puts $f "  tmp = (1 << tid);"
  puts $f "  ready   &= ~tmp;"
  puts $f "  suspend |=  tmp;"
  puts $f "  waitq   &= ~tmp;"
  puts $f "\}"
  puts $f ""
  puts $f "void slp_tsk(void)"
  puts $f "\{"
  puts $f "  sus_tsk(run_tsk);"
  puts $f "\}"
  puts $f ""
  puts $f "void wai_tsk(UWORD x)"
  puts $f "\{"
  puts $f "  UWORD tmp ;"
  puts $f "  tmp = (1 << run_tsk);"
  puts $f "  ready   &= ~tmp;"
  puts $f "  suspend &= ~tmp;"
  puts $f "  waitq   |=  tmp;"
  puts $f "  tcb\[run_tsk\].wcount = x ;"
  puts $f "\}"
  puts $f ""
  puts $f "UBYTE is_tsk_ready(UBYTE tid)"
  puts $f "\{"
  puts $f "  return( (ready >> tid) & 1 ) ;"
  puts $f "\}"
  puts $f ""
  puts $f "void timer_handler(void)"
  puts $f "\{"
  puts $f "  volatile UWORD xtmp;"
  puts $f "  volatile UBYTE loop;"
  puts $f "  /* call timer handling */"
  puts $f "  xtmp = waitq ;"
  puts $f "  for ( loop = 0 ; loop < TSK_ID_MAX ; loop++ ) \{"
  puts $f "    if ( xtmp & 1 ) \{"
  puts $f "      tcb\[loop\].wcount-- ;"
  puts $f "      if ( tcb\[loop\].wcount == 0 ) \{ rsm_tsk(loop); \}"
  puts $f "    \}"
  puts $f "    xtmp >>= 1 ;"
  puts $f "  \}"
  puts $f "\}"
  # close file
  close $f
}

# define buttons
button .btnEXIT -bg gray -text "EXIT" -command "exit" -width 10

button .btnMAKE -width 10 -bg yellow -text "make" -command {
  # create file name
  set theAcountOfTasks [.sbxSpan get]
  # create file name
  set theFileName [format "%s.c" $tFileName]
  # make
  gencfile $theAcountOfTasks
}

# window area placing
grid .lblAcountOfTasks -column 0 -row 0
grid .sbxSpan          -column 1 -row 0

grid .lblFileName -column 0 -row 2
grid .edtFileName -column 1 -row 2

grid .btnMAKE -column 0 -row 3
grid .btnEXIT -column 1 -row 3

