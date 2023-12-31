// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.2
// LVGL version: 8.3.3
// Project name: battery

#ifndef _BATTERY_UI_H
#define _BATTERY_UI_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "lvgl.h"

#include "ui_helpers.h"
#include "ui_events.h"
    // SCREEN: ui_Screen1
    void ui_Screen1_screen_init(void);
    extern lv_obj_t *ui_Screen1;
    extern lv_obj_t *ui_Button1;
    extern lv_obj_t *ui_Label5;
    extern lv_obj_t *ui_Speed_Number_2;
    extern lv_obj_t *ui_Speed_Number_1;
    extern lv_obj_t *ui_speedd;
    extern lv_obj_t *ui_Label8;
    extern lv_obj_t *ui_Battery;
    extern lv_obj_t *ui_Batterypanel;
    extern lv_obj_t *ui_Bar2;
    extern lv_obj_t *ui_Image5;
    extern lv_obj_t *ui_Batterypanel1;
    extern lv_obj_t *ui_Bar3;
    extern lv_obj_t *ui_Image2;
    extern lv_obj_t *ui_Label3;
    extern lv_obj_t *ui_speedd1;
    extern lv_obj_t *ui_speedd2;
    extern lv_obj_t *ui_Label4;
    extern lv_obj_t *ui_speedd3;
    extern lv_obj_t *ui_speedd4;
    extern lv_obj_t *ui_Label9;
    extern lv_obj_t *ui_Label11;
    extern lv_obj_t *ui_Label10;
    extern lv_obj_t *ui_Label12;
    extern lv_obj_t *ui_Title;
    extern lv_obj_t *ui_Image1;
    extern lv_obj_t *ui_Label17;
    extern lv_obj_t *ui_batterypercentagevolumne;
    extern lv_obj_t *ui_Label13;
    extern lv_obj_t *ui_Label7;
    extern lv_obj_t *ui_batterypercentagevolumne1;
    extern lv_obj_t *ui_Label15;
    extern lv_obj_t *ui_Label16;
    extern lv_obj_t *ui_regeneratedpanel;
    extern lv_obj_t *ui_Label19;
    extern lv_obj_t *ui_Label20;
    extern lv_obj_t *ui_Label21;
    extern lv_obj_t *ui_Label22;
    extern lv_obj_t *ui_Label23;
    extern lv_obj_t *ui_Label14;
    extern lv_obj_t *ui_Slider1;
    extern lv_obj_t *ui_flash;
    extern lv_obj_t *ui_Slider2;
    extern lv_obj_t *ui_Panel3;
    extern lv_obj_t *ui_Label18;
    extern lv_obj_t *ui_Label1;
    extern lv_obj_t *ui_footer;
    extern lv_obj_t *ui_Bar1;
    extern lv_obj_t *ui_Label2;
    extern lv_obj_t *ui_Label6;
    extern lv_obj_t *ui____initial_actions0;

    LV_IMG_DECLARE(ui_img_ebike_bg_png);       // assets\ebike_bg.png
    LV_IMG_DECLARE(ui_img_icn_charge_png);     // assets\icn_charge.png
    LV_IMG_DECLARE(ui_img_menu_line_png);      // assets\menu_line.png
    LV_IMG_DECLARE(ui_img_slider_battery_png); // assets\slider_battery.png
    LV_IMG_DECLARE(ui_img_icn_flash_png);      // assets\icn_flash.png
    LV_IMG_DECLARE(ui_img_speed_slider_png);   // assets\speed_slider.png

    LV_FONT_DECLARE(ui_font_batteryv);
    LV_FONT_DECLARE(ui_font_Big);
    LV_FONT_DECLARE(ui_font_Iltatic_style);
    LV_FONT_DECLARE(ui_font_Medium);
    LV_FONT_DECLARE(ui_font_Number);
    LV_FONT_DECLARE(ui_font_percentagekm);
    LV_FONT_DECLARE(ui_font_Small);

    void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
