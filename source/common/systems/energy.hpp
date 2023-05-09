#pragma once

#include <glm/glm.hpp>
#include <systems/event-handler.hpp>

using namespace std;

#include <chrono>

#define decreasedTime 250
#define decreasedEnergy 1

namespace our
{

    // The crashing system is responsible for checking if the car has crashed with any obstacle.
    class EnergySystem {
        // Save the car entity
        int energy = 1000;
        std::chrono::high_resolution_clock::time_point lastTime;
        our::EventHandlerSystem * eventHandler;


        void increaseEnergy(int amount){
            energy = max(amount + amount , 100);
        }

        void decreaseEnergy(int amount){
            energy = max(energy - amount, 0);
            if (energy <= 0) eventHandler->loseGame();
        }

    public:
        void startTimer(EventHandlerSystem* eventHandler){
            lastTime = std::chrono::high_resolution_clock::now();
            this->eventHandler = eventHandler;
            energy = 100;
        }

        int getEnergy(){
            return energy;
        }

        void batteryCrash(){
            increaseEnergy(10);
        }

        void obstacleCrash(){
            decreaseEnergy(20);
        }

        void buildingCrash(){
            decreaseEnergy(5);
        }



        void update() {
            auto currentTime = std::chrono::high_resolution_clock::now();
            auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count();
            printf("energy: %d\n", energy);
            if (elapsedTime >= decreasedTime) {
                lastTime = currentTime;
                energy -= decreasedEnergy; // Decrease energy every 50ms
                if (energy <= 0) eventHandler->loseGame();
            }
        }


    };

}
