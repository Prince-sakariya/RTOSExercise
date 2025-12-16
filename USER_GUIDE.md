# User Guide

## Logging and Trace Macro definitions
1. Clone this repository using the following command and  follow steps outlined in [README.md](/README.md): 

    ```bash
    git clone https://github.com/Prince-sakariya/RTOSExercise.git

    ```

2. If creating new tasks in separate files, make sure to add as dependencies to CMakeLists.txt

3. Logging can be started using: 
    ```cpp
    Log_Init();
    xLoggingEnabled = 1; // <-- Start logging
    ```
    For example see line 23 in [main.cpp](/main/main.cpp)

4. To flush logs, first stop them and call the flush function:
    ```cpp
    xLoggingEnabled = 0; // <- Stop logging
    LogFlush();
    ```
    For example see line 65 in [exercise.cpp](/main/tasks/exercise2.cpp)

## How to launch?
Use monitor.sh script to make sure your program runs properly (no logs stored, only visible in the terminal). 

If you have cloned this repository and not made any changes to the source code, use the parse_logs.py file to store logs and visualize the timing diagram. It represents exercise 2 from moodle with 3 production lines, 1 printer and a shared queue.  

In a terminal, navigate to the project directory and execute:
```bash
python3 ./parse_logs.py
```

Open the generated task_schedule_basic.pdf and task_schedule_extended to view the timing diagram. 
