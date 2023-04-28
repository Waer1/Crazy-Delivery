#include "entity.hpp"
#include "../deserialize-utils.hpp"

#include <glm/gtx/euler_angles.hpp>

namespace our {

    // This function computes and returns a matrix that represents this transform
    // Remember that the order of transformations is: Scaling, Rotation then Translation
    // HINT: to convert euler angles to a rotation matrix, you can use glm::yawPitchRoll
    // Pitch -> rotation around X-axis
    // Yaw -> rotation around Y-axis
    // Roll -> rotation around Z-axis
    // if we made a 90 degree rotation then we will use a degree of freedom as y,z for example maybe have the same direction, it's not effective in animation
    glm::mat4 Transform::toMat4() const {
        //TODO: (Req 3) Write this function
                //Generating Scale matrix, using unity matrix filling it with the needed scale
				glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), scale);
                //Generating rotation matrix, using unity matrix filling it with the needed rotations in all directions
				glm::mat4 rotationMat = glm::yawPitchRoll(rotation.y, rotation.x, rotation.z);
                //Generating translation matrix, using unity matrix filling it with the needed position to be translated
				glm::mat4 translationMat = glm::translate(glm::mat4(1.0f), position);
				// Order should be: scaling => rotation => translation just to follow TRS
				return translationMat * rotationMat * scaleMat;
    }

     // Deserializes the entity data and components from a json object
    void Transform::deserialize(const nlohmann::json& data){
        position = data.value("position", position);
        rotation = glm::radians(data.value("rotation", glm::degrees(rotation)));
        scale    = data.value("scale", scale);
    }

}