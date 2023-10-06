idf_component_register(SRCS "WZ8048C050.cpp"
                    INCLUDE_DIRS "."
                    "../components/arduino-esp32-master"                 
                    "../components/lvgl-3"
                    "../components/UI"
                    "../components/gt911-arduino-main" 
                    "../components/Arduino_GFX-master"                  
                    )
                    
target_compile_definitions(${COMPONENT_LIB} PUBLIC "-DLOG_LOCAL_LEVEL=ESP_LOG_VERBOSE")
