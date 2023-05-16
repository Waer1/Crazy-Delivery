#pragma once

#include <glm/glm.hpp>
#include <ecs/world.hpp>
#include <systems/event-handler.hpp>
#include <chrono>
#include <iostream>

using namespace std;

#define decreasedTime 400
#define decreasedEnergy 1

namespace our
{

    // The crashing system is responsible for checking if the car has crashed with any obstacle.
    class EnergySystem
    {
        bool crashed = false;

        Entity* getCar(World* world) {
            Entity* car = nullptr;
            // For each entity in the world
            for(auto entity : world->getEntities()){
                if(entity->name == "car"){
                    car = entity;
					break;
                }
            }
            return car;
        }

        Entity* getCamera(World* world) {
            Entity* camera = nullptr;
            // For each entity in the world
            for(auto entity : world->getEntities()){
                if(entity->name == "camera"){
                    camera = entity;
					break;
                }
            }
            return camera;
        }
        // Save the car entity
        int energy = 100;
        std::chrono::high_resolution_clock::time_point lastTime;
        our::EventHandlerSystem *eventHandler;
        vector<Entity *> energyBars;

        void handleEnergyBars(int energy)
        {
            float percentage = ((float)energy / 100) * 100; // calculate the percentage of energy
            for (int i = 0; i < energyBars.size(); i++)
            {
                if (percentage >= i * 100.0 / energyBars.size())
                {
                    energyBars[i]->localTransform.scale.x = 0.0; // show the energy bar
                }
                else
                {
                    energyBars[i]->localTransform.scale.x = 0.0; // hide the energy bar
                }
            }
        }

        void increaseEnergy(int amount)
        {
            energy = min(energy + amount, 100);
        }

        void decreaseEnergy(int amount)
        {
            energy -= amount;
            if (energy <= 0)
                eventHandler->loseGame();
        }

        nlohmann::json generateEnergybar(glm::vec3 position, std::string name)
        {
            return {
                {"name", name},
                {"position", {position.x, position.y, position.z}},
                {"rotation", {0, 0, 0}},
                {"scale", {0.05, 0.35, 1}},
                {"components", nlohmann::json::array({{{"type", "Mesh Renderer"},
                                                       {"mesh", "plane"},
                                                       {"material", "energy_level"}}})}};
        }

        void constructEnergybar(Entity *parent, float start, float end, float step)
        {
            int barIndex = 1;
            for (float x = start; x <= end; x += step)
            {
                Entity *bar = parent->getWorld()->add();
                energyBars.push_back(bar);
                bar->parent = parent;
                bar->deserialize(generateEnergybar(glm::vec3(x, 3.3, -4), "energybar-" + std::to_string(barIndex++)));
                printf("bar: %s\n", bar->name.c_str());
            }
        }

    public:
        void initialize(World *world, EventHandlerSystem *eventHandler)
        {
            lastTime = std::chrono::high_resolution_clock::now();
            this->eventHandler = eventHandler;
            energy = 100;

            // Construct the energy bar
            for (auto entity : world->getEntities())
            {
                if (entity->name == "camera")
                {
                    constructEnergybar(entity, 1.73, 5.115, 0.1);
                }
            }
        }

        int getEnergy()
        {
            return energy;
        }

        void batteryCrash()
        {
            increaseEnergy(10);
        }

        void obstacleCrash()
        {
            decreaseEnergy(0);
        }

        void buildingCrash()
        {
            decreaseEnergy(0);
            crashed = true;
        }

        void update(World* world)
        {
            auto currentTime = std::chrono::high_resolution_clock::now();
            auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count();
            if (elapsedTime >= decreasedTime)
            {
                lastTime = currentTime;
                energy -= decreasedEnergy; // Decrease energy every 50ms
                handleEnergyBars(energy);
                if (energy <= 0)
                    eventHandler->loseGame();
                // Check if the car crashed with a building or an obstacle
                // We'll return it to the starting point if this happened
                if (crashed) {
                    Entity* car = getCar(world);
                    if (car) {
                        printf("---Crashed---\n");
                        car->localTransform.position = glm::vec3(-52, 1.12, 43);
                        crashed = false;
                    }
                }
            }
        }
    };
}
