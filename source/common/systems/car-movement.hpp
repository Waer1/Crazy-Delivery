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

		Entity* car = nullptr;

        float carRotationSensitivity = 0.01f;
        glm::vec3 carPositionSensitivity = {3.0f, 3.0f, 3.0f}; // The unity per second of car movement if WASD is pressed
        float defaultRotation=3.14159;
        //float maxRightRotation=1.5;
        //float maxLeftRotation=4.5;
        float maxSpeed=20;
        float minSpeed=2;
        float acceleration=0.1;
        float rateOfRotation=0.03;
        float centeringRate=0.02;


    public:
        // When a state enters, it should call this function and give it the pointer to the application
        void initialize(Application* app, World* world){
            this->app = app;

            // First of all, we search for an entity containing both a CameraComponent and a CarMovementComponent
            // As soon as we find one, we break
            for(auto entity : world->getEntities()){
                // if the entity is car then set the car
				if(entity->name == "car"){
                    car = entity;
                    break;
                }
            }
        }

        void decreaseCarSpeed(){
            carPositionSensitivity.z = 3.0f;
        }
        // This should be called every frame to update all entities containing a CarMovementComponent
        void update(World* world, float deltaTime) {

            if(!(car)){
                 std::cout<<"etl3 bara mfesh car"<<std::endl;
                 return;
            }
            // We get a reference to the camera entity's position and rotation and car rotation
            glm::vec3& position = car->localTransform.position;
            glm::vec3& rotation = car->localTransform.rotation; 

            // We get the camera model matrix (relative to its parent) to compute the front, up and right directions
            glm::mat4 matrix = car->localTransform.toMat4();

            glm::vec3 front = glm::vec3(matrix * glm::vec4(0, 0, 1, 0)),
                      right = glm::vec3(matrix * glm::vec4(-1, 0, 0, 0));

            // We change the camera position based on the keys WASD
            // S & W moves the player back and forth
            if(app->getKeyboard().isPressed(GLFW_KEY_W)) {
                position += front * (deltaTime * this->carPositionSensitivity.z);
                if(this->carPositionSensitivity.z <= this->maxSpeed){
                    this->carPositionSensitivity.z+=this->acceleration;
                }
            }
            else{
                // if(this->carPositionSensitivity.z>=this->minSpeed){
                //     this->carPositionSensitivity.z-=this->acceleration;
                // }
                this->carPositionSensitivity.z = 7.5;
            }
            //Brake
			if(app->getKeyboard().isPressed(GLFW_KEY_B)) {
                 if(this->carPositionSensitivity.z > 10){
                     this->carPositionSensitivity.z=10;
                 }
                 position += front * (deltaTime * this->carPositionSensitivity.z);
                 if(this->carPositionSensitivity.z > 0){
                     this->carPositionSensitivity.z-=this->acceleration;
                 }
			 }
             
            // Move Backwards
            if(app->getKeyboard().isPressed(GLFW_KEY_S)) {
                position -= front * (deltaTime * this->carPositionSensitivity.z);
            }

            // A & D moves the car left or right 
            if(app->getKeyboard().isPressed(GLFW_KEY_D)){
                //Moving the position of each entity in the space right
                position += right * (deltaTime * this->carPositionSensitivity.x);
                rotation.y -=this->rateOfRotation;

            }
    
            if(app->getKeyboard().isPressed(GLFW_KEY_A)) {
                //Moving the position of each entity in the space left
                position -= right * (deltaTime * this->carPositionSensitivity.x);
                rotation.y +=this->rateOfRotation;
            }            

    }
};  

}