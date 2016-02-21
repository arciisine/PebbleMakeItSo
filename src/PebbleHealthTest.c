#include <pebble.h>
#include "message_format.h"

static Window *window;
static TextLayer *text_layer;
static bool running = false;

static void set_running_message() {
  text_layer_set_text(text_layer, running ? "Running" : "Stopped"); 
}

void on_app_message(uint16_t type, AppWorkerMessage *data) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Received message: %d", type);
  if (type == ActionStart) {
    running = true;
    set_running_message();
  } else if (type == ActionStop) {
    running = false;
    set_running_message();
  }
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  app_worker_launch();
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  text_layer = text_layer_create(GRect(0, 72, bounds.size.w, 20));
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  set_running_message();
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

static void window_unload(Window *window) {
  text_layer_destroy(text_layer);
}

static void init(void) {
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
  app_worker_message_subscribe(on_app_message);
}

static void deinit(void) {
  app_worker_message_unsubscribe();
  window_destroy(window);
}

int main(void) {
  running = app_worker_is_running();
  if (launch_reason() == APP_LAUNCH_WORKER) {
    app_worker_kill();
  }
  init();
  app_event_loop();
  deinit();
}
