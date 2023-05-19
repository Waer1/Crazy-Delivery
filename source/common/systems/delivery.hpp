#pragma once

#include "../ecs/world.hpp"
#include <glm/glm.hpp>

#include <iostream>
using namespace std;

namespace our
{

    // The delivery system is responsible for creating the deliveries and removing them when they are delivered.
    class DeliverySystem {
        Entity *deliveryOnCar;
		vector<glm::vec2> pickUpPoints; // points that we will choose randomly from them

        void getDeliveryOnCar(World* world) {
            // For each entity in the world
            for(auto entity : world->getEntities()){
                if(entity->name == "delivery-on-car"){
					deliveryOnCar = entity;
					break;
                }
            }
        }

        //Function is responsible for generating the delivery objects in the real map
		nlohmann::json generateDelivery(glm::vec2 position, std::string name) {
			return {
				{"name", name},
				{"position", {position.x, 1.8, position.y}},
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
            //Selecting the possible values that we will choose from them randomly to be delivered
            pickUpPoints.push_back(glm::vec2(30, -3.75));
            pickUpPoints.push_back(glm::vec2(-30, 1));
            pickUpPoints.push_back(glm::vec2(-50, -30));
            pickUpPoints.push_back(glm::vec2(-54, 25));
            pickUpPoints.push_back(glm::vec2(-20, 40));
            pickUpPoints.push_back(glm::vec2(60, 37));
            pickUpPoints.push_back(glm::vec2(55, 10));
            //Getting the delivery on car object
            getDeliveryOnCar(world);
            //Then make it with scale 0 just not to be drawn on the screen
            removeDeliveryOnCar();

            // Pick a random points from the pickUpPoints
            for (int i = 0; i < numOfDeliveries; i++) {
                //Selecting a random number from the array that contains all possible points to pickup a delivery
                int randomIndex = rand() % pickUpPoints.size();
                glm::vec2 randomPoint = pickUpPoints[randomIndex];
                //removing that random point from the array just not to select it again
                pickUpPoints.erase(pickUpPoints.begin() + randomIndex);
                //Adding this chosen point to be the point of generation to the delivery
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
