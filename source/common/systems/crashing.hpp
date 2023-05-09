#pragma once

#include "../ecs/world.hpp"

#include <glm/glm.hpp>
#include <systems/event-handler.hpp>
#include <systems/energy.hpp>


#include <iostream>
using namespace std;

namespace our
{

    // The crashing system is responsible for checking if the car has crashed with any obstacle.
    class CrashingSystem {
				// Save the car entity
        Entity *car;
        EventHandlerSystem *events;
        EnergySystem *energy;

        float distanceXYZ(glm::vec4 a, glm::vec4 b){
            return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2));
        }

        float distanceXZ(glm::vec4 a, glm::vec4 b){
            return sqrt(pow(a.x - b.x, 2) + pow(a.z - b.z, 2));
        }
				
        bool volumeCrashEvent(Entity *car, Entity *object, float threshold) {
						// Get the car position
						glm::vec4 carPosition = car->getLocalToWorldMatrix() * glm::vec4(car->localTransform.position, 1);

						// Get the distance between the car and the object
						float dis = distanceXYZ(carPosition, glm::vec4(object->localTransform.position, 1));

						return dis < threshold;
        }

        bool areaCrashEvent(Entity *car, Entity *object, float threshold) {
            // Get the car position
            glm::vec4 carPosition = car->getLocalToWorldMatrix() * glm::vec4(car->localTransform.position, 1);
            glm::vec4 objectPosition = glm::vec4(object->localTransform.position, 1);

            // Get the distance between the car and the object
            float dis = distanceXZ(carPosition, objectPosition);

            return dis < threshold;
        }

    public:


        void initializeCrashingSystem(World* world,EventHandlerSystem * events, EnergySystem * energy) {
            getCar(world);
            this->events = events;
            this->energy = energy;
        }

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

                if(entity->name == "battery" && volumeCrashEvent(car, entity, 3)) {
                    printf("battery\n");
                    energy->batteryCrash();
                } else if(entity->name == "obstacles" && volumeCrashEvent(car, entity, 5)) {
                    printf("obstacles\n");
                    energy->obstacleCrash();
                } else if(entity->name == "building" && areaCrashEvent(car, entity, 14)){
                    printf("building\n");
                    energy->buildingCrash();
                } else if(entity->name == "arrow" && areaCrashEvent(car, entity, 3)){
                    printf("arrow\n");
                    events->winGame();
                }
            }
        }
    };

}
