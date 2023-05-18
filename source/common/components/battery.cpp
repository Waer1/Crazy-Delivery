#include "battery.hpp"
#include "../ecs/entity.hpp"
#include <glm/glm.hpp>

namespace our {
    void BatteryComponent::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;

        isTaken = data.value("isTaken", isTaken);
    }
}