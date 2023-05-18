#pragma once

#include "../ecs/world.hpp"
#include <glm/glm.hpp>

#include <iostream>
using namespace std;

namespace our
{

    // The crashing system is responsible for checking if the car has crashed with any obstacle.
    class BigObstaclesSystem {
				// Save the car entity
        vector<glm::vec2> pickUpPoints={
                        glm::vec2(30, -3.75),
                        glm::vec2(-30, 1),
                        glm::vec2(-50, -30),
                        glm::vec2(-54, 25),
                        glm::vec2(-20, 40),
                        glm::vec2(60, 37),
                        glm::vec2(55, 10)
        }; // points that we will choose randomly from them
        vector<glm::vec2> horizontalPoints={
                glm::vec2(0, -5), // middle
                glm::vec2(0, -47), // left
                glm::vec2(0, 34), // right
                }, verticalPoints={
                    glm::vec2(52, 0), // up
                    glm::vec2(-54, 0), // down
                };


        nlohmann::json generateBigObstacle(glm::vec3 startingPosition,glm::vec3 startingLinearVelocity , glm::vec3 startingAngularVelocity) {
            return {
                    {"name", "obstacles"},
                    {"direction", true},
                    {"position", {startingPosition.x, startingPosition.y, startingPosition.z}},
                    {"rotation", {0, 90, 0}},
                    {"scale", {2, 2, 2}},
                    {"components", nlohmann::json::array({
                         {
                                 {"type", "Mesh Renderer"},
                                 {"mesh", "monkey"},
                                 {"material", "monkey"}
                         },
                         {
                                 {"type", "Movement"},
                                 {"linearVelocity", {startingLinearVelocity.x, startingLinearVelocity.y, startingLinearVelocity.z}},
                                 {"angularVelocity", {startingAngularVelocity.x, startingAngularVelocity.y, startingAngularVelocity.z}}
                        }
                    })}
            };
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
        void initialize(World* world, int numOfBigObstacles) {

            for(glm::vec2 point : horizontalPoints){
                    Entity* delivery = world->add();
                    delivery->deserialize(
                            generateBigObstacle(glm::vec3(point.x ,3.5, point.y),
                                                glm::vec3(20,0,0),
                                                glm::vec3(0,0,0)
                                                ));
            }

            for(glm::vec2 point : verticalPoints){
                Entity* delivery = world->add();
                delivery->deserialize(
                        generateBigObstacle(glm::vec3(point.x ,3.5, point.y),
                                            glm::vec3(0,0,20),
                                            glm::vec3(0,0,0)
                                            ));
            }

        }

    };
}
