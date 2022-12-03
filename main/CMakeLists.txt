idf_component_register(
    SRCS "esp_display_main.c" "lcd.c" "lcd.h" "util.c" "util.h" "protocol/d2h.h" "protocol/d2h.c"
         "protocol/h2d.c" "protocol/h2d.h" "protocol/comm.h"
    INCLUDE_DIRS ""
)

target_compile_options(${COMPONENT_LIB} PRIVATE "-Wno-format")
