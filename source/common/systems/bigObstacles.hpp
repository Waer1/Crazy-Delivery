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

        nlohmann::json generateBigObstacle(glm::vec3 startingPosition, glm::vec3 startingAngularVelocity) {
            return {
                    {"name", "obstacles"},
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
                                                                         {"linearVelocity", {0, 0, 0}},
                                                                         {"angularVelocity", {0, 0, 0}}
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

						// Pick a random points from the pickUpPoints
						for (int i = 0; i < numOfBigObstacles; i++) {
								int randomIndex = rand() % pickUpPoints.size();
								glm::vec2 randomPoint = pickUpPoints[randomIndex];
								pickUpPoints.erase(pickUpPoints.begin() + randomIndex);
								Entity* delivery = world->add();
								delivery->deserialize(generateBigObstacle(glm::vec3(randomPoint.x ,3.5, randomPoint.y), generateRandomVec3(0,60)));
						}
				}

    };
}
