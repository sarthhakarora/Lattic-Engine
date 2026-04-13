#include "timer.h"
#include "raylib.h"
#include "stdbool.h"

Timer create_timer(float time) {
    Timer timer = {
        .timePassed = 0,
        .timeSpan = time
    };

    return timer;
}

void update_timer(Timer *timer) {
    timer->timePassed += GetFrameTime();   
}

bool check_timer(Timer *timer) {
    bool isactive;

    if (timer->timePassed >= timer->timeSpan) {
        timer->timePassed = 0;
        isactive = true;
    } else {
        isactive = false;
    }

    return isactive;
}
