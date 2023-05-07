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



        void constructFence(Entity* road, float start, float end, float step, float side, int angle) {
            for (float x = start; x <= end; x += step) {
                Entity* fenceR = road->getWorld()->add();
                fenceR->parent = road;
                fenceR->deserialize(generateFence(glm::vec3(x, 0, side), glm::vec3(-90, 0, angle)));
            }
        }

		void constructFenceRightLeft(Entity* road, float start, float end, float step, float right, float left) {
            constructFence(road, start, end, step, right, 180);
            constructFence(road, start, end, step, left, 0);
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
            }else if(newEntity->name == "road-down") {
                // the down fence
                constructFence(newEntity, -19, 21, 0.6, 1.4, 180);
                // the up fence
                constructFence(newEntity, -14.2, 16.3, 0.6, -2.5, 0);
            }else if(newEntity->name == "road-up") {
                // the up fence
                constructFence(newEntity, -19, 21, 0.6, 1.2, 180);
                // the down fence
                constructFence(newEntity, -14.2, 16.3, 0.6, -2.6 , 0);
            }else if(newEntity->name == "road-left"){
                // the left fence
                constructFence(newEntity, -15, 17, 0.6, -2.6, 0);
                // the right fences
                constructFence(newEntity, -10, -2, 0.6, 1.5 , 180);
                constructFence(newEntity, 3.5, 12.51, 0.6, 1.5 , 180);
            }else if(newEntity->name == "road-right"){
                // the right fence
                constructFence(newEntity, -15, 17, 0.6, -2.6, 0);
                // the left fences
                constructFence(newEntity, -10, -1.5, 0.6, 1.5 , 180);
                constructFence(newEntity, 4.05, 12.4, 0.6, 1.5 , 180);
            }


            if(entityData.contains("children")){
                //TODO: (Req 8) Recursively call this world's "deserialize" using the children data
                // and the current entity as the parent
                deserialize(entityData["children"], newEntity);
            }
        }
    }

}