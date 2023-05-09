#version 330

#define DIRECTIONAL 0
#define POINT       1
#define SPOT        2

struct Light {
    int type;               // Type of light (0: directional, 1: point, 2: spot)
    vec3 position;          // Position of the light
    vec3 direction;         // Direction of the light
    vec3 diffuse;           // Diffuse color of the light
    vec3 specular;          // Specular color of the light
    vec3 attenuation;       // Attenuation factors (constant, linear, quadratic)
    vec2 cone_angles;       // Cone angles (inner, outer) for spot lights
};

#define MAX_LIGHTS 8         // Maximum number of lights

uniform Light lights[MAX_LIGHTS];      // Array of lights
uniform int light_count;               // Number of lights

struct Sky {
    vec3 top, horizon, bottom;         // Sky colors (top, horizon, bottom)
};

uniform Sky sky;                       // Sky uniform variable

struct Material {
    sampler2D albedo;                   // Albedo texture
    sampler2D specular;                 // Specular texture
    sampler2D roughness;                // Roughness texture
    sampler2D ambient_occlusion;        // Ambient occlusion texture
    sampler2D emissive;                 // Emissive texture
};

uniform Material material;              // Material uniform variable

in Varyings {
    vec4 color;
    vec2 tex_coord;
    vec3 normal;
    vec3 view;
    vec3 world;
} fs_in;

out vec4 frag_color;

float lambert(vec3 normal, vec3 world_to_light_direction) {
    return max(0.0, dot(normal, world_to_light_direction));
}

float phong(vec3 reflected, vec3 view, float shininess) {
    return pow(max(0.0, dot(reflected, view)), shininess);
}

vec3 compute_sky_light(vec3 normal){
    vec3 extreme = normal.y > 0 ? sky.top : sky.bottom;
    return mix(sky.horizon, extreme, normal.y * normal.y);
}

void main() {
    vec3 normal = normalize(fs_in.normal);      // Normalize the interpolated normal vector
    vec3 view = normalize(fs_in.view);          // Normalize the interpolated view vector
    
		// Compute the ambient light contribution based on the sky color and normal
    vec3 ambient_light = compute_sky_light(normal);

		// Fetch the diffuse color from the albedo texture
    vec3 material_diffuse = texture(material.albedo, fs_in.tex_coord).rgb;
		// Fetch the specular color from the specular texture
    vec3 material_specular = texture(material.specular, fs_in.tex_coord).rgb;
		// Fetch the roughness value from the roughness texture
    float material_roughness = texture(material.roughness, fs_in.tex_coord).r;
		// Fetch the ambient color from the ambient occlusion texture
    vec3 material_ambient = material_diffuse * texture(material.ambient_occlusion, fs_in.tex_coord).r;
		// Fetch the emissive color from the emissive texture
    vec3 material_emissive = texture(material.emissive, fs_in.tex_coord).rgb;

		// Calculate the shininess factor based on the roughness
    float shininess = 2.0 / pow(clamp(material_roughness, 0.001, 0.999), 4.0) - 2.0;
    
		// Initialize the fragment color with the emissive and ambient light contributions
    vec3 color = material_emissive + ambient_light * material_ambient;

    vec3 world_to_light_dir;

    for(int light_idx = 0; light_idx < min(MAX_LIGHTS, light_count); light_idx++){
				// Fetch the current light from the lights array
        Light light = lights[light_idx];
				// Attenuation factor, initially set to 1.0
        float attenuation = 1.0;
        
        if(light.type == DIRECTIONAL){
						// Set the world-to-light direction as the opposite of the light direction
            world_to_light_dir = -light.direction;
        } else {
            world_to_light_dir = light.position - fs_in.world;
						// Compute the distance between the fragment and the light
            float d = length(world_to_light_dir);
						// Normalize the world-to-light direction
            world_to_light_dir /= d;

						 // Compute the attenuation factor based on the distance and attenuation coefficients
            attenuation = 1.0 / dot(light.attenuation, vec3(d*d, d, 1.0));

            if(light.type == SPOT) {
								// Compute the angle between the light direction and the opposite of the world-to-light direction
                float angle = acos(dot(light.direction, -world_to_light_dir));
                attenuation *= smoothstep(light.cone_angles.y, light.cone_angles.x, angle);
            }
        }

				// Compute the diffuse light contribution using Lambertian reflection model
        vec3 computed_diffuse = light.diffuse * material_diffuse * lambert(normal, world_to_light_dir);

				// Compute the reflection direction of the light
        vec3 reflected = reflect(-world_to_light_dir, normal);
				// Compute the specular light contribution using the Phong reflection model
        vec3 computed_specular = light.specular * material_specular * phong(reflected, view, shininess);

        color += (computed_diffuse + computed_specular) * attenuation; 
    }
    
    frag_color = vec4(color, 1.0); 		// Set the fragment color
}