#pragma once

#include "../ecs/world.hpp"

#include <glm/glm.hpp>
#include <systems/event-handler.hpp>
#include <systems/energy.hpp>
#include <systems/delivery.hpp>
#include <systems/knife.hpp>
#include <systems/movement.hpp>
#include <systems/car-movement.hpp>
#include <systems/battery-handler.hpp>
#include <systems/sound.hpp>


#include <iostream>
using namespace std;

#define CrashTime 500

namespace our
{
    // The crashing system is responsible for checking if the car has crashed with any other entity.
	// And act accordingly depending on what entity the car collided with
    class CrashingSystem {
		Sound test = Sound("assets/sounds/Song.mp3", true);
		Sound monkeys = Sound("assets/sounds/Monkey-Noises.mp3", false);
		Sound gorillas = Sound("assets/sounds/Gorilla-SoundBible.com-1576451741.mp3", false);
		Sound arrived=Sound("assets/sounds/destination.m4a",false);
		// Save the car entity
        Entity *car;
        EventHandlerSystem *events;
        EnergySystem *energy;
        DeliverySystem *delivery;
        BatterySystem *batterySystem;
        std::chrono::high_resolution_clock::time_point lastCrashTime, currentTime;
        CarMovementSystem* carMovement;

        void setCar(World* world) {
            // For each entity in the world
			// Check if it's name is "car" and then set the car entity we have
            for (auto entity : world->getEntities()){
                if (entity->name == "car"){
					car = entity;
					break;
                }
            }
        }

		bool crash(Entity *object1, Entity *object2, std::string type) {
			// Get the car's max position, min position, current position, and size
			glm::vec3 object1Position = object1->localTransform.position;
			glm::vec3 object1Size = object1->localTransform.scale;
			glm::vec3 object1Max = object1Position + object1Size;
			glm::vec3 object1Min = object1Position - object1Size;

			// Get the collider's position
			glm::vec3 object2Position = object2->localTransform.position;

			glm::vec3 object2Size = object2->localTransform.scale;
			
			// Set the object size
			if (type == "destination")
				object2Size = {1.5, 10.0, 1.5};
			else if (type == "pole")
				object2Size = {0.2, 0.5, 0.2};
			else if (type == "building")
				object2Size += glm::vec3(0.2, 0, 0.2);

			// Get the collider's max and min positions
			glm::vec3 object2Max = object2Position + object2Size;
			glm::vec3 object2Min = object2Position - object2Size;

			// If the car is in the range of the obstacle, that's a collision!
			return (object1Max.x >= object2Min.x && object1Min.x <= object2Max.x &&
					object1Max.y >= object2Min.y && object1Min.y <= object2Max.y &&
					object1Max.z >= object2Min.z && object1Min.z <= object2Max.z);
		}

     	public:
			void initialize(World* world, EventHandlerSystem* events, EnergySystem* energy, DeliverySystem* delivery, BatterySystem* battery, CarMovementSystem* carMovement) {
				this->events = events;
				this->energy = energy;
				this->delivery = delivery;
				this->batterySystem = battery;
				this->carMovement = carMovement;
				setCar(world);

				// Prevent the car from crashing at the start
				lastCrashTime = std::chrono::high_resolution_clock::now();
				test.changeVolume(25);
				test.play();
				printf("Sound played\n");
			}

			bool checkTime() {
				// Check for the crashing time threshold
				auto currentTime = std::chrono::high_resolution_clock::now();
				auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastCrashTime).count();
				if (elapsedTime < CrashTime)
					return false;

				// Set the last crashing time as the current time
				lastCrashTime = currentTime;

				return true;
			}

			bool update(World* world,std::string&postProcessIndicator){

				bool applyPostProcess=false;

				// For each entity in the world
				for (auto entity : world->getEntities()) {

					if (entity->name == "car")
						continue;

					// Delivery Pick-up
					if (entity->name == "delivery" && crash(car, entity, "delivery")) {
						if (!events->isCarryDeliver()) {
							monkeys.play();
							events->collectDeliver();
							delivery->addDeliveryOnCar();
							delivery->removeDelivery(entity, world);
						}
					}
					// Knife Pick-up
					else if (entity->name == "knife" && crash(car, entity, "knife")) {
						printf("Knife Pick-up\n");
						if (!events->carryingKnife()) {
							printf("Collecting Knife\n");
							events->collectKnife();
							events->addKnife(entity, world);
						}
					}
					// Arrived at the destination
					else if (entity->name == "arrow" && crash(car, entity, "destination")) {
						if (!checkTime())
							continue;
						if (events->isCarryDeliver()){
							arrived.play();
							energy->deliverMonkey();
						}
						events->deliverDelivery(world);
						delivery->removeDeliveryOnCar();
					}
					// Hit a street pole
					else if (entity->name == "StreetPole" && crash(car, entity, "pole")) {
						carMovement->poleCrash();
					}
					// Getting energy from a battery
					else if (entity->name == "battery" && crash(car, entity, "battery")) {
						energy->batteryCrash();
						batterySystem->takeBattery(entity);
						applyPostProcess=true;
						postProcessIndicator="battery";
					}
					// Crashing with a building
					else if (entity->name == "building" && crash(car, entity, "building")) {
						if (!checkTime())
							continue;
						energy->buildingCrash();
						applyPostProcess=true;
						postProcessIndicator="obstacle";
					}
					// Crashing with an obstacle
					else if (entity->name == "obstacles" && crash(car, entity, "obstacle")) {
						if (!checkTime())
							continue;

						if (events->carryingKnife())
							events->killMonkey();
						else {
							gorillas.play();
							energy->obstacleCrash();
							carMovement->decreaseCarSpeed();
							applyPostProcess=true;
							postProcessIndicator="obstacle";
						}
					}
				}
				return applyPostProcess;
			}
    };
}
