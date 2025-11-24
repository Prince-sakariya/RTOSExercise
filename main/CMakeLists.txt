idf_component_register(
    SRCS 
        "main.cpp"
        "tasks/stopwatch.cpp"
        "display.cpp"
        "tasks/exercise2.cpp"

    PRIV_REQUIRES 
        spi_flash 
        Watchy
    INCLUDE_DIRS 
        "." 
        "tasks"
)