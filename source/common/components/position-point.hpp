#pragma once

#include "../ecs/component.hpp"

#include <glm/glm.hpp>

namespace our {

    class PositionPointComponent : public Component {
    public:
        glm::vec3 position = {0, 0, 0};

        static std::string getID() { return "Position Point"; }

        // Reads linearVelocity & angularVelocity from the given json object
        void deserialize(const nlohmann::json& data) override;
    };

}