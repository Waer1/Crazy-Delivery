#pragma once

#include "../ecs/world.hpp"
#include "../components/camera.hpp"

#include "../application.hpp"
#include <glm/glm.hpp>
#include <iostream>
using namespace std;

namespace our
{

    // The car movement system is responsible for moving the car entity.
    class CarMovementSystem {
        Application* app; // The application in which the state runs
        Keyboard* keyboard;

		CameraComponent* camera = nullptr;
		Entity* car = nullptr;

    public:
        // When a state enters, it should call this function and give it the pointer to the application
        void initialize(Application* app, World* world){
            this->app = app;
            this->keyboard = &app->getKeyboard();

            // First of all, we search for an entity containing both a CameraComponent and a CarMovementComponent
            // As soon as we find one, we break
            for(auto entity : world->getEntities()){
                camera = entity->getComponent<CameraComponent>();

				if(entity->name == "car"){
                    car = entity;
                }

                if(camera && car){
                    break;
                }
            }
        }

        // This should be called every frame to update all entities containing a CarMovementComponent
        void update(World* world, float deltaTime) {
            // If there is no entity with both a CameraComponent and CarEntity, we can do nothing so we return
            if(!(camera && car)) return;

            // Get the entity that we found via getOwner of camera
            Entity* entity = camera->getOwner();

            // We get a reference to the camera entity's position and rotation and car rotation
            glm::vec3& position = car->localTransform.position;
            glm::vec3& rotation = car->localTransform.rotation; 

            // We get the camera model matrix (relative to its parent) to compute the front, up and right directions
            glm::mat4 matrix = car->localTransform.toMat4();

            glm::vec3 front = glm::vec3(matrix * glm::vec4(0, 0, 1, 0)),
                      right = glm::vec3(matrix * glm::vec4(-1, 0, 0, 0));

            float sensitivity = 9.0f;
            // We change the camera position based on the keys WASD
            // S & W moves the player back and forth
            if(keyboard->isPressed(GLFW_KEY_W)) {
                position += front * (deltaTime * sensitivity);
				right.x = abs(right.x);
            }

			if(keyboard->isPressed(GLFW_KEY_S)) {
				position -= front * (deltaTime * (sensitivity - 1.0f));
				right.x = -abs(right.x);
			}

            // A & D moves the car left or right 
            if(keyboard->isPressed(GLFW_KEY_D)){
                // Move the position of each entity in the space right
                position += right * (deltaTime * sensitivity);
				rotation.y -= 0.03;
            }
    
            if(keyboard->isPressed(GLFW_KEY_A)) {
                // Move the position of each entity in the space left
                position -= right * (deltaTime * sensitivity);
				rotation.y += 0.03;
                
            }
        }
    };
}
