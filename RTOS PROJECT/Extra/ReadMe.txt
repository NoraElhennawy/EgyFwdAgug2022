Changes Made to Implement extra Part:
-in RTos_config: define Trace   ,define
-in Tasks.c: check if edf_schedueler then use xreadyedfList instead of readyList
-in main.c: decalre TASKState[] array to hold task states 
            Modify in Trans_Task to call vTaskList() and send TaskState on Queue. 