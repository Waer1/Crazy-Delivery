#pragma once

#include "../ecs/component.hpp"

namespace our {

    class BatteryComponent : public Component {
    public:
				bool isTaken = false;

        // The ID of this component type is "Camera"
        static std::string getID() { return "Battery"; }

        // Reads camera parameters from the given json object
        void deserialize(const nlohmann::json& data) override;
    };

}