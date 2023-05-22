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
    Entity* knifeOnCar;
    bool moveKnifeForward=false;
    bool moveKnifeBackward=false;
    float knifeFrontSpeed=0.03,knifeBackSpeed=0.05;

    int targetDelivers;
    int numOfObstacles;
    bool haveDeliver = 0, haveKnife = 0;

    std::chrono::high_resolution_clock::time_point lastTime = std::chrono::high_resolution_clock::now();
    bool clickButton = true;

    // Sounds
    Sound peep = Sound("assets/sounds/Horn.mp3", false);

    public:
        void initialize(Application* app, int targetDelivers, BigObstaclesSystem* obstacle, KnifeSystem* knifeSystem, World* world){
            this->app = app;
            this->targetDelivers = targetDelivers;
            this->numOfObstacles = obstacle->getNumOfObstacles();
            this->knife = knifeSystem;

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
            for (auto entity : world->getEntities()){
				if (entity->name == "knife-on-car"){
                    knifeOnCar = entity;
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

        int getRemainingDeliveries() {
            return targetDelivers;
        }

        void deliverDelivery(World* world){
            if (haveDeliver) {
                haveDeliver = 0;
                targetDelivers--;

                if(targetDelivers == 0){
                    knife->showKnife();
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
            if (numOfObstacles < 2) {
                winGame();
            }
        }

        void loseGame(){
            app->changeState("lose");
        }

        void winGame(){
            app->changeState("win");
        }

        void update(World* world, float deltaTime) {
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

            // Throwing knife
            if (knifeOnCar && knifeOnCar->localTransform.scale != glm::vec3(0, 0, 0)){

                float lastXPos=knifeOnCar->localTransform.position.x;
                float lastYPos=knifeOnCar->localTransform.position.y;
                glm::mat4 knifeMatrix = knifeOnCar->localTransform.toMat4();
                //getting the value of the front vector which is vector in Z direction multiplied by the matrix calculated before
                glm::vec3 front = glm::vec3(knifeMatrix * glm::vec4(0, 0, 1, 0));


                if (clickButton && app->getKeyboard().isPressed(GLFW_KEY_K)){
                    if(!moveKnifeBackward) {
                        moveKnifeForward=true;
                    }
                }
                //If we enabled moving forward then we need to increase the value of Z to move forward
                if(moveKnifeForward){
                    knifeOnCar->localTransform.position += front*(float(0.03));
                }

                if(knifeOnCar->localTransform.position.z>=7){
                    moveKnifeForward=false;
                    moveKnifeBackward=true;
                    knifeOnCar->localTransform.position.z=7;
                }

                if(knifeOnCar->localTransform.position.z<=0){
                    moveKnifeBackward=false;
                    moveKnifeForward=false;
                    knifeOnCar->localTransform.position.z=0;
                }
                //If we reached our maximum position then we need to enable moving backward then returning back to initial position
                if(moveKnifeBackward){
                    knifeOnCar->localTransform.position -= front*(float(0.05));
                }

                knifeOnCar->localTransform.position.x=lastXPos;
                knifeOnCar->localTransform.position.y=lastYPos;
            }
        }
    };
}
