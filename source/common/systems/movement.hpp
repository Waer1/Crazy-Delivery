#pragma once

#include "../ecs/world.hpp"
#include "../components/movement.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <iostream>
using namespace std;
namespace our
{

    // The movement system is responsible for moving every entity which contains a MovementComponent.
    // This system is added as a simple example for how use the ECS framework to implement logic. 
    // For more information, see "common/components/movement.hpp"
    class MovementSystem {

        int sign = 1;
        void obstacleMovement(Entity* obstacle, float deltaTime) {
            MovementComponent* movement = obstacle->getComponent<MovementComponent>();
            // Change the position and rotation based on the linear & angular velocity and delta time.
            obstacle->localTransform.position += sign * deltaTime * movement->linearVelocity;
            if(abs(obstacle->localTransform.position.x) >= 19){
                obstacle->localTransform.rotation.y = -obstacle->localTransform.rotation.y;
                obstacle->localTransform.position.z += sign * 2;
                sign = -sign;
            }

            //            obstacle->localTransform.rotation += deltaTime * movement->angularVelocity;
        }

    public:

        // This should be called every frame to update all entities containing a MovementComponent.
        void update(World* world, float deltaTime) {
            // For each entity in the world
            for(auto entity : world->getEntities()){

                if(entity->name == "obstacles") {
                    obstacleMovement(entity, deltaTime);
                }

            }

        }

    };

}
