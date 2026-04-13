#pragma once

#include "stdbool.h"

typedef struct {
    float timeSpan;
    float timePassed;
} Timer;

Timer create_timer(float time);
void update_timer(Timer *timer);
bool check_timer(Timer *timer);