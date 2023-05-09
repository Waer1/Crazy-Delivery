#version 330

layout(location = 0) in vec3 position;       // Vertex position attribute
layout(location = 1) in vec4 color;          // Vertex color attribute
layout(location = 2) in vec2 tex_coord;      // Texture coordinates attribute
layout(location = 3) in vec3 normal;         // Vertex normal attribute

uniform mat4 VP;                            // View-projection matrix
uniform vec3 eye;               						// Position of the camera (eye)
uniform mat4 M;                             // Model matrix
uniform mat4 M_IT;                          // Inverse-transpose of the model matrix

out Varyings {
    vec4 color;                             // Color to be passed to the fragment shader
    vec2 tex_coord;                         // Texture coordinates to be passed to the fragment shader
    vec3 normal;                            // Normal to be passed to the fragment shader
    vec3 view;                              // Vector from vertex to camera position
    vec3 world;                             // World position of the vertex
} vs_out;

void main() {
    // Get the world position of the vertex
    vec3 world = (M * vec4(position, 1.0)).xyz;

    // Calculate the clip-space position of the vertex
    gl_Position = VP * vec4(world, 1.0);

    // Pass vertex attributes to the fragment shader
    vs_out.color = color;
    vs_out.tex_coord = tex_coord;

    // Transform the normal to world space using the inverse-transpose of the model matrix
    vs_out.normal = normalize((M_IT * vec4(normal, 0.0)).xyz);

    // Calculate the vector from the vertex to the camera position
    vs_out.view = eye - world;

    // Pass the world position of the vertex
    vs_out.world = world;
}
