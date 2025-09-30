idf_component_register(
    SRCS 
        "main.cpp"

    INCLUDE_DIRS 
        "controllers"
        "actuators"
        "sensors"
        "utils"
        "."
    
    REQUIRES
        nvs_flash
        freertos
        driver
)
