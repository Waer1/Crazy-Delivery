#pragma once

#include "../ecs/world.hpp"

#include <glm/glm.hpp>
#include <systems/event-handler.hpp>
#include <systems/energy.hpp>
#include <systems/delivery.hpp>
#include <systems/movement.hpp>


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
		DeliverySystem *delivery;

        void getTargetEntities(World* world) {
            // For each entity in the world
            for(auto entity : world->getEntities()){
                if(entity->name == "car"){
					car = entity;
                }
            }
        }

		bool crash(Entity *object1, Entity *object2) {
			// get the car's max and min position
			glm::vec3 object1Position = object1->localTransform.position;
			glm::vec3 object1Size = object1->localTransform.scale;
			glm::vec3 object1Max = object1Position + object1Size;
			glm::vec3 object1Min = object1Position - object1Size;

			// get the collider's position
			glm::vec3 object2Position = object2->localTransform.position;
			// get the collider's size
			glm::vec3 object2Size = object2->localTransform.scale;
			// get the collider's max and min position
			glm::vec3 object2Max = object2Position + object2Size;
			glm::vec3 object2Min = object2Position - object2Size;

			// if the car is in the range of the obstacle, take an action
			return (object1Max.x >= object2Min.x && object1Min.x <= object2Max.x &&
					object1Max.y >= object2Min.y && object1Min.y <= object2Max.y &&
					object1Max.z >= object2Min.z && object1Min.z <= object2Max.z);
		}
		
		bool crashDestination(Entity *object1, Entity *object2) {
			// get the car's max and min position
			glm::vec3 object1Position = object1->localTransform.position;
			glm::vec3 object1Size = object1->localTransform.scale;
			glm::vec3 object1Max = object1Position + object1Size;
			glm::vec3 object1Min = object1Position - object1Size;

			// get the collider's position
			glm::vec3 object2Position = object2->localTransform.position;
			// get the collider's size
			glm::vec3 object2Size = {1.5, 10.0, 1.5};
			// get the collider's max and min position
			glm::vec3 object2Max = object2Position + object2Size;
			glm::vec3 object2Min = object2Position - object2Size;

			// if the car is in the range of the obstacle, take an action
			return (object1Max.x >= object2Min.x && object1Min.x <= object2Max.x &&
					object1Max.y >= object2Min.y && object1Min.y <= object2Max.y &&
					object1Max.z >= object2Min.z && object1Min.z <= object2Max.z);
		}

    public:
        void initialize(World* world, EventHandlerSystem* events, EnergySystem* energy, DeliverySystem* delivery) {
            this->events = events;
            this->energy = energy;
			this->delivery = delivery;
            getTargetEntities(world);
        }

        void update(World* world) {
            // For each entity in the world
            for(auto entity : world->getEntities()) {
                if(entity->name == "car"){
                    continue;
                }

				if (crash(car, entity)) {
					if (entity->name == "battery") {
						printf("battery\n");
						energy->batteryCrash();
					} else if (entity->name == "obstacles") {
						printf("obstacles\n");
						energy->obstacleCrash();
					} else if (entity->name == "building") {
						printf("building\n");
						energy->buildingCrash();
					} else if (entity->name == "arrow" && crashDestination(car, entity)) {
						events->deliverDelivery();
						delivery->removeDeliveryOnCar();
					} else if (entity->name == "delivery") {
						if (!events->isCarryDeliver()) {
							events->collectDeliver();
							delivery->addDeliveryOnCar();
							delivery->removeDelivery(entity, world);
						}
					}
				}
			}


        }

    };

}
