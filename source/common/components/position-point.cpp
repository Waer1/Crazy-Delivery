#include "position-point.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our {
    void PositionPointComponent::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
        position = data.value("position", position);
    }
}