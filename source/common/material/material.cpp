#include "material.hpp"

#include "../asset-loader.hpp"
#include "deserialize-utils.hpp"

namespace our {

    // This function should setup the pipeline state and set the shader to be used
    void Material::setup() const {
        //TODO: (Req 7) Write this function

				// Start the the pipeline state
        pipelineState.setup();

        // Set the shader to be used
				shader->use();
    }

    // This function read the material data from a json object
    void Material::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;

        if(data.contains("pipelineState")){
            pipelineState.deserialize(data["pipelineState"]);
        }
        shader = AssetLoader<ShaderProgram>::get(data["shader"].get<std::string>());
        transparent = data.value("transparent", false);
    }

    // This function should call the setup of its parent and
    // set the "tint" uniform to the value in the member variable tint 
    void TintedMaterial::setup() const {
        //TODO: (Req 7) Write this function

				// Setup the material
        Material::setup();
				
        // Set the "tint" uniform in the shader
				shader->set("tint", tint);
    }

    // This function read the material data from a json object
    void TintedMaterial::deserialize(const nlohmann::json& data){
        Material::deserialize(data);
        if(!data.is_object()) return;
        tint = data.value("tint", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }

    // This function should call the setup of its parent and
    // set the "alphaThreshold" uniform to the value in the member variable alphaThreshold
    // Then it should bind the texture and sampler to a texture unit and send the unit number to the uniform variable "tex"
    void TexturedMaterial::setup() const {
        //TODO: (Req 7) Write this function

				// Setup the material
				TintedMaterial::setup();

				// Set the "alphaThreshold" uniform in the shader
				shader->set("alphaThreshold", alphaThreshold);

				// Bind the texture and sampler to a texture unit 0 and send the unit number to the uniform variable "tex"
        if (texture) {
				  texture->bind();
        }
        if (sampler) {
				  sampler->bind(0);
        }
				shader->set("tex", 0);
    }

    // This function read the material data from a json object
    void TexturedMaterial::deserialize(const nlohmann::json& data){
        TintedMaterial::deserialize(data);
        if(!data.is_object()) return;
        alphaThreshold = data.value("alphaThreshold", 0.0f);
        texture = AssetLoader<Texture2D>::get(data.value("texture", ""));
        sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));
    }

    // setup of the lightMaterial to create the needed textures based on the type
    void LightMaterial::setup() const
    {
				// Setup the material
        Material::setup();

        if (albedo != nullptr) {
            // select an active texture unit -> 0
            glActiveTexture(GL_TEXTURE0);
            // bind the texture to unit 0
            albedo->bind();
            // bind the sampler to unit 0
            sampler->bind(0);
            shader->set("material.albedo", 0);
        }

        if (specular != nullptr) {
            // Select an active texture unit -> 1
            glActiveTexture(GL_TEXTURE1);
            // Bind the texture to unit 1
            specular->bind();
            // Bind the sampler to unit 1
            sampler->bind(1);
            shader->set("material.specular", 1);
        }

        if (emissive != nullptr) {
            // Select an active texture unit -> 2
            glActiveTexture(GL_TEXTURE2);
            // Bind the texture to unit 2
            emissive->bind();
            // Bind the sampler to unit 2
            sampler->bind(2);
            shader->set("material.emissive", 2);
        }

        if (roughness != nullptr) {
            // Select an active texture unit -> 3
            glActiveTexture(GL_TEXTURE3);
            // Bind the texture to unit 3
            roughness->bind();
            // Bind the sampler to unit 3
            sampler->bind(3);
            shader->set("material.roughness", 3);
        }

        if (ambient_occlusion != nullptr) {
            // Select an active texture unit -> 4
            glActiveTexture(GL_TEXTURE4);
            // Bbind the texture to unit 4
            ambient_occlusion->bind();
            // Bind the sampler to unit 4
            sampler->bind(4);
            shader->set("material.ambient_occlusion", 4);
				}

				glActiveTexture(GL_TEXTURE0);
    }

    // This function read the material data from a json object
    void LightMaterial::deserialize(const nlohmann::json &data)
    {
        Material::deserialize(data);

        if (!data.is_object())
            return;

        sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));
        albedo = AssetLoader<Texture2D>::get(data.value("albedo", ""));
        specular = AssetLoader<Texture2D>::get(data.value("specular", ""));
        emissive = AssetLoader<Texture2D>::get(data.value("emissive", ""));
        roughness = AssetLoader<Texture2D>::get(data.value("roughness", ""));
        ambient_occlusion = AssetLoader<Texture2D>::get(data.value("ambient_occlusion", ""));
    }
}