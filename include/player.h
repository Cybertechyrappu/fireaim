#ifndef PLAYER_H
#define PLAYER_H

#include <cmath>

struct Player {
    double x = 0.0, y = 1.7, z = 0.0;
    double yaw = 0.0;
    double pitch = 0.0;
    bool isShooting = false;
    double lastFireTime = 0.0;

    void updateLook(double dyaw, double dpitch) {
        yaw += dyaw;
        pitch = std::clamp(pitch + dpitch, -M_PI / 3.0, M_PI / 3.0);
    }

    void getLookDirection(double& dx, double& dy, double& dz) const {
        dx = std::sin(yaw) * std::cos(pitch);
        dy = std::sin(pitch);
        dz = std::cos(yaw) * std::cos(pitch);
    }

    bool canShoot(double currentTime) const {
        return (currentTime - lastFireTime) >= GameConstants::FIRE_RATE;
    }
};

#endif // PLAYER_H