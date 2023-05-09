#pragma once

#include "application.hpp"

namespace our
{

    // The crashing system is responsible for checking if the car has crashed with any obstacle.
    class EventHandlerSystem {
    Application * app;
    int targetDelivers;
    bool haveDeliver = 0;

    public:
        void startHandler(Application* app, int _targetDelivers){
            this->app = app;
            this->targetDelivers = _targetDelivers;
        }

        void collectDeliver(){
            haveDeliver = 1;
        }

        bool isCarryDeliver(){
            return haveDeliver;
        }

        void deliverDelivery(){
            if(haveDeliver){
                haveDeliver=0;
                targetDelivers--;
                printf("%d" ,targetDelivers);

                if(targetDelivers == 0){
                    winGame();
                }
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
