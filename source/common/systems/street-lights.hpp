#pragma once

#include "../ecs/world.hpp"
#include <glm/glm.hpp>

#include <iostream>
using namespace std;

namespace our
{

    class LightSystem {
		vector<glm::vec2> spotlightPositions; // Spotlight Positions
        int numOfSpotLights = 26;

		nlohmann::json generatePole(glm::vec2 position, std::string name) {
			return {
				{"name", name},
				{"position", {position.x, 1.12, position.y}},
				{"rotation", {0, 0, 0}},
				{"scale", {1, 1, 1}},
				{"components", nlohmann::json::array({
					{
						{"type", "Mesh Renderer"},
						{"mesh", "street-light"},
						{"material", "street-light"}
					}
				})}
			};
		}

        nlohmann::json generateLights(glm::vec2 position, std::string name) {
			return {
				{"name", name},
				{"position", {position.x, 5, position.y}},
				{"components", nlohmann::json::array({
					{
						{"type", "Light"},
						{"lightType", "spot"},
                        {"direction", {0, -1, 0}},
                        {"diffuse", {0.2, 0.2, 0.2}},
                        {"specular", {1, 1, 1}},
                        {"attenuation", {0.0095, 0.0095, 0.0095}},
                        {"cone_angles", {1.57, 2}}
					}
				})}
			};
		}

    public:
        void initialize(World* world) {
            // Cross Road Lights
            spotlightPositions.push_back(glm::vec2(-35, -11));
            spotlightPositions.push_back(glm::vec2(0, -11));
            spotlightPositions.push_back(glm::vec2(40, -11));

            spotlightPositions.push_back(glm::vec2(-40, -36));
            spotlightPositions.push_back(glm::vec2(0, -36));
            spotlightPositions.push_back(glm::vec2(40, -36));

            spotlightPositions.push_back(glm::vec2(-40, -52.7));
            spotlightPositions.push_back(glm::vec2(0, -52.7));
            spotlightPositions.push_back(glm::vec2(40, -52.7));

            spotlightPositions.push_back(glm::vec2(-40, 6.4));
            spotlightPositions.push_back(glm::vec2(0, 6.4));
            spotlightPositions.push_back(glm::vec2(40, 6.4));

            spotlightPositions.push_back(glm::vec2(-40, 29.8));
            spotlightPositions.push_back(glm::vec2(0, 29.8));
            spotlightPositions.push_back(glm::vec2(40, 29.8));

            spotlightPositions.push_back(glm::vec2(-40, 46.5));
            spotlightPositions.push_back(glm::vec2(0, 46.5));
            spotlightPositions.push_back(glm::vec2(40, 46.5));

            // Side Road Lights
            spotlightPositions.push_back(glm::vec2(-59.2, 40));
            spotlightPositions.push_back(glm::vec2(-59.2, 7));
            spotlightPositions.push_back(glm::vec2(-59.2, -17));
            spotlightPositions.push_back(glm::vec2(-59.2, -47));

            spotlightPositions.push_back(glm::vec2(65.5, 40));
            spotlightPositions.push_back(glm::vec2(65.5, 10));
            spotlightPositions.push_back(glm::vec2(65.5, -12));
            spotlightPositions.push_back(glm::vec2(65.5, -34));

            for (int i = 0; i < numOfSpotLights; i++) {
                glm::vec2 lightPosition = spotlightPositions[i];
                
                Entity* spotLight = world->add();
                spotLight->deserialize(generatePole(lightPosition, "StreetPole"));

                Entity* lightEntity = world->add();
                lightEntity->deserialize(generateLights(lightPosition, "SpotLight"));
            }
        }
    };
}
