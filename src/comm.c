#include <pebble.h>
#include "comm.h"
#include "globals.h"

void pomo_completed() {
  // need to access
}

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

void init_comm() {
   // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);

  // Open AppMessage
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());

  window_set_click_config_provider(s_main_window, (ClickConfigProvider) config_provider);
}

void main_window_unload(Window *window) {
  // Destroy output TextLayer
  text_layer_destroy(s_output_layer);
}

