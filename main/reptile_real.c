#include "reptile_real.h"
#include "sensors.h"
#include "gpio.h"
#include "lvgl.h"
#include "lvgl_port.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

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

extern lv_obj_t *menu_screen;

static void update_status_labels(void) {
  float temp = sensors_read_temperature();
  float hum = sensors_read_humidity();
  lv_label_set_text_fmt(label_temp, "Temp\u00e9rature: %.1f \u00b0C", temp);
  lv_label_set_text_fmt(label_hum, "Humidit\u00e9: %.1f %%", hum);
  lv_label_set_text(label_pump, pump_running ? "Pompe: ON" : "Pompe: OFF");
  lv_label_set_text(label_heat, heat_running ? "Chauffage: ON" : "Chauffage: OFF");
  lv_label_set_text(label_feed, feed_running ? "Nourrissage: ON" : "Nourrissage: OFF");
}

static void sensor_timer_cb(lv_timer_t *t) {
  (void)t;
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
  vTaskDelete(NULL);
}

static void pump_btn_cb(lv_event_t *e) {
  (void)e;
  if (!pump_running)
    xTaskCreate(pump_task, "pump_task", 2048, NULL, 5, NULL);
}

static void heat_btn_cb(lv_event_t *e) {
  (void)e;
  if (!heat_running)
    xTaskCreate(heat_task, "heat_task", 2048, NULL, 5, NULL);
}

static void feed_btn_cb(lv_event_t *e) {
  (void)e;
  if (!feed_running)
    xTaskCreate(feed_task, "feed_task", 2048, NULL, 5, NULL);
}

static void menu_btn_cb(lv_event_t *e) {
  (void)e;
  if (lvgl_port_lock(-1)) {
    lv_scr_load(menu_screen);
    if (update_timer) {
      lv_timer_del(update_timer);
      update_timer = NULL;
    }
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

  if (sensors_init() != ESP_OK) {
    lv_obj_t *mbox = lv_msgbox_create(NULL);
    lv_msgbox_add_title(mbox, "Erreur");
    lv_msgbox_add_text(mbox, "Capteurs non initialis\u00e9s");
    lv_msgbox_add_close_button(mbox);
    lv_obj_center(mbox);
    return;
  }

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

