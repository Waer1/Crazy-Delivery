#pragma once

#include "../ecs/world.hpp"

#include <glm/glm.hpp>

#include <iostream>
using namespace std;

namespace our
{

    // The crashing system is responsible for checking if the car has crashed with any obstacle.
    class CrashingSystem {
        float distance(glm::vec4 a, glm::vec4 b){
            return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2));
        }

    public:

        void update(World* world) {
            // For each entity in the world
            Entity *car;
            for(auto entity : world->getEntities()){
                    if(entity->name == "car"){
                        car = entity;
                        break;
                    }
            }

            for(auto entity : world->getEntities()) {
                if(entity->name == "car"){
                    continue;
                }


                if(distance( car->getLocalToWorldMatrix() * glm::vec4(car->localTransform.position ,1)  ,
                             entity->getLocalToWorldMatrix() * glm::vec4(entity->localTransform.position,1 )) < 10){

                    if(entity->name == "battery") {
                        printf("battery\n");
                    } else if(entity->name == "obstacles") {
                        printf("obstacles\n");
                    } else if(entity->name == "fence"){
//                        printf("fence\n");
                    } else if(entity->name == "arrow"){
                        printf("arrow\n");
                    }

                }

            }


        }
    };

}