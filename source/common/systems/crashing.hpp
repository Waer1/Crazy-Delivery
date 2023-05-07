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

        float distanceXYZ(glm::vec4 a, glm::vec4 b){
            return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2));
        }

        float distanceXZ(glm::vec4 a, glm::vec4 b){
            return sqrt(pow(a.x - b.x, 2) + pow(a.z - b.z, 2));
        }
				
        bool crashEvent(Entity *car, Entity *object, float threshold) {
						// Get the car position
						glm::vec4 carPosition = car->getLocalToWorldMatrix() * glm::vec4(car->localTransform.position, 1);

						// Get the distance between the car and the object
						float dis = distanceXYZ(carPosition, glm::vec4(object->localTransform.position, 1));

						return dis < threshold;
        }

        bool arrowCrashEvent(Entity *car, Entity *object, float threshold) {
            // Get the car position
            glm::vec4 carPosition = car->getLocalToWorldMatrix() * glm::vec4(car->localTransform.position, 1);
            glm::vec4 objectPosition = glm::vec4(object->localTransform.position, 1);

            // Get the distance between the car and the object
            float dis = distanceXZ(carPosition, objectPosition);

            return dis < threshold;
        }
				
				bool fenceCrashEvent(Entity *car, Entity *object, float threshold) {
						// Get the car position
						glm::vec4 carPosition = car->getLocalToWorldMatrix() * glm::vec4(car->localTransform.position, 1);
						glm::vec4 objectPosition = object->getLocalToWorldMatrix() * glm::vec4(object->localTransform.position, 1);

						float dis = distanceXZ(carPosition, objectPosition);

						return dis < threshold;
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
                } else if(entity->name == "fence" && fenceCrashEvent(car, entity, 0.7)){
                    printf("fence\n");
                } else if(entity->name == "arrow" && arrowCrashEvent(car, entity, 3)){
										printf("arrow\n");
                }
            }
        }
    };

}
