#pragma once

#include <glm/glm.hpp>
#include <ecs/world.hpp>
#include <systems/event-handler.hpp>
#include <chrono>

using namespace std;

#include <chrono>

#define decreasedTime 5000
#define decreasedEnergy 1

namespace our
{

    // The crashing system is responsible for checking if the car has crashed with any obstacle.
    class EnergySystem {
        // Save the car entity
        int energy = 1000;
        std::chrono::high_resolution_clock::time_point lastTime;
        our::EventHandlerSystem * eventHandler;
				vector<Entity*> energyBars;

        void increaseEnergy(int amount){
            energy = max(energy + amount , 100);
        }

        void decreaseEnergy(int amount){
            energy -= amount;
            if (energy <= 0) eventHandler->loseGame();
        }

				nlohmann::json generateEnergybar(glm::vec3 position, std::string name) {
				return {
								{"name", name},
								{"position", {position.x, position.y, position.z}},
								{"rotation", {0, 0, 0}},
								{"scale", {0.05, 0.35, 1}},
								{"components", nlohmann::json::array({
									{
										{"type", "Mesh Renderer"},
										{"mesh", "plane"},
										{"material", "energy_level"}
									}
								})}
							};
				}

				void constructEnergybar(Entity* parent, float start, float end, float step) {
						int barIndex = 1;
						for (float x = start; x <= end; x += step) {
								Entity* bar = parent->getWorld()->add();
								energyBars.push_back(bar);
								bar->parent = parent;
								bar->deserialize(generateEnergybar(glm::vec3(x, 3.3, -4), "energybar-" + std::to_string(barIndex++)));
								printf("bar: %s\n", bar->name.c_str());
						}
				}
    public:
        void initialize(World* world, EventHandlerSystem* eventHandler){
            lastTime = std::chrono::high_resolution_clock::now();
            this->eventHandler = eventHandler;
            energy = 100;

						// Construct the energy bar
						for (auto entity : world->getEntities()) {
							if (entity->name == "camera") {
									constructEnergybar(entity, 1.73, 5.115, 0.1);
							}
						}
        }

        int getEnergy(){
            return energy;
        }

        void batteryCrash(){
            increaseEnergy(10);
        }

        void obstacleCrash(){
            decreaseEnergy(0);
        }

        void buildingCrash(){
            decreaseEnergy(0);
        }

        void update() {
            auto currentTime = std::chrono::high_resolution_clock::now();
            auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count();
            printf("energy: %d\n", energy);
            if (elapsedTime >= decreasedTime) {
                lastTime = currentTime;
//                energy -= decreasedEnergy; // Decrease energy every 50ms
                if (energy <= 0) eventHandler->loseGame();
            }
        }
    };
}
