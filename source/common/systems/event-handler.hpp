#pragma once

#include "application.hpp"

namespace our
{

    // The crashing system is responsible for checking if the car has crashed with any obstacle.
    class EventHandlerSystem {
        Application * app;

    public:
        void startHandler(Application* app){
            this->app = app;
        }

        void loseGame(){
            app->changeState("lose");
        }

        void winGame(){
            app->changeState("win");
        }
    };

}
