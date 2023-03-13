#version 330

// This vertex shader should be used to render a triangle whose normalized device coordinates are:
// (-0.5, -0.5, 0.0), ( 0.5, -0.5, 0.0), ( 0.0,  0.5, 0.0)
// And it also should send the vertex color as a varying to the fragment shader where the colors are (in order):
// (1.0, 0.0, 0.0), (0.0, 1.0, 0.0), (0.0, 0.0, 1.0)

out Varyings {
    vec3 color;
} vs_out;

// Currently, the triangle is always in the same position, but we don't want that.
// So two uniforms should be added: translation (vec2) and scale (vec2).
// Each vertex "v" should be transformed to be "scale * v + translation".
// The default value for "translation" is (0.0, 0.0) and for "scale" is (1.0, 1.0).

// Declare translation and scale uniforms
uniform vec2 translation = vec2(0.0, 0.0);
uniform vec2 scale = vec2(1.0, 1.0);

void main() {
    // Define an array of vertex positions
    vec3 vertices[3] = vec3[3](
		vec3(-0.5, -0.5, 0.0),
		vec3( 0.5, -0.5, 0.0),
		vec3( 0.0,  0.5, 0.0)
    );

	// Define an array of vertex colors
	const vec3 colors[3] = vec3[3](
		vec3(1.0, 0.0, 0.0),
		vec3(0.0, 1.0, 0.0),
		vec3(0.0, 0.0, 1.0)
	);

    // Transform each vertex by a vector that contains scale and translation
	vec3 Scale = vec3(scale.x, scale.y, 0.0));
	vec3 Translation = vec3(translation.x, translation.y, 0.0);
    gl_Position = vec4((vertices[gl_VertexID] * Scale + Translation, 1.0);

    // Pass the color to the fragment shader
    vs_out.color = colors[gl_VertexID];
}