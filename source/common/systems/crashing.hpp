#pragma once

#include "../ecs/world.hpp"

#include <glm/glm.hpp>

namespace our
{

    // The crashing system is responsible for checking if the car has crashed with any obstacle.
    class CrashingSystem {

    public:

        // This should be called every frame to update all entities containing a MovementComponent.
        void update(World* world) {
            // For each entity in the world
            for(auto entity : world->getEntities()){

            }
        }
    };

}
