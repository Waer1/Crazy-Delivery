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
        vector<Entity*> BigObstacles;

        float distanceXYZ(glm::vec4 a, glm::vec4 b){
            return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2));
        }

        float distanceXZ(glm::vec4 a, glm::vec4 b){
            return sqrt(pow(a.x - b.x, 2) + pow(a.z - b.z, 2));
        }
		
        bool obstacleCrashEvent(Entity *obstacle, Entity *object, float threshold) {
            // Get the car position
            glm::vec4 carPosition = glm::vec4(obstacle->localTransform.position, 1);
            glm::vec4 objectPosition = glm::vec4(object->localTransform.position, 1);

            // Get the distance between the car and the object
            float dis = distanceXZ(carPosition, objectPosition);

            return dis < threshold;
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

        void getBigObstacles(World* world) {
            // For each entity in the world
            for(auto entity : world->getEntities()){
                if(entity->name == "obstacles"){
                    BigObstacles.push_back(entity);
                    break;
                }
            }
        }

        glm::vec3 generateRandomVec3(int from, int to){
            srand(static_cast<unsigned int>(time(nullptr)));
            // Generate random x, y, and z values between x and y
            float randomX = static_cast<float>(rand()) / RAND_MAX * (to - from) + from;
            float randomZ = static_cast<float>(rand()) / RAND_MAX * (to - from) + from;

            // Construct a glm::vec3 with the random values
            glm::vec3 vec(randomX, 0, randomZ);
            return vec;
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

    public:
        void initialize(World* world, EventHandlerSystem* events, EnergySystem* energy, DeliverySystem* delivery) {
            this->events = events;
            this->energy = energy;
			this->delivery = delivery;
            getCar(world);
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
					} else if (entity->name == "arrow") {
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

            for(auto bigObstacle : BigObstacles) {
                for(auto entity : world->getEntities()) {
                    if(entity->name == "building" && obstacleCrashEvent(entity, bigObstacle, 10)){
                        MovementComponent* bm = bigObstacle->getComponent<MovementComponent>();
                        bm->angularVelocity = generateRandomVec3(0, 80);
                        bm->linearVelocity = -bm->linearVelocity;
                    }
                }
            }
        }

    };

}
