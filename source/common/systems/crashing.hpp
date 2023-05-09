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
        vector<Entity*> BigObstacles;

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



    public:
        void initializeCrashingSystem(World* world,EventHandlerSystem * events, EnergySystem * energy) {
            this->events = events;
            this->energy = energy;
            getCar(world);
        }

        void update(World* world){
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
                    events->deliverDelivery();
                } else if(entity->name == "delivery" && areaCrashEvent(car, entity, 4)
                && !events->isCarryDeliver()) {
                    printf("delivery\n");
                    events->collectDeliver();
                }
            }

            for(auto bigObstacle : BigObstacles) {
                for(auto entity : world->getEntities()) {
                    if(entity->name == "building" && obstacleCrashEvent(entity, bigObstacle, 10)){
                        MovementComponent* bm = bigObstacle->getComponent<MovementComponent>();
                        bm->angularVelocity = generateRandomVec3(-150,150);
                        bm->linearVelocity = -bm->linearVelocity;
                    }
                }
            }

        }


    };

}
