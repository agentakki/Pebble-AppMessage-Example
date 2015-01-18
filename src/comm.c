#include <pebble.h>
#include "comm.h"
#include "globals.h"

void pomo_completed(int t_id) {
  // send sample data
  
  text_layer_set_text(s_output_layer, "Sending pomo to iOS app ..."); // optional
  
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);
  if (iter == NULL) return;
  dict_write_cstring(iter, 0, POMO_COMPLETE);
  dict_write_int(iter, 1, &t_id, sizeof(int), false);
  dict_write_end(iter);
  app_message_outbox_send();
  
}

void list_request() {
  
  text_layer_set_text(s_output_layer, "Sending list request to iOS app ..."); // optional
  
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);
  if (iter == NULL) return;
  dict_write_cstring(iter, 0, LIST_REQUEST);
  dict_write_end(iter);
  app_message_outbox_send();
  
}

void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  text_layer_set_text(s_output_layer, "inbox_received_callback");

  // Get the first pair
  Tuple *t = dict_read_first(iterator);
  
  int nUnreceivedTasks = 0;
  
  if (!strcmp(t->value->cstring, LIST_RESPONSE)) {
    text_layer_set_text(s_output_layer, "received LIST_RESPONSE"); 
  }
  else if (!strcmp(t->value->cstring, TASK)) {
    text_layer_set_text(s_output_layer, "received TASK"); 
  }
  else {
    text_layer_set_text(s_output_layer, "received unkown response from iOS app"); 
  }
  
  
  return;
  
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

void inbox_dropped_callback(AppMessageResult reason, void *context) {
  text_layer_set_text(s_output_layer, "Message dropped!");


  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
  text_layer_set_text(s_output_layer, "failed :(");
}

void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
  text_layer_set_text(s_output_layer, "Message sent!");
}

void init_comm() {
   // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);

  // Open AppMessage
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
}

void main_window_unload(Window *window) {
  // Destroy output TextLayer
  text_layer_destroy(s_output_layer);
}

