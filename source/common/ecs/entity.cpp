#include "entity.hpp"
#include "../deserialize-utils.hpp"
#include "../components/component-deserializer.hpp"

#include <glm/gtx/euler_angles.hpp>

namespace our {

    // This function returns the transformation matrix from the entity's local space to the world space
    // Remember that you can get the transformation matrix from this entity to its parent from "localTransform"
    // To get the local to world matrix, you need to combine this entities matrix with its parent's matrix and
    // its parent's parent's matrix and so on till you reach the root.
    glm::mat4 Entity::getLocalToWorldMatrix() const {
        //TODO: (Req 8) Write this function

				// Create a temporary entity pointer to the current entity's parent
        Entity* temp  = parent;

				// Create a tranformation matrix for the localToWorld
        glm::mat4 localToWorld = localTransform.toMat4();

				// Keep going up to all the parents until we reach the root
        while(temp) {
						// Update the localToWorld matrix by multiplying it with the parent's localTransform matrix 
            localToWorld  = temp->localTransform.toMat4() * localToWorld;

						// Move the pointer up to the parent of the entity
            temp = temp->parent;
        }
				
        return localToWorld;
    }

    // Deserializes the entity data and components from a json object
    void Entity::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
        name = data.value("name", name);
        localTransform.deserialize(data);
        if(data.contains("components")){
            if(const auto& components = data["components"]; components.is_array()){
                for(auto& component: components){
                    deserializeComponent(component, this);
                }
            }
        }
    }

}