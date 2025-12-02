idf_component_register(
    SRCS 
        "main.cpp"
        "tasks/stopwatch.cpp"
        "display.cpp"
        "tasks/exercise2.cpp"
        "trace-hooks/trace-queue.c"

    PRIV_REQUIRES 
        spi_flash 
        Watchy
    INCLUDE_DIRS 
        "." 
        "tasks"
        "trace-hooks/include"
)