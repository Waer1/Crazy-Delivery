#pragma once

#include <application.hpp>

#include <ecs/world.hpp>
#include <systems/forward-renderer.hpp>
#include <systems/movement.hpp>
#include <systems/crashing.hpp>
#include <systems/energy.hpp>
#include <systems/event-handler.hpp>
#include <systems/delivery.hpp>
#include <systems/street-lights.hpp>
#include <systems/big-obstacles.hpp>
#include <systems/car-movement.hpp>
#include <systems/free-camera-controller.hpp>
#include <systems/battery-handler.hpp>
#include <systems/radio.hpp>
#include <asset-loader.hpp>
#include <string>


// This state shows how to use the ECS framework and deserialization.
class Playstate: public our::State {

    our::World* world;
    our::ForwardRenderer* renderer;
    our::CarMovementSystem* carController;
    our::MovementSystem* movementSystem;
    our::CrashingSystem* crashingSystem;
    our::EnergySystem* energySystem;
    our::EventHandlerSystem* eventHandlerSystem;
    our::DeliverySystem* deliverySystem;
    our::LightSystem* lightSystem;
    our::BigObstaclesSystem* bigObstaclesSystem;
    our::FreeCameraControllerSystem* cameraController;
    our::BatterySystem* batteryHandlerSystem;
    our::RadioSystem* radioSystem;

    int timer=0;
    std::string lastPostProcessEvent="";

    void onInitialize() override {
        world = new our::World;
        renderer = new our::ForwardRenderer;
        carController = new our::CarMovementSystem;
        movementSystem = new our::MovementSystem;
        crashingSystem = new our::CrashingSystem;
        energySystem = new our::EnergySystem;
        eventHandlerSystem = new our::EventHandlerSystem;
        deliverySystem = new our::DeliverySystem;
        lightSystem = new our::LightSystem;
        bigObstaclesSystem = new our::BigObstaclesSystem;
        cameraController = new our::FreeCameraControllerSystem;
        batteryHandlerSystem = new our::BatterySystem;
				radioSystem = new our::RadioSystem;

        // First of all, we get the scene configuration from the app config
        auto& config = getApp()->getConfig()["scene"];
        // If we have assets in the scene config, we deserialize them
        if(config.contains("assets")){
            our::deserializeAllAssets(config["assets"]);
        }
        // If we have a world in the scene config, we use it to populate our world
        if(config.contains("world")){
            world->deserialize(config["world"]);
        }
				// We initialize the camera controller system since it needs a pointer to the app
        cameraController->enter(getApp());

        // We initialize the car controller system since it needs a pointer to the app
        carController->initialize(getApp(), world);

        // Target number of deliveries that a player can make
        int numOfDeliveries = 5;

        // initialize the event handler system
        eventHandlerSystem->startHandler(getApp(), numOfDeliveries);

        // Get the car entity
        deliverySystem->initialize(world, numOfDeliveries);
        lightSystem->initialize(world);
        energySystem->initialize(world, eventHandlerSystem);
        crashingSystem->initialize(world, eventHandlerSystem, energySystem, deliverySystem, batteryHandlerSystem, carController);
        bigObstaclesSystem->initialize(world);
				radioSystem->initialize(getApp());

        // Then we initialize the renderer
        auto size = getApp()->getFrameBufferSize();
        renderer->initialize(size, config["renderer"]);
    }

    void onDraw(double deltaTime) override {
        // Here, we just run a bunch of systems to control the world logic
        movementSystem->update(world, (float)deltaTime);
        carController->update(world, (float)deltaTime);
				cameraController->update(world, (float)deltaTime);
				radioSystem->update();

        std::string postProcessType="";
        bool applyPostProcess =crashingSystem->update(world,postProcessType);
        if(postProcessType!=""){
          lastPostProcessEvent=postProcessType;
        }
        
        energySystem->update(world);
				batteryHandlerSystem->update(world);

        if(applyPostProcess){
          if(lastPostProcessEvent!=""){
            renderer->applyEffect(lastPostProcessEvent);
            applyPostProcess=false;
            timer=1;
          }
        }
        if(renderer->getEffect(lastPostProcessEvent) && timer==80){
          renderer->ignoreEffect(lastPostProcessEvent);
          applyPostProcess=false;
          timer=0;
        }else{
          timer++;
        }
        
        // Get a reference to the keyboard object
        auto& keyboard = getApp()->getKeyboard();

        if(keyboard.justPressed(GLFW_KEY_ESCAPE)){
            // If the escape  key is pressed in this frame, go to the play state
            getApp()->changeState("menu");
        }

        // And finally we use the renderer system to draw the scene
        renderer->render(world);
    }

    void onDestroy() override {
        // Don't forget to destroy the renderer
        renderer->destroy();
        // On exit, we call exit for the camera controller system to make sure that the mouse is unlocked
        cameraController->exit();
        // Clear the world
        world->clear();
        // and we delete all the loaded assets to free memory on the RAM and the VRAM
        our::clearAllAssets();
        // remove all pointers
        delete world;
        delete renderer;
        delete carController;
        delete movementSystem;
        delete crashingSystem;
        delete energySystem;
        delete eventHandlerSystem;
        delete deliverySystem;
        delete lightSystem;
        delete bigObstaclesSystem;
        delete cameraController;
        delete batteryHandlerSystem;
				delete radioSystem;
    }
};