#pragma once

#include <application.hpp>

#include <ecs/world.hpp>
#include <systems/forward-renderer.hpp>
#include <systems/movement.hpp>
#include <systems/crashing.hpp>
#include <systems/energy.hpp>
#include <systems/event-handler.hpp>
#include <systems/delivery.hpp>
#include <systems/bigObstacles.hpp>
#include <systems/car-movement.hpp>
#include <asset-loader.hpp>


// This state shows how to use the ECS framework and deserialization.
class Playstate: public our::State {

    our::World world;
    our::ForwardRenderer renderer;
	our::CarMovementSystem carController;
    our::MovementSystem movementSystem;
    our::CrashingSystem crashingSystem;
    our::EnergySystem energySystem;
    our::EventHandlerSystem eventHandlerSystem;
    our::DeliverySystem deliverySystem;
    our::BigObstaclesSystem bigObstaclesSystem;

    void onInitialize() override {
        // First of all, we get the scene configuration from the app config
        auto& config = getApp()->getConfig()["scene"];
        // If we have assets in the scene config, we deserialize them
        if(config.contains("assets")){
            our::deserializeAllAssets(config["assets"]);
        }
        // If we have a world in the scene config, we use it to populate our world
        if(config.contains("world")){
            world.deserialize(config["world"]);
        }
        
		// We initialize the car controller system since it needs a pointer to the app
		carController.initialize(getApp(), &world);

        // Target number of deliveries that a player can make
        int numOfDeliveries = 5, numberofBigObstacles = 1;

        // initialize the event handler system
        eventHandlerSystem.startHandler(getApp(), numOfDeliveries);

        // Get the car entity
        deliverySystem.initialize(&world, numOfDeliveries);
        crashingSystem.initialize(&world, &eventHandlerSystem, &energySystem, &deliverySystem);
        energySystem.initialize(&world, &eventHandlerSystem);
        bigObstaclesSystem.initialize(&world, numberofBigObstacles);

        // Then we initialize the renderer
        auto size = getApp()->getFrameBufferSize();
        renderer.initialize(size, config["renderer"]);
    }

    void onDraw(double deltaTime) override {
        // Here, we just run a bunch of systems to control the world logic
        movementSystem.update(&world, (float)deltaTime);
        carController.update(&world, (float)deltaTime);
        crashingSystem.update(&world);
        energySystem.update();

        // And finally we use the renderer system to draw the scene
        renderer.render(&world);

        // Get a reference to the keyboard object
        auto& keyboard = getApp()->getKeyboard();

        if(keyboard.justPressed(GLFW_KEY_ESCAPE)){
            // If the escape  key is pressed in this frame, go to the play state
            getApp()->changeState("menu");
        }
    }

    void onDestroy() override {
        // Don't forget to destroy the renderer
        renderer.destroy();
        // Clear the world
        world.clear();
        // and we delete all the loaded assets to free memory on the RAM and the VRAM
        our::clearAllAssets();
    }
};