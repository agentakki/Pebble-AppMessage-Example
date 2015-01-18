#include <pebble.h>
#include "comm.h"

void pomo_completed(int t_id) {
  
  APP_LOG(APP_LOG_LEVEL_ERROR, "sending pomo");
  
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);
  if (iter == NULL) return;
  dict_write_cstring(iter, 0, POMO_COMPLETE);
  dict_write_int(iter, 1, &t_id, sizeof(int), false);
  dict_write_end(iter);
  app_message_outbox_send();
  
}

void list_request() {
  
  APP_LOG(APP_LOG_LEVEL_ERROR, "sending list request");
 
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);
  if (iter == NULL) return;
  dict_write_cstring(iter, 0, LIST_REQUEST);
  dict_write_end(iter);
  app_message_outbox_send();
  
}

void inbox_received_callback(DictionaryIterator *iterator, void *context) {

  // Get the first pair
  Tuple *t = dict_read_first(iterator);
  
  //int nUnreceivedTasks = 0;
  
  if (!strcmp(t->value->cstring, LIST_RESPONSE)) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "received LIST_RESPONSE");
  }
  else if (!strcmp(t->value->cstring, TASK)) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "received TASK");
  }
  else {
    APP_LOG(APP_LOG_LEVEL_ERROR, "received unkown message from iOS app ...");
  }
}

void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
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
