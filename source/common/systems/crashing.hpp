#pragma once

#include "../ecs/world.hpp"

#include <glm/glm.hpp>

#include <iostream>
using namespace std;

namespace our
{

    // The crashing system is responsible for checking if the car has crashed with any obstacle.
    class CrashingSystem {
				// Save the car entity
        Entity *car;

        float distance(glm::vec4 a, glm::vec4 b){
            return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2));
        }

        bool crashEvent(Entity *car, Entity *object, float threshold) {
                // Get the car position
                glm::vec4 carPosition = car->getLocalToWorldMatrix() * glm::vec4(car->localTransform.position, 1);

                // Get the distance between the car and the object
                float dis = distance(carPosition, glm::vec4(object->localTransform.position, 1));

                return dis < threshold;
        }

        bool arrowCrashEvent(Entity *car, Entity *object, float threshold) {
            // Get the car position
            glm::vec4 carPosition = car->getLocalToWorldMatrix() * glm::vec4(car->localTransform.position, 1);
            glm::vec4 objectPosition = car->getLocalToWorldMatrix() * glm::vec4(object->localTransform.position, 1);


            // Get the distance between the car and the object
            float dis = sqrt(pow(carPosition.x - objectPosition.x, 2) + pow(carPosition.z - objectPosition.z, 2) );

            return dis < threshold;
        }
				
				bool fenceCrashEvent(Entity *car, Entity *object, bool type, float threshold) {
						// Get the car position
						glm::vec4 carPosition = car->getLocalToWorldMatrix() * glm::vec4(car->localTransform.position, 1);
						glm::vec4 objectPosition = object->getLocalToWorldMatrix() * glm::vec4(object->localTransform.position, 1);

						float dis = 0;
						if (type) { // 1 => Vertical
            		dis = abs(carPosition.x - objectPosition.x);
            		return dis < threshold && abs(carPosition.z - objectPosition.z) < 14;
						} else {    // 0 => Horizontal
            		dis = abs(carPosition.z - objectPosition.z);
            		return dis < threshold && abs(carPosition.x - objectPosition.x) < 48;
						}
				}
    public:

        void getCar(World* world) {
            // For each entity in the world
            for(auto entity : world->getEntities()){
                if(entity->name == "car"){
                        car = entity;
                        break;
                }
            }
        }

        void update(World* world) {
            // For each entity in the world
            for(auto entity : world->getEntities()) {
                if(entity->name == "car"){
                    continue;
                }

                if(entity->name == "battery" && crashEvent(car, entity, 3)) {
										printf("battery\n");
                } else if(entity->name == "obstacles" && crashEvent(car, entity, 5)) {
										printf("obstacles\n");
                } else if(entity->name == "fence-v" && fenceCrashEvent(car, entity, 1, 1)){
                    printf("fence-v\n");
                } else if(entity->name == "fence-h" && fenceCrashEvent(car, entity, 0, 1)){
                    printf("fence-h\n");
                } else if(entity->name == "arrow" && crashEvent(car, entity, 6)){
										printf("arrow\n");
                }
            }
        }
    };

}
