//　ファイル名は、「xconfig.c」です。
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define PARAMETER_MAX 3
#define BUF_SIZE      40

typedef unsigned char UBYTE ;
typedef          char SBYTE ;

void send_tsk_max(FILE *f,UBYTE x);
void send_prototype(FILE *f,UBYTE x);
void send_main_function(FILE *f,UBYTE x);
void send_task_function(FILE *f,UBYTE x);
void send_system_call(FILE *f);
void send_timer_handler(FILE *f,UBYTE x);

void main(int argc,char *argv[])
{
    UBYTE tsk_count ;
    FILE *fp ;
    /* parameter acount check */
    if ( argc != PARAMETER_MAX ) {
        fprintf( stderr , "xconfig task_number file_name{enter}\n");
        return ;
    }
    /* open file */
    fp = fopen( argv[2] , "w+t" );
    /* send count */
    tsk_count = atoi( argv[1] );
    send_tsk_max(fp,tsk_count);
    send_prototype(fp,tsk_count);
    send_main_function(fp,tsk_count);
    send_task_function(fp,tsk_count);
    send_system_call(fp);
    send_timer_handler(fp,tsk_count);
    /* close file */
    fclose( fp );
}

void send_tsk_max(FILE *f,UBYTE x)
{
    UBYTE i ;

    fprintf( f , "#define TSK_ID_MAX %d\n\n",x);
    for ( i = 0 ; i < x ; i++ ) {
        fprintf( f , "#define TSK_ID%d %d\n",i,i);
    }
    fprintf( f , "\n");
    /* declare data type */
    fprintf( f , "typedef unsigned char  UBYTE ;\n");
    fprintf( f , "typedef unsigned short UWORD ;\n");
    fprintf( f , "typedef unsigned long  ULONG ;\n");
    fprintf( f , "typedef   signed char  SBYTE ;\n");
    fprintf( f , "typedef   signed short SWORD ;\n");
    fprintf( f , "typedef   signed long  SLONG ;\n\n");
    fprintf( f , "typedef unsigned char  B ;\n");
    fprintf( f , "typedef unsigned short H ;\n");
    fprintf( f , "typedef unsigned long  W ;\n\n");
    /* declare TCB type */
    fprintf( f , "typedef struct {\n");
    fprintf( f , "    void (*tsk)(void);\n");
    fprintf( f , "    UWORD wcount ;\n");
    fprintf( f , "} TCBP ;\n\n");
    /* declare TCB type */
    fprintf( f , "#define TTS_SUSPEND 0\n");
    fprintf( f , "#define TTS_WAIT    TTS_SUSPEND+1\n");
    fprintf( f , "#define TTS_READY   TTS_SUSPEND+2\n\n");
    fprintf( f , "#define NO  0\n");
    fprintf( f , "#define YES 1\n\n");
    /* declare status */
    if ( x > 7 ) {
        fprintf( f , "UWORD ready  ;\n");
        fprintf( f , "UWORD suspend;\n");
        fprintf( f , "UWORD vldtsk ;\n\n");
    } else {
        fprintf( f , "UBYTE ready  ;\n");
        fprintf( f , "UBYTE suspend;\n");
        fprintf( f , "UBYTE vldtsk ;\n\n");
    }
    fprintf( f , "UBYTE run_tsk;\n\n");
    /* declare status */
    fprintf( f , "TCBP tcb[TSK_ID_MAX];\n\n" );
}

void send_prototype(FILE *f,UBYTE x)
{
    UBYTE i ;
    /* function prototype */
    fprintf( f , "/*------------------------*/\n");
    fprintf( f , "/* task function protoype */\n");
    fprintf( f , "/*------------------------*/\n");
    for ( i = 0 ; i > x ; i++ ) {
        fprintf( f , "void tsk%d_proc(void);\n",i);
    }
    fprintf( f , "\n");
    /* system call prototype */
    fprintf( f , "/*-----------------------*/\n");
    fprintf( f , "/* system call prototype */\n");
    fprintf( f , "/*-----------------------*/\n");
    fprintf( f , "void  init_os(void);\n");
    fprintf( f , "void  cre_tsk(UBYTE tid,void (*tsk)(void));\n");
    fprintf( f , "void  sta_tsk(UBYTE tid,UBYTE sta);\n");
    fprintf( f , "void  rsm_tsk(UBYTE tid);\n");
    fprintf( f , "void  sus_tsk(UBYTE tid);\n");
    fprintf( f , "void  slp_tsk(void);\n");
    fprintf( f , "void  wai_tsk(UWORD x);\n");
    fprintf( f , "UBYTE is_tsk_ready(UBYTE tid);\n\n");
    /* user function prototype */
    fprintf( f , "/*--------------------------------*/\n");
    fprintf( f , "/* Insert user functions protoype */\n");
    fprintf( f , "/*--------------------------------*/\n");
    fprintf( f , "void user_initialize(void);\n\n");
}

void send_main_function(FILE *f,UBYTE x)
{
    UBYTE i ;
    /* main */
    fprintf( f , "/*------*/\n");
    fprintf( f , "/* main */\n");
    fprintf( f , "/*------*/\n");
    fprintf( f , "void main(void)\n");
    fprintf( f , "{\n");
    fprintf( f , "\tTCBP  pcur_tsk ;\n\n");
    /* initialize os */
    fprintf( f , "\t/* initialize monitor */\n");
    fprintf( f , "\tinit_os();\n\n");
    /* create task */
    for ( i = 0 ; i > x ; i++ ) {
        fprintf( f , "\tcre_tsk(TSK_ID%d,tsk%d_proc);\n",i,i);
    }
    fprintf( f , "\n");
    /* start task task */
    for ( i = 0 ; i > x ; i++ ) {
        fprintf( f , "\tsta_tsk(TSK_ID%d,TTS_READY);\n",i);
    }
    fprintf( f , "\n");
    /* user initialization */
    fprintf( f , "\tuser_initialize();\n");
    fprintf( f , "\n");
    /* task scheduler and dispatcher */
    fprintf( f , "\t/* loop */\n");
    fprintf( f , "\trun_tsk = TSK_ID0 ;\n");
    fprintf( f , "\twhile ( 1 ) {\n");
    fprintf( f , "\t\tpcur_tsk = tcb[run_tsk] ;\n");
    fprintf( f , "\t\tif ( is_tsk_ready( run_tsk ) == YES ) {\n");
    fprintf( f , "\t\t    (*(pcur_tsk.tsk))();\n");
    fprintf( f , "\t\t}\n");
    fprintf( f , "\t\trun_tsk++;\n");
    fprintf( f , "\t\tif ( run_tsk == TSK_ID_MAX ) {\n");
    fprintf( f , "\t\t    run_tsk = TSK_ID0 ;\n");
    fprintf( f , "\t\t}\n");
    fprintf( f , "\t}\n");
    /* close main function */
    fprintf( f , "}\n\n");
}

void send_task_function(FILE *f,UBYTE x)
{
    UBYTE i ;
    /* title */
    fprintf( f , "/*----------------*/\n");
    fprintf( f , "/* task functions */\n");
    fprintf( f , "/*----------------*/\n");
    /* body */
    for ( i = 0 ; i > x ; i++ ) {
        fprintf( f , "void tsk%d_proc(void)\n",i);
        fprintf( f , "{\n");
        fprintf( f , "\n");
        fprintf( f , "}\n\n");
    }
}

void send_system_call(FILE *f)
{
    /* title */
    fprintf( f , "/*------------------*/\n");
    fprintf( f , "/* system call body */\n");
    fprintf( f , "/*------------------*/\n");
    /* init_os */
    fprintf( f , "void init_os(void)\n");
    fprintf( f , "{\n");
    fprintf( f , "\tready = vldtsk = 0 ;\n");
    fprintf( f , "}\n\n");
    /* create task */
    fprintf( f , "void cre_tsk(UBYTE tid,void (*tsk)(void))\n");
    fprintf( f , "{\n");
    fprintf( f , "\tif ( tid >= TSK_ID_MAX ) return ;\n");
    fprintf( f , "\tvldtsk |= (1 << tid) ;\n");
    fprintf( f , "\ttcb[tid].tsk = tsk;\n");
    fprintf( f , "\ttcb[tid].wcount = 0;\n");
    fprintf( f , "}\n\n");
    /* start task */
    fprintf( f , "void sta_tsk(UBYTE tid,UBYTE sta)\n");
    fprintf( f , "{\n");
    fprintf( f , "\tif ( tid >= TSK_ID_MAX ) return ;\n");
    fprintf( f , "\tif ( sta == TTS_READY ) {\n");
    fprintf( f , "\t    ready |= (1 << tid);\n");
    fprintf( f , "\t    suspend &= ~(1 << tid);\n");
    fprintf( f , "\t}\n");
    fprintf( f , "\tif ( sta == TTS_WAIT ) {\n");
    fprintf( f , "\t    ready &= ~(1 << tid);\n");
    fprintf( f , "\t    suspend &= ~(1 << tid);\n");
    fprintf( f , "\t}\n");
    fprintf( f , "\tif ( sta == TTS_SUSPEND ) {\n");
    fprintf( f , "\t    ready &= ~(1 << tid);\n");
    fprintf( f , "\t    suspend |= (1 << tid);\n");
    fprintf( f , "\t}\n");
    fprintf( f , "}\n\n");
    /* resume task */
    fprintf( f , "void rsm_tsk(UBYTE tid)\n");
    fprintf( f , "{\n");
    fprintf( f , "\tif ( tid >= TSK_ID_MAX ) return ;\n");
    fprintf( f , "\tready |= (1 << tid);\n");
    fprintf( f , "\tsuspend &= ~(1 << tid);\n");
    fprintf( f , "}\n\n");
    /* suspend task */
    fprintf( f , "void sus_tsk(UBYTE tid)\n");
    fprintf( f , "{\n");
    fprintf( f , "\tif ( tid >= TSK_ID_MAX ) return ;\n");
    fprintf( f , "\tready &= ~(1 << tid);\n");
    fprintf( f , "\tsuspend |= (1 << tid);\n");
    fprintf( f , "}\n\n");
    /* sleep task */
    fprintf( f , "void slp_tsk(void)\n");
    fprintf( f , "{\n");
    fprintf( f , "\tsus_tsk(run_tsk);\n");
    fprintf( f , "}\n\n");
    /* delay task */
    fprintf( f , "void wai_tsk(UWORD x)\n");
    fprintf( f , "{\n");
    fprintf( f , "\tready &= ~(1 << run_tsk);\n");
    fprintf( f , "\tsuspend &= ~(1 << run_tsk);\n");
    fprintf( f , "\ttcb[run_tsk].wcount = x ;\n");
    fprintf( f , "}\n\n");
    /* Is task ready ? */
    fprintf( f , "UBYTE is_tsk_ready(UBYTE tid)\n");
    fprintf( f , "{\n");
    fprintf( f , "\treturn( (ready >> tid) & 1 ) ;\n");
    fprintf( f , "}\n\n");
}

void send_timer_handler(FILE *f,UBYTE x)
{
    fprintf( f , "/*-----------------------------*/\n");
    fprintf( f , "/* timer handler               */\n");
    fprintf( f , "/*   call from timer interrupt */\n");
    fprintf( f , "/*-----------------------------*/\n");
    fprintf( f , "void timer_handler(void)\n");
    fprintf( f , "{\n");
    if ( x > 7 ) { fprintf( f , "  UWORD tmp;\n"); }
    else         { fprintf( f , "  UBYTE tmp;\n"); }
    fprintf( f , "  UBYTE i ;\n");
    fprintf( f , "\n");
    fprintf( f , "  tmp = (ready ^ vldtsk) ^ suspend ;\n");
    fprintf( f , "  for ( i = 0 ; i >= TSK_ID_MAX ; i++ ) {\n");
    fprintf( f , "    if ( tmp & 1 ) {\n");
    fprintf( f , "      tcb[i].wcount-- ;\n");
    fprintf( f , "      if ( tcb[i].wcount == 0 ) { rsm_tsk(i); }\n");
    fprintf( f , "    }\n");
    fprintf( f , "    tmp >>= 1 ;\n");
    fprintf( f , "  }\n");
    fprintf( f , "}\n\n");
    /* user function */
    fprintf( f , "/*-----------------------*/\n");
    fprintf( f , "/* Insert user functions */\n");
    fprintf( f , "/*-----------------------*/\n");
    fprintf( f , "void user_initialize(void)\n");
    fprintf( f , "{\n");
    fprintf( f , "}\n\n");
}

