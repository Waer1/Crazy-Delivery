#pragma once

#include "../ecs/entity.hpp"
#include "camera.hpp"
#include "mesh-renderer.hpp"
#include "free-camera-controller.hpp"
#include "movement.hpp"
#include "light.hpp"

namespace our {

    // Given a json object, this function picks and creates a component in the given entity
    // based on the "type" specified in the json object which is later deserialized from the rest of the json object
    //each system searches for the components needed for it, as for example renderer focuses on position and MeshRenderer components, and doesn't care about other components
    inline void deserializeComponent(const nlohmann::json& data, Entity* entity){
        std::string type = data.value("type", "");
        Component* component = nullptr;
        //TODO: (Req 8) Add an option to deserialize a "MeshRendererComponent" to the following if-else statement

        if(type == CameraComponent::getID()) {
            component = entity->addComponent<CameraComponent>();
        } else if (type == FreeCameraControllerComponent::getID()) {
            component = entity->addComponent<FreeCameraControllerComponent>();
        } else if (type == MovementComponent::getID()) {
            component = entity->addComponent<MovementComponent>();
        } else if(type == MeshRendererComponent::getID()){
            component = entity->addComponent<MeshRendererComponent>();
        } else if (type == LightComponent::getID()) {
						component = entity->addComponent<LightComponent>();
				}
        //serialization -> taking our world with its entities to a file
        //deserialization-> opening a JSON file and filling our entities and their components
        //JSON file has the entities and the components attached to each of them
        if(component) component->deserialize(data);
    }

}