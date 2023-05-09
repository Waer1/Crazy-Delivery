#include "light.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our {
		// Reads the light parameters from the given json object
		void LightComponent::deserialize(const nlohmann::json& data){
				if (!data.is_object()) return;
				
				std::string type_str = data.value("lightType", "directional");
				if (type_str == "directional")
						type = LightType::DIRECTIONAL;
				else if (type_str == "spot")
						type = LightType::SPOT;
				else 
						type = LightType::POINT;
				
				direction = data.value("direction", direction);
				diffuse = data.value("diffuse", diffuse);
				specular = data.value("specular", specular);
				attenuation = data.value("attenuation", attenuation);
				cone_angles = data.value("cone_angles", cone_angles);
		}
}