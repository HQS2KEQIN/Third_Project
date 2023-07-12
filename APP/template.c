//操作系统主函数模板
//static  OS_TCB   AppTaskStartTCB;
//static  CPU_STK  AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE];

//static  void  AppTaskStart          (void     *p_arg);

//int main(void)
//{
//    OS_ERR  err;

//	
//    CPU_Init();                                                 /* Initialize the uC/CPU Services                       */
//    Mem_Init();                                                 /* Initialize Memory Management Module                  */
//    Math_Init();                                                /* Initialize Mathematical Module                       */

//    OSInit(&err);                                               /* Init uC/OS-III.                                      */

//    OSTaskCreate((OS_TCB       *)&AppTaskStartTCB,              /* Create the start task                                */
//                 (CPU_CHAR     *)"App Task Start",
//                 (OS_TASK_PTR   )AppTaskStart,
//                 (void         *)0u,
//                 (OS_PRIO       )APP_CFG_TASK_START_PRIO,
//                 (CPU_STK      *)&AppTaskStartStk[0u],
//                 (CPU_STK_SIZE  )AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE / 10u],
//                 (CPU_STK_SIZE  )APP_CFG_TASK_START_STK_SIZE,
//                 (OS_MSG_QTY    )0u,
//                 (OS_TICK       )0u,
//                 (void         *)0u,
//                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
//                 (OS_ERR       *)&err);

//    OSStart(&err);                                              /* Start multitasking (i.e. give control to uC/OS-III). */

//    (void)&err;

//    return (0u);
//}


//static  void  AppTaskStart (void *p_arg)
//{
//    OS_ERR  err;


//   (void)p_arg;

//    BSP_Init();                                                 /* Initialize BSP functions                             */
//    BSP_Tick_Init();                                            /* Initialize Tick Services.                            */


//#if OS_CFG_STAT_TASK_EN > 0u
//    OSStatTaskCPUUsageInit(&err);                               /* Compute CPU capacity with no task running            */
//#endif

//#ifdef CPU_CFG_INT_DIS_MEAS_EN
//    CPU_IntDisMeasMaxCurReset();
//#endif


//    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */

//    }
//}
