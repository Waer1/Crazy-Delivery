#pragma once

#include "../ecs/world.hpp"
#include "../components/movement.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

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
			
			if (entity->localTransform.position.y > movement->movementRangeY.y) {
				entity->localTransform.position.y = movement->movementRangeY.y;
			} else if (entity->localTransform.position.y < movement->movementRangeY.x) {
				entity->localTransform.position.y = movement->movementRangeY.x;
			}

            if((movement->movementRangeX.y + movement->movementRangeX.x) != 0 &&
			    (entity->localTransform.position.x >= movement->movementRangeX.y || entity->localTransform.position.x <= movement->movementRangeX.x) ){
                movement->direction.x = -movement->direction.x;
            }

            if((movement->movementRangeY.y + movement->movementRangeY.x) != 0 &&
				(entity->localTransform.position.y >= movement->movementRangeY.y || entity->localTransform.position.y <= movement->movementRangeY.x) ){
                movement->direction.y = -movement->direction.y;
            }

            if((movement->movementRangeZ.y + movement->movementRangeZ.x) != 0 &&
               	(entity->localTransform.position.z >= movement->movementRangeZ.y || entity->localTransform.position.z <= movement->movementRangeZ.x) ){
                movement->direction.z = -movement->direction.z;
            }
        }

        void obstacleMovement(Entity* obstacle, float deltaTime) {

            MovementComponent* movement = obstacle->getComponent<MovementComponent>();

            // Change the position and rotation based on the linear & angular velocity and delta time.
            obstacle->localTransform.position += movement->direction * deltaTime * movement->linearVelocity;

            // check first if the object have range in the X axis
            // if the entity exceed the range of movement in the X axis in the positive or negative direction
            if(movement->movementRangeX.y != 0 || movement->movementRangeX.x != 0) {
                if( obstacle->localTransform.position.x >= movement->movementRangeX.y ||
                    obstacle->localTransform.position.x <= movement->movementRangeX.x) {
                    // change the direction of movement of the obstacle(monkey face)
                    obstacle->localTransform.rotation.y = -obstacle->localTransform.rotation.y;
                    // swap the side road
                    obstacle->localTransform.position.z += movement->direction.z * 5;
                    movement->direction = -movement->direction;
                }
            }
            // check first if the object have range in the Z axis
            // if the entity exceed the range of movement in the Z axis in the positive or negative direction
            if(movement->movementRangeZ.y != 0 || movement->movementRangeZ.x != 0) {
                if( obstacle->localTransform.position.z >= movement->movementRangeZ.y ||
                    obstacle->localTransform.position.z <= movement->movementRangeZ.x) {
                    // change the direction of movement of the obstacle(monkey face)
                    obstacle->localTransform.rotation.y = (obstacle->localTransform.rotation.y == 0) ? 210 : 0;
                    // swap the side road
                    obstacle->localTransform.position.x += movement->direction.x * 7;
                    movement->direction = -movement->direction;
                }
            }

        }

    public:
        // This should be called every frame to update all entities containing a MovementComponent.
        void update(World* world, float deltaTime) {
            // For each entity in the world
            for(auto entity : world->getEntities()){
                if (entity->name == "obstacles") {
                    obstacleMovement(entity, deltaTime);
                    MovementComponent* obstacleMovement = entity->getComponent<MovementComponent>();
                    obstacleMovement->linearVelocity = min(obstacleMovement->linearVelocity * glm::vec3(1.01,1.01,1.01), glm::vec3(20,20,20));
                } else if(entity->name == "battery" || entity->name == "arrow") {
                    rangeMovement(entity, deltaTime);
                } else {
					// Get the movement component if it exists
					MovementComponent* movement = entity->getComponent<MovementComponent>();
					// If the movement component exists
					if(movement){
						// Change the position and rotation based on the linear & angular velocity and delta time.
						entity->localTransform.position += deltaTime * movement->linearVelocity;
						entity->localTransform.rotation += deltaTime * movement->angularVelocity;
					}
				}
            }
        }
    };
}
