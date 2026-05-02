#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <string>

struct Projectile {
    std::string id;
    double x, y, z;
    double dirX, dirY, dirZ;
    double createdAt;

    Projectile(const std::string& id_, double x_, double y_, double z_, 
               double dx_, double dy_, double dz_, double created_)
        : id(id_), x(x_), y(y_), z(z_), dirX(dx_), dirY(dy_), dirZ(dz_), createdAt(created_) {}

    void update(double deltaTime) {
        x += dirX * GameConstants::PROJECTILE_SPEED * deltaTime;
        y += dirY * GameConstants::PROJECTILE_SPEED * deltaTime;
        z += dirZ * GameConstants::PROJECTILE_SPEED * deltaTime;
    }

    bool isExpired(double currentTime) const {
        return (currentTime - createdAt) > 3.0;
    }
};

#endif // PROJECTILE_H