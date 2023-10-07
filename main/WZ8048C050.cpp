#include <lvgl.h>
#include <ArduinoJson.h>

#include "ui.h"

#include <SPI.h>

SPIClass &spi = SPI;
uint16_t touchCalibration_x0 = 300, touchCalibration_x1 = 3600, touchCalibration_y0 = 300, touchCalibration_y1 = 3600;
uint8_t touchCalibration_rotate = 1, touchCalibration_invert_x = 2, touchCalibration_invert_y = 0;

static uint32_t screenWidth;
static uint32_t screenHeight;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t disp_draw_buf[800 * 480 / 10];
// static lv_color_t disp_draw_buf;
static lv_disp_drv_t disp_drv;
static void json_reader(void *pvParameter);
SemaphoreHandle_t xGuiSemaphore;

void ui_reset(void);

// struct SpiRamAllocator
// {
//     void *allocate(size_t size)
//     {
//         return heap_caps_malloc(size, MALLOC_CAP_SPIRAM);
//     }

//     void deallocate(void *pointer)
//     {
//         heap_caps_free(pointer);
//     }

//     void *reallocate(void *ptr, size_t new_size)
//     {
//         return heap_caps_realloc(ptr, new_size, MALLOC_CAP_SPIRAM);
//     }
// };

#include <Arduino_GFX_Library.h>
#define TFT_BL 2
#define GFX_BL DF_GFX_BL // default backlight pin, you may replace DF_GFX_BL to actual backlight pin

#if defined(DISPLAY_DEV_KIT)
Arduino_GFX *lcd = create_default_Arduino_GFX();
#else  /* !defined(DISPLAY_DEV_KIT) */

Arduino_ESP32RGBPanel *bus = new Arduino_ESP32RGBPanel(
    GFX_NOT_DEFINED /* CS */, GFX_NOT_DEFINED /* SCK */, GFX_NOT_DEFINED /* SDA */,
    40 /* DE */, 41 /* VSYNC */, 39 /* HSYNC */, 0 /* PCLK */,
    45 /* R0 */, 48 /* R1 */, 47 /* R2 */, 21 /* R3 */, 14 /* R4 */,
    5 /* G0 */, 6 /* G1 */, 7 /* G2 */, 15 /* G3 */, 16 /* G4 */, 4 /* G5 */,
    8 /* B0 */, 3 /* B1 */, 46 /* B2 */, 9 /* B3 */, 1 /* B4 */
);

// option 1:
Arduino_RPi_DPI_RGBPanel *lcd = new Arduino_RPi_DPI_RGBPanel(
    bus,
    800 /* width */, 0 /* hsync_polarity */, 8 /* hsync_front_porch */, 4 /* hsync_pulse_width */, 43 /* hsync_back_porch */,
    480 /* height */, 0 /* vsync_polarity */, 8 /* vsync_front_porch */, 4 /* vsync_pulse_width */, 12 /* vsync_back_porch */,
    1 /* pclk_active_neg */, 16000000 /* prefer_speed */, true /* auto_flush */);
#endif /* !defined(DISPLAY_DEV_KIT) */

#include "touch.h"
/* Display flushing */

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

#if (LV_COLOR_16_SWAP != 0)
    lcd->draw16bitBeRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#else
    lcd->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#endif

    lv_disp_flush_ready(disp);
}

void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
    if (touch_has_signal())
    {
        if (touch_touched())
        {
            data->state = LV_INDEV_STATE_PR;

            /*Set the coordinates*/
            data->point.x = touch_last_x;
            data->point.y = touch_last_y;
            // Serial.print("Data x :");
            // Serial.println(touch_last_x);

            // Serial.print("Data y :");
            // Serial.println(touch_last_y);
        }
        else if (touch_released())
        {
            data->state = LV_INDEV_STATE_REL;
        }
    }
    else
    {
        data->state = LV_INDEV_STATE_REL;
    }
}

#define Z_THRESHOLD 350 // Touch pressure threshold for validating touches
#define _RAWERR 20      // Deadband error allowed in successive position samples

extern "C" void app_main()
{
    // put your setup code here, to run once:
    xGuiSemaphore = xSemaphoreCreateMutex();

    Serial.begin(9600); // Init Display
    lcd->begin();
    lcd->fillScreen(BLACK);
    lcd->setTextSize(2);

    delay(500);

#ifdef TFT_BL
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);
#endif
    lv_init();
    touch_init();
    screenWidth = lcd->width();
    screenHeight = lcd->height();
    lv_disp_draw_buf_init(&draw_buf, disp_draw_buf, NULL, screenWidth * screenHeight / 10);
    lv_disp_drv_init(&disp_drv);
    /* Change the following line to your display resolution */
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    /* Initialize the (dummy) input device driver */
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);
    ui_init(); // 开机UI界面

    lcd->fillScreen(BLACK);
    Serial.println("Setup done");

    // Create a JSON object
    // DynamicJsonDocument jsonDoc(200);
    // jsonDoc["sensor"] = "ESP32";
    // jsonDoc["value"] = 42;

    // // Serialize JSON to a string
    // String jsonString;
    // serializeJson(jsonDoc, jsonString);

    // // Send JSON data over UART
    // Serial.println(jsonString);
    ui_reset();
    xTaskCreatePinnedToCore(json_reader,
                            "json_reader",
                            4096,
                            NULL,
                            0,
                            NULL,
                            1);

    while (1)
    {
        if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY))
        {
            lv_task_handler();
            xSemaphoreGive(xGuiSemaphore);
            delay(5);
        }

        // lv_timer_handler();
        //  delay(5);
    }
    vTaskDelete(NULL);
}

void ui_reset()
{
    lv_slider_set_value(ui_Slider1, 0, LV_ANIM_OFF);
    lv_slider_set_value(ui_Slider2, 0, LV_ANIM_OFF);

    lv_obj_clear_flag(ui_Slider1, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(ui_Slider2, LV_OBJ_FLAG_CLICKABLE);

    lv_label_set_text(ui_Speed_Number_1, "0");
    lv_label_set_text(ui_Speed_Number_2, "0");
    lv_label_set_text(ui_Label1, "0");
    lv_bar_set_value(ui_Bar1, 0, LV_ANIM_OFF);
    lv_bar_set_value(ui_Bar2, 0, LV_ANIM_OFF);
    lv_bar_set_value(ui_Bar3, 0, LV_ANIM_OFF);

    lv_label_set_text(ui_Label2, "+0");
}

void refresh_date(DynamicJsonDocument &data)
{

    auto power_percentage = data["power_generated_percentage"].as<float>();
    auto speed_percentage = data["speed_percentage"].as<float>();
    auto speed_value = data["speed_value"].as<float>();
    auto power_generated_value = data["power_generated_value"].as<float>();
    auto temp_percentage = data["temp_percentage"].as<float>();
    auto temp_value = data["temp_value"].as<float>();

    /* Battery information*/

    auto b1_value_voltage = data["b1_value_voltage"].as<float>();
    auto b1_value_distance = data["b1_value_distance"].as<float>();
    auto b1_value = data["b1_value_percentage"].as<float>();
    auto b1_value_percentage = data["b1_value_percentage"].as<float>();

    auto b2_value_voltage = data["b2_value_voltage"].as<float>();
    auto b2_value_distance = data["b2_value_distance"].as<float>();
    auto b2_value = data["b2_value_percentage"].as<float>();
    auto b2_value_percentage = data["b2_value_percentage"].as<float>();

    Serial.println(temp_value);
    Serial.println(String(temp_value ? temp_value : 0, 1).c_str());

    lv_slider_set_value(ui_Slider1, int(power_percentage ? power_percentage : 0), LV_ANIM_ON);
    lv_slider_set_value(ui_Slider2, int(speed_percentage ? speed_percentage : 0), LV_ANIM_ON);

    lv_obj_clear_flag(ui_Slider1, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(ui_Slider2, LV_OBJ_FLAG_CLICKABLE);

    lv_label_set_text(ui_Speed_Number_1, String(speed_value ? speed_value : 0, 0).c_str());
    lv_label_set_text(ui_Speed_Number_2, String(speed_value ? speed_value : 0, 0).c_str());
    lv_label_set_text(ui_Label1, String(power_generated_value ? power_generated_value : 0, 1).c_str());
    lv_bar_set_value(ui_Bar1, int(temp_percentage ? temp_percentage : 0), LV_ANIM_ON);
    lv_label_set_text(ui_Label2, String(temp_value ? temp_value : 0, 1).c_str());

    lv_label_set_text(ui_Label3, String(b1_value_voltage ? b1_value_voltage : 0, 0).c_str());   // b1 battery voltage
    lv_label_set_text(ui_Label9, String(b1_value_distance ? b1_value_distance : 0, 1).c_str()); // b1 is distance cal
    lv_label_set_text(ui_Label3, String(b1_value ? b1_value : 0, 0).c_str());
    lv_bar_set_value(ui_Bar2, int(b1_value_percentage ? b1_value_percentage : 0), LV_ANIM_ON);

    lv_label_set_text(ui_Label4, String(b2_value_voltage ? b2_value_voltage : 0, 0).c_str());
    lv_label_set_text(ui_Label10, String(b2_value_distance ? b2_value_distance : 0, 1).c_str());
    lv_label_set_text(ui_Label15, String(b2_value ? b2_value : 0, 0).c_str());
    lv_bar_set_value(ui_Bar3, int(b2_value_percentage ? b2_value_percentage : 0), LV_ANIM_ON);
}

static void json_reader(void *pvParameter)
{
    (void)pvParameter;
    // using SpiRamJsonDocument = BasicJsonDocument<SpiRamAllocator>;

    while (1)
    {
        String data = Serial.readStringUntil('\n');

        // Parse received JSON data
        // SpiRamJsonDocument jsonDoc(500); // // Psram allocated
        DynamicJsonDocument jsonDoc(500);
        DeserializationError error = deserializeJson(jsonDoc, data);

        if (error)
        {
            // Serial.println("Error parsing JSON data");
        }
        else
        {
            Serial.println("Received JSON data:");
            serializeJson(jsonDoc, Serial);

            // const char *sensor = jsonDoc["sensor"];
            refresh_date(jsonDoc);
        }
        vTaskDelay(pdMS_TO_TICKS(10)); // Adjust the delay as needed
    }
}
