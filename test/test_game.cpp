#include "../include/game.h"
#include <cassert>
#include <iostream>

void testPlayer() {
    Player p;
    assert(p.x == 0.0);
    assert(p.y == 1.7);
    assert(p.z == 0.0);
    assert(p.yaw == 0.0);
    assert(p.pitch == 0.0);

    p.updateLook(0.1, 0.05);
    assert(p.yaw > 0.0);
    assert(p.pitch > 0.0);

    std::cout << "Player tests passed" << std::endl;
}

void testProjectile() {
    Projectile p("test", 0, 1.7, 0, 0, 0, -1, 1.0);
    assert(p.x == 0);
    assert(p.y == 1.7);
    assert(p.z == 0);
    assert(p.isExpired(5.0) == true);
    assert(p.isExpired(2.5) == false);

    std::cout << "Projectile tests passed" << std::endl;
}

void testTarget() {
    Target t("test_target", 5.0, 2.0, 3.0, 2.0);
    assert(t.id == "test_target");
    assert(t.x == 5.0);
    assert(t.y == 2.0);
    assert(t.z == 3.0);
    assert(t.speed == 2.0);
    assert(t.active == true);
    assert(t.points == 100);

    std::cout << "Target tests passed" << std::endl;
}

int main() {
    testPlayer();
    testProjectile();
    testTarget();
    std::cout << "All tests passed!" << std::endl;
    return 0;
}