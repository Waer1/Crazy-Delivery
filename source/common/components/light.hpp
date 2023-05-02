#pragma once

#include "../ecs/component.hpp"

#include <glm/glm.hpp>

namespace our {
		// An enum that defines the type of light
		enum struct LightType {
				DIRECTIONAL,
				POINT,
				SPOT
		};

		class LightComponent : public Component {
		public:
				LightType type; // The type of light
				glm::vec3 color; // The color of the light
				glm::vec3 direction = {0, 0, 0}; // The direction of the light
				glm::vec3 diffuse = {0, 0, 0}; // The diffuse color of the light
				glm::vec3 specular = {0, 0, 0}; // The specular color of the light
				glm::vec3 attenuation = {0, 0, 0}; // The attenuation of the light
				glm::vec2 cone_angles = {0, 0}; // The cone angles of the light => x: inner_angle, y: outer_angle

				// The ID of this component type is "Light"
        static std::string getID() { return "Light"; }

				// Receives the light parameters from the json object
				void deserialize(const nlohmann::json& data) override;
		};
}