#include <pebble.h>

#define KEY_DATA 5

static Window *s_main_window;
static TextLayer *s_output_layer;

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  text_layer_set_text(s_output_layer, "inbox_received_callback");


  // Get the first pair
  Tuple *t = dict_read_first(iterator);

  // Process all pairs present
  while (t != NULL) {
    // Long lived buffer
    static char s_buffer[64];

    // Process this pair's key
    switch (t->key) {
      case KEY_DATA:{
        // Copy value and display
        int key = (int) t->key;
        snprintf(s_buffer, sizeof(s_buffer), "Received '%d' : '%s'", key, t->value->cstring);
        text_layer_set_text(s_output_layer, s_buffer);
        break;
    }
    }

    // Get next pair, if any
    t = dict_read_next(iterator);
  }
  
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  text_layer_set_text(s_output_layer, "Message dropped!");


  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
  text_layer_set_text(s_output_layer, "failed :(");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
  text_layer_set_text(s_output_layer, "Message sent!");
}

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);

  // Create output TextLayer
  s_output_layer = text_layer_create(GRect(5, 0, window_bounds.size.w - 5, window_bounds.size.h));
  text_layer_set_font(s_output_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  text_layer_set_text(s_output_layer, "Waiting for response from RockTomato ...");
  text_layer_set_overflow_mode(s_output_layer, GTextOverflowModeWordWrap);
  layer_add_child(window_layer, text_layer_get_layer(s_output_layer));
}

static void main_window_unload(Window *window) {
  // Destroy output TextLayer
  text_layer_destroy(s_output_layer);
}

void down_single_click_handler(ClickRecognizerRef recognizer, void *context) {
  //Window *window = (Window *)context;
  text_layer_set_text(s_output_layer, "Begin transmission!");
  
  DictionaryIterator *iter;
  
  app_message_outbox_begin(&iter);
  if (iter == NULL) return;
  
  dict_write_cstring(iter, 42, "Hi Akshay!");
  dict_write_end(iter);
  
  app_message_outbox_send();
  
}

void config_provider(Window *window) {
    window_single_click_subscribe(BUTTON_ID_DOWN, down_single_click_handler);
  
}

static void init() {
  // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);

  // Open AppMessage
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());

  // Create main Window
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  
  window_set_click_config_provider(s_main_window, (ClickConfigProvider) config_provider);
  
  window_stack_push(s_main_window, true);
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