#pragma once

#include "../ecs/world.hpp"
#include "application.hpp"
#include <systems/knife.hpp>
#include <systems/big-obstacles.hpp>
#include <systems/crashing.hpp>

#include <chrono>

using namespace std;
#define clickDelay 200

namespace our
{
    // The crashing system is responsible for checking if the car has crashed with any obstacle.
    class EventHandlerSystem {
    Application * app;
    KnifeSystem* knife;
    Entity* frontLight, *backLight;

    int targetDelivers;
    int numOfObstacles;
    bool haveDeliver = 0, haveKnife = 0;

    std::chrono::high_resolution_clock::time_point lastTime = std::chrono::high_resolution_clock::now();
    bool clickButton = true;

    // Sounds
    Sound peep = Sound("assets/sounds/Horn.mp3", false);

    public:
        void initialize(Application* app, int targetDelivers, BigObstaclesSystem* obstacle, World* world){
            this->app = app;
            this->targetDelivers = targetDelivers;
            our::BigObstaclesSystem* obstacleSystem = obstacle;
            this->numOfObstacles = obstacleSystem->getNumOfObstacles();

            // Get front and back lights
            for (auto entity : world->getEntities()){
				if (entity->name == "frontLight"){
                    frontLight = entity;
                    break;
                }
            }
            for (auto entity : world->getEntities()){
                if (entity->name == "backLight") {
                    backLight = entity;
                    break;
                }
            }
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
                    knife = new our::KnifeSystem;
                    knife->initialize(world);
                }
            }
        }

        void addKnife(Entity* entity, World* world) {
            knife->addKnifeOnCar();
			knife->removeKnife(entity, world);
        }

        void killMonkey(Entity* entity, World* world) {
            world->markForRemoval(entity);
            world->deleteMarkedEntities();
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

        void update() {
            auto currentTime = std::chrono::high_resolution_clock::now();
			auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count();
			if (elapsedTime > clickDelay) {
				clickButton = true;
			}

            // Car Honk
            if (clickButton && app->getKeyboard().isPressed(GLFW_KEY_SPACE))
                peep.play();
            
            // Car Lights
            if (clickButton && app->getKeyboard().isPressed(GLFW_KEY_L)) {
				lastTime = std::chrono::high_resolution_clock::now();
                clickButton = false;

                if (frontLight->localTransform.scale == glm::vec3(0, 0, 0)) {
                    frontLight->localTransform.scale = glm::vec3(1, 1, 1);
                    backLight->localTransform.scale = glm::vec3(1, 1, 1);
                }
                else {
                    frontLight->localTransform.scale = glm::vec3(0, 0, 0);
                    backLight->localTransform.scale = glm::vec3(0, 0, 0);
                }
            }
        }

        void destroy() {
            if (knife)
                delete knife;
        }

    };

}
