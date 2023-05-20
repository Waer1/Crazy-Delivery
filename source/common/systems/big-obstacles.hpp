#pragma once

#include "../ecs/world.hpp"
#include <glm/glm.hpp>

#include <iostream>
using namespace std;

namespace our
{

    // The crashing system is responsible for checking if the car has crashed with any obstacle.
    class BigObstaclesSystem {

        nlohmann::json generateBigObstacle(glm::vec3 startingPosition, glm::vec3 startingLinearVelocity,
																					glm::vec3 startingAngularVelocity,
																					glm::vec2 Xrange = glm::vec2(0, 0),
																					glm::vec2 Yrange = glm::vec2(0, 0),
																					glm::vec2 Zrange= glm::vec2(0, 0),
																					glm::vec3 rotation = glm::vec3(0, 0, 0)) {
            return {
                    {"name", "obstacles"},
                    {"position", {startingPosition.x, startingPosition.y, startingPosition.z}},
                    {"rotation", {rotation.x, rotation.y, rotation.z}},
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
																																{"angularVelocity", {startingAngularVelocity.x, startingAngularVelocity.y, startingAngularVelocity.z}},
																																{"movementRangeX", {Xrange.x, Xrange.y}},
																																{"movementRangeY", {Yrange.x, Yrange.y}},
																																{"movementRangeZ", {Zrange.x, Zrange.y}}
																														}
																												})}
            };
        }

        float generateRandomNumber(int from, int to){
            float random = static_cast<float>(rand()) / RAND_MAX * (to - from) + from;
            return random;
        }

        vector<glm::vec2> horizontalPoints = {
						glm::vec2(generateRandomNumber(-40, 40), -5), // middle
						glm::vec2(generateRandomNumber(-40, 40), -47), // left
						glm::vec2(generateRandomNumber(-40, 40), 34), // right
        }, 

				verticalPoints = {
						glm::vec2(52, generateRandomNumber(-40, 40)), // up
						glm::vec2(-54, generateRandomNumber(-40, 40)), // down
        };

    public:

        int getNumOfObstacles() {
            return horizontalPoints.size() + verticalPoints.size();
        }

        void initialize(World* world) {
            srand(static_cast<unsigned int>(time(nullptr))); // Call srand only once at the beginning

            for(glm::vec2 point : horizontalPoints){
                Entity* obstacle = world->add();
                obstacle->deserialize(
                        generateBigObstacle(glm::vec3(point.x, 3.5, point.y),
                                            glm::vec3(1, 0, 0),
                                            glm::vec3(0, 0, 0),
                                            glm::vec2(-55, 60),
                                            glm::vec2(0, 0),
                                            glm::vec2(0, 0),
                                            glm::vec3(0, 90, 0)
                        ));
            }

            for(glm::vec2 point : verticalPoints){
                Entity* obstacle = world->add();
                obstacle->deserialize(
                        generateBigObstacle(glm::vec3(point.x, 3.5, point.y),
                                            glm::vec3(0, 0, 1),
                                            glm::vec3(0, 0, 0),
                                            glm::vec2(0, 0),
                                            glm::vec2(0, 0),
                                            glm::vec2(-48, 43),
                                            glm::vec3(0, 0, 0)
                        ));
            }
        }
    };
}
