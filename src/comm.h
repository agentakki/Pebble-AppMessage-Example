#pragma once
#define KEY_DATA 5 // DELETE ME
  
#define LIST_RESPONSE "LIST_RESPONSE"
#define TASK "TASK"
  
#define POMO_COMPLETE "POMO_COMPLETE"
#define LIST_REQUEST "LIST_REQUEST"

/*
  When called, signals the iOS app that a pomo 
  has been completed for the task with id t_id
*/
void pomo_completed(int t_id);

void list_request();

void inbox_received_callback(DictionaryIterator *iterator, void *context);

void inbox_dropped_callback(AppMessageResult reason, void *context);

void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context);

void outbox_sent_callback(DictionaryIterator *iterator, void *context);

void init_comm();