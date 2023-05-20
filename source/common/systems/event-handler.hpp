#pragma once

#include "../ecs/world.hpp"
#include "application.hpp"
#include <systems/knife.hpp>
#include <systems/big-obstacles.hpp>
#include <systems/crashing.hpp>

namespace our
{
    // The crashing system is responsible for checking if the car has crashed with any obstacle.
    class EventHandlerSystem {
    Application * app;
    KnifeSystem* knife;
    int targetDelivers;
    int numOfObstacles;
    bool haveDeliver = 0, haveKnife = 0;

    public:
        void startHandler(Application* app, int targetDelivers, BigObstaclesSystem* obstacle){
            this->app = app;
            this->targetDelivers = targetDelivers;
            our::BigObstaclesSystem* obstacleSystem = obstacle;
            this->numOfObstacles = obstacleSystem->getNumOfObstacles();
        }

        void collectDeliver(){
            haveDeliver = 1;
        }

        void collectKnife() {
            haveKnife = 1;
        }

        bool isCarryDeliver(){
            return haveDeliver;
        }

        bool carryingKnife() {
            return haveKnife;
        }

        void deliverDelivery(World* world){
            if (haveDeliver) {
                haveDeliver = 0;
                targetDelivers--;

                if(targetDelivers == 0){
                    printf("Deliveries done\n");
                    knife = new our::KnifeSystem;
                    knife->initialize(world);
                    printf("Knife initialized\n");
                }
            }
        }

        void addKnife(Entity* entity, World* world) {
            knife->addKnifeOnCar();
			knife->removeKnife(entity, world);
        }

        void killMonkey() {
            numOfObstacles--;
            if (numOfObstacles == 0) {
                winGame();
            }
        }

        void loseGame(){
            app->changeState("lose");
        }

        void winGame(){
            app->changeState("win");
        }

    };

}
