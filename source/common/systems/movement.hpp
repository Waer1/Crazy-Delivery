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

        void rangeMovement(Entity* entity, float deltaTime) {

            MovementComponent* movement = entity->getComponent<MovementComponent>();
            // Change the position and rotation based on the linear & angular velocity and delta time.
            entity->localTransform.position += movement->direction * deltaTime * movement->linearVelocity;
            entity->localTransform.rotation += deltaTime * movement->angularVelocity;

            if((movement->movementRangeX.y + movement->movementRangeX.x) != 0 &&
            (entity->localTransform.position.x >= movement->movementRangeX.y || entity->localTransform.position.x < movement->movementRangeX.x) ){
                movement->direction.x = -movement->direction.x;
            }

            if((movement->movementRangeY.y + movement->movementRangeY.x) != 0 &&
               (entity->localTransform.position.y >= movement->movementRangeY.y || entity->localTransform.position.y < movement->movementRangeY.x) ){
                movement->direction.y = -movement->direction.y;
            }

            if((movement->movementRangeZ.y + movement->movementRangeZ.x) != 0 &&
               (entity->localTransform.position.z >= movement->movementRangeZ.y || entity->localTransform.position.z < movement->movementRangeZ.x) ){
                movement->direction.z = -movement->direction.z;
            }
        }

        void obstacleMovement(Entity* obstacle, float deltaTime) {

            MovementComponent* movement = obstacle->getComponent<MovementComponent>();
            // Change the position and rotation based on the linear & angular velocity and delta time.
            obstacle->localTransform.position += movement->direction * deltaTime * movement->linearVelocity;
            if(abs(obstacle->localTransform.position.x) >= 19){
                // change the direction of movement of the obstacle(monkey face)
                obstacle->localTransform.rotation.y = -obstacle->localTransform.rotation.y;
                // swap the side road
                obstacle->localTransform.position.z += movement->direction.z * 2;
                movement->direction = -movement->direction;
            }
        }

    public:

        // This should be called every frame to update all entities containing a MovementComponent.
        void update(World* world, float deltaTime) {
            // For each entity in the world
            for(auto entity : world->getEntities()){

                if(entity->name == "obstacles") {
                    obstacleMovement(entity, deltaTime);
                } else if(entity->name == "battery" || entity->name == "arrow") {
                    rangeMovement(entity, deltaTime);
                }

            }

        }

    };

}
