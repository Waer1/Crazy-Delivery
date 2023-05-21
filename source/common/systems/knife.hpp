#pragma once

#include "../ecs/world.hpp"
#include "../components/position-point.hpp"
#include <glm/glm.hpp>

#include <iostream>
using namespace std;

namespace our
{
    // The knife appears when all monkeys have been delivered
    class KnifeSystem {
        Entity *knifeOnCar;
        Entity *knife;
		vector<glm::vec3> knifePoints;

        void getKnifeOnCar(World* world) {
            // For each entity in the world
            for(auto entity : world->getEntities()){
                if(entity->name == "knife-on-car") {
					knifeOnCar = entity;
					break;
                }
            }
        }

        nlohmann::json generateKnife(glm::vec3 position, std::string name) {
            return {
                {"name", name},
                {"position", {position.x, position.y, position.z}},
                {"rotation", {30, 0, 0}},
                {"scale", {0, 0, 0}},
                {"components", nlohmann::json::array({
                    {
                        {"type", "Mesh Renderer"},
                        {"mesh", "knife"},
                        {"material", "knife"}
                    },
					{
						{"type", "Movement"},
						{"angularVelocity", {0, 60, 0}}
					}
                })}
            };
        }

        public:
            void initialize(World* world) {
                // Get the random knife points from the world
                Entity* KnifePoints;
                for (auto entity : world->getEntities()){
                    if(entity->name == "knives"){
                        KnifePoints = entity;
                        break;
                    }
                }

                // Get the list of poistion point components
                auto positionComponents = KnifePoints->getComponents<PositionPointComponent>();
                for (auto positionComponent : positionComponents) {
                    knifePoints.push_back(positionComponent->position);
                }

                getKnifeOnCar(world);

                // Then make it with scale 0 just not to be drawn on the screen
                removeKnifeOnCar();

                // Pick a random point from the knifePoints
                int randomIndex = rand() % knifePoints.size();
                glm::vec3 randomPoint = knifePoints[randomIndex];

                // Adding the knife to the world
                knife = world->add();
                knife->deserialize(generateKnife(randomPoint, "knife"));
            }

            void removeKnifeOnCar() {
                knifeOnCar->localTransform.scale = glm::vec3(0, 0, 0);
            }

            void addKnifeOnCar() {
                knifeOnCar->localTransform.scale = glm::vec3(10, 10, 10);
            }

            void showKnife() {
                knife->localTransform.scale = glm::vec3(15, 15, 15);
            }

            void removeKnife(Entity* knife, World* world) {
                world->markForRemoval(knife);
                world->deleteMarkedEntities();
            }
    };
}
