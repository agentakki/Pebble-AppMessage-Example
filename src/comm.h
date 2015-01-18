#pragma once
#define KEY_DATA 5

void pomo_completed();

void inbox_received_callback(DictionaryIterator *iterator, void *context);

void inbox_dropped_callback(AppMessageResult reason, void *context);

void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context);

void outbox_sent_callback(DictionaryIterator *iterator, void *context);

void down_single_click_handler(ClickRecognizerRef recognizer, void *context);

void config_provider(Window *window);

void init_comm();