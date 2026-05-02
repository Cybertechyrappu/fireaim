#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace GameConstants {
    const int SCREEN_WIDTH = 1280;
    const int SCREEN_HEIGHT = 720;
    const int TARGETS_PER_WAVE = 10;
    const int TOTAL_WAVES = 5;
    const int BASE_SCORE = 100;
    const double FIRE_RATE = 0.3;
    const double PROJECTILE_SPEED = 50.0;
    const double TARGET_SPEED = 2.0;
    const double TARGET_SPEED_INCREMENT = 0.5;
    const double TARGET_MIN_DISTANCE = 5.0;
    const double TARGET_MAX_DISTANCE = 20.0;
    const int MAX_COMBO = 5;
    const double COMBO_TIME_WINDOW = 2.0;
}

#endif // CONSTANTS_H