
//GENERATED, please run `npm run-script gen-message-format` to update
#pragma once
typedef enum {
  ActionStart = 0,
ActionStop = 1,
ActionTimer = 2,
ActionSteps = 3 
} Action;

typedef enum {
  AppKeyCount = 0 
} AppKey;

const char* ActionNames[] = { "Start",
"Stop",
"Timer",
"Steps"};

