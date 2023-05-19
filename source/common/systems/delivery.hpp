#pragma once

#include "../ecs/world.hpp"
#include "../components/position-point.hpp"
#include <glm/glm.hpp>

#include <iostream>
using namespace std;

namespace our
{

    // The delivery system is responsible for creating the deliveries and removing them when they are delivered.
    class DeliverySystem {
        Entity *deliveryOnCar;
				vector<glm::vec3> pickUpPoints; // points that we will choose randomly from them

        void getDeliveryOnCar(World* world) {
            // For each entity in the world
            for(auto entity : world->getEntities()){
                if(entity->name == "delivery-on-car"){
					deliveryOnCar = entity;
					break;
                }
            }
        }

				nlohmann::json generateDelivery(glm::vec3 position, std::string name) {
					return {
						{"name", name},
						{"position", {position.x, position.y, position.z}},
						{"rotation", {0, 0, 0}},
						{"scale", {0.5, 0.5, 0.5}},
						{"components", nlohmann::json::array({
							{
								{"type", "Mesh Renderer"},
								{"mesh", "monkey"},
								{"material", "monkey"}
							},
							{
								{"type", "Movement"},
								{"angularVelocity", {0, 30, 0}}
							}
						})}
					};
				}

    public:
        void initialize(World* world, int numOfDeliveries) {
						// Get the delivery points from the world
						Entity* deliveryPoints;
						for(auto entity : world->getEntities()){
								if(entity->name == "deliveries"){
										deliveryPoints = entity;
										break;
								}
						}

						// Get the list of poistion point components
						auto positionComponents = deliveryPoints->getComponents<PositionPointComponent>();
						for (auto positionComponent : positionComponents) {
								pickUpPoints.push_back(positionComponent->position);
						}

            getDeliveryOnCar(world);

            removeDeliveryOnCar();

            // Pick a random points from the pickUpPoints
            for (int i = 0; i < numOfDeliveries; i++) {
                int randomIndex = rand() % pickUpPoints.size();
                glm::vec3 randomPoint = pickUpPoints[randomIndex];
                pickUpPoints.erase(pickUpPoints.begin() + randomIndex);

                Entity* delivery = world->add();
                delivery->deserialize(generateDelivery(randomPoint, "delivery"));
            }
        }

				void removeDeliveryOnCar() {
					deliveryOnCar->localTransform.scale = glm::vec3(0, 0, 0);
				}

				void addDeliveryOnCar() {
					deliveryOnCar->localTransform.scale = glm::vec3(0.3, 0.3, 0.3);
				}

				void removeDelivery(Entity* delivery, World* world) {
								world->markForRemoval(delivery);
								world->deleteMarkedEntities();
				}
    };
}
