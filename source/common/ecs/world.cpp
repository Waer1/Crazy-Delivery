#include "world.hpp"

namespace our {

		nlohmann::json generateFence(glm::vec3 position, glm::vec3 rotation) {
			return {
							{"name", "fence"},
							{"position", {position.x, position.y, position.z}},
							{"rotation", {rotation.x, rotation.y, rotation.z}},
							{"scale", {0.009, 0.03, 0.03}},
							{"components", nlohmann::json::array({
								{
									{"type", "Mesh Renderer"},
									{"mesh", "fence"},
									{"material", "fence"}
								}
							})}
						};
		}

		void constructFenceRightLeft(Entity* road, float start, float end, float step, float right, float left) {
				for (float x = start; x <= end; x += step) {
					Entity* fenceR = road->getWorld()->add();
					fenceR->parent = road;
					fenceR->deserialize(generateFence(glm::vec3(x, 0, right), glm::vec3(-90, 0, 180)));

					Entity* fenceL = road->getWorld()->add();
					fenceL->parent = road;
					fenceL->deserialize(generateFence(glm::vec3(x, 0, left), glm::vec3(-90, 0, 0)));
				}
		}

    // This will deserialize a json array of entities and add the new entities to the current world
    // If parent pointer is not null, the new entities will be have their parent set to that given pointer
    // If any of the entities has children, this function will be called recursively for these children
    void World::deserialize(const nlohmann::json& data, Entity* parent){
        if(!data.is_array()) return;
        for(const auto& entityData : data){
            //TODO: (Req 8) Create an entity, make its parent "parent" and call its deserialize with "entityData".

						// Create a new entity and set it parent to the given parent
            Entity * newEntity = add();
            newEntity->parent = parent;

						// Deserialize the entity with the given entityData
            newEntity->deserialize(nlohmann::json(entityData));

						if (newEntity->name == "central-road") {
							constructFenceRightLeft(newEntity, -14, 16.4, 0.6, 1.3, -2.5);
						}

            if(entityData.contains("children")){
                //TODO: (Req 8) Recursively call this world's "deserialize" using the children data
                // and the current entity as the parent
                deserialize(entityData["children"], newEntity);
            }
        }
    }

}