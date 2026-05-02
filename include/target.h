#ifndef TARGET_H
#define TARGET_H

#include <string>

struct Target {
    std::string id;
    double x, y, z;
    double speed;
    int points;
    bool active;

    Target(const std::string& id_, double x_, double y_, double z_, double speed_)
        : id(id_), x(x_), y(y_), z(z_), speed(speed_), points(GameConstants::BASE_SCORE), active(true) {}

    void update(double deltaTime, double currentTime) {
        double angle = currentTime * speed * 0.5;
        double newX = x * std::cos(angle) - z * std::sin(angle);
        double newZ = x * std::sin(angle) + z * std::cos(angle);
        x = newX;
        z = newZ;
    }
};

#endif // TARGET_H