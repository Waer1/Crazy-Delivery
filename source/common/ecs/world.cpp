#include "world.hpp"
#include <cstdlib> // for rand() and srand()
#include <ctime> // for time()


namespace our {
    glm::vec3 randomPositions[3]=
            {
            glm::vec3(0, 3.5, -3.75),
                glm::vec3(5, 3.5, -3.75),
                glm::vec3(-5, 3.5, -3.75)
            };

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

            if(entityData.contains("children")){
                //TODO: (Req 8) Recursively call this world's "deserialize" using the children data
                // and the current entity as the parent
                deserialize(entityData["children"], newEntity);
            }
        }
    }

}