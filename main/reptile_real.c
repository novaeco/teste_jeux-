#include "reptile_real.h"
#include "sensors.h"
#include "gpio.h"
#include "lvgl.h"
#include "lvgl_port.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "settings.h"
#include <math.h>

static void pump_task(void *arg);
static void heat_task(void *arg);
static void feed_task(void *arg);

static lv_obj_t *screen;
static lv_obj_t *label_temp;
static lv_obj_t *label_hum;
static lv_obj_t *label_pump;
static lv_obj_t *label_heat;
static lv_obj_t *label_feed;
static lv_timer_t *update_timer;
static volatile bool pump_running;
static volatile bool heat_running;
static volatile bool feed_running;
static bool pump_auto_active;
static bool heat_auto_active;
static TaskHandle_t pump_task_handle;
static TaskHandle_t heat_task_handle;
static TaskHandle_t feed_task_handle;

extern lv_obj_t *menu_screen;

static void update_status_labels(void) {
  float temp = sensors_read_temperature();
  float hum = sensors_read_humidity();
  if (isnan(temp))
    lv_label_set_text(label_temp, "Temp\u00e9rature: N/A");
  else
    lv_label_set_text_fmt(label_temp, "Temp\u00e9rature: %.1f \u00b0C", temp);
  if (isnan(hum))
    lv_label_set_text(label_hum, "Humidit\u00e9: N/A");
  else
    lv_label_set_text_fmt(label_hum, "Humidit\u00e9: %.1f %%", hum);
  lv_label_set_text(label_pump,
                    pump_running    ? "Pompe: ON" :
                    pump_auto_active? "Pompe: AUTO" : "Pompe: OFF");
  lv_label_set_text(label_heat,
                    heat_running    ? "Chauffage: ON" :
                    heat_auto_active? "Chauffage: AUTO" : "Chauffage: OFF");
  lv_label_set_text(label_feed, feed_running ? "Nourrissage: ON" : "Nourrissage: OFF");
}

static void sensor_timer_cb(lv_timer_t *t) {
  (void)t;

  float temp = sensors_read_temperature();
  float hum = sensors_read_humidity();

  /* Gestion du chauffage avec hystérésis ±1 °C */
  if (temp <= g_settings.temp_threshold - 1) {
    heat_auto_active = true;
  } else if (temp >= g_settings.temp_threshold + 1) {
    heat_auto_active = false;
  }
  if (heat_auto_active && !heat_running)
    xTaskCreate(heat_task, "heat_task", 2048, NULL, 5, &heat_task_handle);

  /* Gestion de l'humidité avec hystérésis ±5 % HR */
  if (hum <= g_settings.humidity_threshold - 5) {
    pump_auto_active = true;
  } else if (hum >= g_settings.humidity_threshold + 5) {
    pump_auto_active = false;
  }
  if (pump_auto_active && !pump_running)
    xTaskCreate(pump_task, "pump_task", 2048, NULL, 5, &pump_task_handle);

  update_status_labels();
}

static void pump_task(void *arg) {
  (void)arg;
  pump_running = true;
  if (lvgl_port_lock(-1)) {
    update_status_labels();
    lvgl_port_unlock();
  }
  reptile_water_gpio();
  pump_running = false;
  if (lvgl_port_lock(-1)) {
    update_status_labels();
    lvgl_port_unlock();
  }
  pump_task_handle = NULL;
  vTaskDelete(NULL);
}

static void heat_task(void *arg) {
  (void)arg;
  heat_running = true;
  if (lvgl_port_lock(-1)) {
    update_status_labels();
    lvgl_port_unlock();
  }
  reptile_heat_gpio();
  heat_running = false;
  if (lvgl_port_lock(-1)) {
    update_status_labels();
    lvgl_port_unlock();
  }
  heat_task_handle = NULL;
  vTaskDelete(NULL);
}

static void feed_task(void *arg) {
  (void)arg;
  feed_running = true;
  if (lvgl_port_lock(-1)) {
    update_status_labels();
    lvgl_port_unlock();
  }
  reptile_feed_gpio();
  feed_running = false;
  if (lvgl_port_lock(-1)) {
    update_status_labels();
    lvgl_port_unlock();
  }
  feed_task_handle = NULL;
  vTaskDelete(NULL);
}

static void pump_btn_cb(lv_event_t *e) {
  (void)e;
  if (!pump_running)
    xTaskCreate(pump_task, "pump_task", 2048, NULL, 5, &pump_task_handle);
}

static void heat_btn_cb(lv_event_t *e) {
  (void)e;
  if (!heat_running)
    xTaskCreate(heat_task, "heat_task", 2048, NULL, 5, &heat_task_handle);
}

static void feed_btn_cb(lv_event_t *e) {
  (void)e;
  if (!feed_running)
    xTaskCreate(feed_task, "feed_task", 2048, NULL, 5, &feed_task_handle);
}

static void menu_btn_cb(lv_event_t *e) {
  (void)e;
  // Stop regulation tasks and ensure hardware is off
  if (pump_task_handle) {
    vTaskDelete(pump_task_handle);
    pump_task_handle = NULL;
  }
  if (heat_task_handle) {
    vTaskDelete(heat_task_handle);
    heat_task_handle = NULL;
  }
  if (feed_task_handle) {
    vTaskDelete(feed_task_handle);
    feed_task_handle = NULL;
  }
  pump_running = false;
  heat_running = false;
  feed_running = false;
  pump_auto_active = false;
  heat_auto_active = false;
  DEV_Digital_Write(WATER_PUMP_PIN, 0);
  DEV_Digital_Write(HEAT_RES_PIN, 0);
  DEV_Digital_Write(SERVO_FEED_PIN, 0);

  if (lvgl_port_lock(-1)) {
    if (update_timer) {
      lv_timer_del(update_timer);
      update_timer = NULL;
    }
    lv_scr_load(menu_screen);
    if (screen) {
      lv_obj_del(screen);
      screen = NULL;
    }
    lvgl_port_unlock();
  }
}

void reptile_real_start(esp_lcd_panel_handle_t panel, esp_lcd_touch_handle_t tp) {
  (void)panel;
  (void)tp;

  sensors_init();

  if (!lvgl_port_lock(-1))
    return;

  screen = lv_obj_create(NULL);

  label_temp = lv_label_create(screen);
  lv_obj_align(label_temp, LV_ALIGN_TOP_LEFT, 10, 10);

  label_hum = lv_label_create(screen);
  lv_obj_align(label_hum, LV_ALIGN_TOP_LEFT, 10, 40);

  label_pump = lv_label_create(screen);
  lv_obj_align(label_pump, LV_ALIGN_TOP_LEFT, 10, 80);
  lv_obj_t *btn_pump = lv_btn_create(screen);
  lv_obj_align(btn_pump, LV_ALIGN_TOP_RIGHT, -10, 75);
  lv_obj_t *lbl = lv_label_create(btn_pump);
  lv_label_set_text(lbl, "Pompe");
  lv_obj_center(lbl);
  lv_obj_add_event_cb(btn_pump, pump_btn_cb, LV_EVENT_CLICKED, NULL);

  label_heat = lv_label_create(screen);
  lv_obj_align(label_heat, LV_ALIGN_TOP_LEFT, 10, 120);
  lv_obj_t *btn_heat = lv_btn_create(screen);
  lv_obj_align(btn_heat, LV_ALIGN_TOP_RIGHT, -10, 115);
  lbl = lv_label_create(btn_heat);
  lv_label_set_text(lbl, "Chauffage");
  lv_obj_center(lbl);
  lv_obj_add_event_cb(btn_heat, heat_btn_cb, LV_EVENT_CLICKED, NULL);

  label_feed = lv_label_create(screen);
  lv_obj_align(label_feed, LV_ALIGN_TOP_LEFT, 10, 160);
  lv_obj_t *btn_feed = lv_btn_create(screen);
  lv_obj_align(btn_feed, LV_ALIGN_TOP_RIGHT, -10, 155);
  lbl = lv_label_create(btn_feed);
  lv_label_set_text(lbl, "Nourrir");
  lv_obj_center(lbl);
  lv_obj_add_event_cb(btn_feed, feed_btn_cb, LV_EVENT_CLICKED, NULL);

  lv_obj_t *btn_menu = lv_btn_create(screen);
  lv_obj_align(btn_menu, LV_ALIGN_BOTTOM_MID, 0, -10);
  lbl = lv_label_create(btn_menu);
  lv_label_set_text(lbl, "Menu");
  lv_obj_center(lbl);
  lv_obj_add_event_cb(btn_menu, menu_btn_cb, LV_EVENT_CLICKED, NULL);

  update_status_labels();
  update_timer = lv_timer_create(sensor_timer_cb, 1000, NULL);
  lv_disp_load_scr(screen);
  lvgl_port_unlock();
}

