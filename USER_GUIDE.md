# User Guide

## Logging and Trace Macro definitions
1. Clone this repository using the following command and  follow steps outlined in [README.md](/RTOSExercise/README.md): 

    ```bash
    git clone https://github.com/Prince-sakariya/RTOSExercise.git

    ```

2. If creating new tasks in separate files, make sure to add as dependencies to CMakeLists.txt

3. Logging can be started using: 
    ```cpp
    Log_Init();
    xLoggingEnabled = 1; // <-- Start logging
    ```

4. To flush logs, first stop them and call the flush function:
    ```cpp
    xLoggingEnabled = 0; // <- Stop logging
    LogFlush();
    ```

## How to launch?
Use monitor.sh script to make sure your program runs properly (no logs stored, only visible in the terminal). 

Use the parse_logs.py file to store logs and visualize the timing diagram. 

In a terminal, navigate to the project directory and execute:
```bash
python3 ./parse_logs.py
```

Open the generated task_schedule.pdf to view the timing diagram. 