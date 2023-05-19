#pragma once

#include "../ecs/world.hpp"
#include "../components/movement.hpp"
#include "../components/battery.hpp"
#include <glm/glm.hpp>
#include <chrono>

#include <iostream>
using namespace std;

#define delayTime 5000

namespace our
{

    // The battery system is responsible for handling the crashing of the car with any battery.
    class BatterySystem {
        std::chrono::high_resolution_clock::time_point lastTime;

    public:
        void takeBattery(Entity* battery) {
            lastTime = std::chrono::high_resolution_clock::now();
						MovementComponent* movement = battery->getComponent<MovementComponent>();
						movement->movementRangeY = glm::vec2(100.0, 120.0);
						battery->localTransform.position.y = 100.0;

						BatteryComponent* batteryComponent = battery->getComponent<BatteryComponent>();
						batteryComponent->isTaken = true;
        }

				void update(World* world) {
						auto currentTime = std::chrono::high_resolution_clock::now();
            auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count();
						if (elapsedTime > delayTime) {
								// For each battery, check if it is taken, if so, return the movement range to its original values
								for (auto entity : world->getEntities()) {
									if (entity->name == "battery") {
										BatteryComponent* batteryComponent = entity->getComponent<BatteryComponent>();
										if (batteryComponent->isTaken) {
												batteryComponent->isTaken = false;
												// return the battery to its original position
												MovementComponent* movement = entity->getComponent<MovementComponent>();
												movement->movementRangeY = glm::vec2(1.5, 4.0);

												entity->localTransform.position.y = 3.0;
										}
									}
								}
						}
				}
    };
}
