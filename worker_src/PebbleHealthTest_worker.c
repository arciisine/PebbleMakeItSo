#include <pebble_worker.h>
#include "../src/message_format.h"

AppTimer* next_event_limit;

void on_app_message(uint16_t type, AppWorkerMessage *data) {
  
}

void on_health_event(HealthEventType event, void *context) {
   APP_LOG(APP_LOG_LEVEL_DEBUG,  "Health Event: %d", event);
   worker_launch_app();
}

void on_timer_event(void* data) {
   APP_LOG(APP_LOG_LEVEL_DEBUG,  "Timer Event");
   worker_launch_app();
}

void init() {
  app_worker_message_subscribe(on_app_message);
  app_worker_send_message(ActionStart, NULL);
  health_service_events_subscribe(on_health_event, NULL);
  app_timer_register((uint32_t)10000, on_timer_event, NULL);
}

void deinit() {
  app_worker_send_message(ActionStop, NULL);
  app_worker_message_unsubscribe();
  health_service_events_unsubscribe();
  if (next_event_limit != NULL) {
    app_timer_cancel(next_event_limit);
  }
}

int main(void) {
  init();
  worker_event_loop();
  deinit();
}
