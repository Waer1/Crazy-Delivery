#pragma once

#include "../ecs/world.hpp"
#include <glm/glm.hpp>

#include <iostream>
using namespace std;

namespace our
{

    class LightSystem {
				vector<glm::vec3> spotlightPositions; // Spotlight Positions
        int numOfSpotLights = 26;

		nlohmann::json generatePole(glm::vec3 position, std::string name) {
			return {
				{"name", name},
				{"position", {position.x, position.y, position.z}},
				{"rotation", {0, 0, 0}},
				{"scale", {3.5, 1.65, 3.5}},
				{"components", nlohmann::json::array({
					{
						{"type", "Mesh Renderer"},
						{"mesh", "street-light"},
						{"material", "street-light"}
					}
				})}
			};
		}

		nlohmann::json generateLights(glm::vec3 position, std::string name) {
			return {
				{"name", name},
				{"position", {position.x, position.y + 5, position.z}},
				{"components", nlohmann::json::array({
					{
						{"type", "Light"},
						{"lightType", "spot"},
                        {"direction", {0, -1, 0}},
                        {"diffuse", {0.35, 0.35, 0.35}},
                        {"specular", {1, 1, 1}},
                        {"attenuation", {0.0095, 0.0095, 0.0095}},
                        {"cone_angles", {1.57, 1.9}}
					}
				})}
			};
		}

    public:
        void initialize(World* world) {
						// Get the delivery points from the world
						Entity* lightPoints;
						for(auto entity : world->getEntities()){
								if(entity->name == "streetLight-position"){
										lightPoints = entity;
										break;
								}
						}

						// Get the list of poistion point components
						auto positionComponents = lightPoints->getComponents<PositionPointComponent>();
						for (auto positionComponent : positionComponents) {
								spotlightPositions.push_back(positionComponent->position);
						}

            for (int i = 0; i < numOfSpotLights; i++) {
                glm::vec3 lightPosition = spotlightPositions[i];
                
                Entity* spotLight = world->add();
                spotLight->deserialize(generatePole(lightPosition, "StreetPole"));

                Entity* lightEntity = world->add();
                lightEntity->deserialize(generateLights(lightPosition, "SpotLight"));
            }
        }
    };
}
