SET(SOURCES screens/ui_Screen1.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    images/ui_img_ebike_bg_png.c
    images/ui_img_icn_charge_png.c
    images/ui_img_menu_line_png.c
    images/ui_img_slider_battery_png.c
    images/ui_img_icn_flash_png.c
    images/ui_img_speed_slider_png.c
    fonts/ui_font_batteryv.c
    fonts/ui_font_Big.c
    fonts/ui_font_Iltatic_style.c
    fonts/ui_font_Medium.c
    fonts/ui_font_Number.c
    fonts/ui_font_percentagekm.c
    fonts/ui_font_Small.c)


idf_component_register(SRCS ${SOURCES}
                       REQUIRES lvgl-3
                       )