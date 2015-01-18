#include <pebble.h>
#include "comm.h"

static Window *s_main_window;

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
}

static void main_window_unload(Window *window) {
}

void down_single_click_handler(ClickRecognizerRef recognizer, void *context) {
  pomo_completed(42);
}

void up_single_click_handler(ClickRecognizerRef recognizer, void *context) {
  list_request();
}

void config_provider(Window *window) {
    window_single_click_subscribe(BUTTON_ID_DOWN, down_single_click_handler);
    window_single_click_subscribe(BUTTON_ID_UP, up_single_click_handler);
}

static void init() {
  // Create main Window
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  
  window_stack_push(s_main_window, true);
  
  window_set_click_config_provider(s_main_window, (ClickConfigProvider) config_provider);

  // init comm
  init_comm();
  
}

static void deinit() {
  // Destroy main Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
