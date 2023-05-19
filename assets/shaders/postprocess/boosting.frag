#version 330

// The texture holding the scene pixels
uniform sampler2D tex;
// Texture coordinates in which the texture will be drawn
in vec2 tex_coord;
// Color whill be sent from the fragment shader
out vec4 frag_color;

#define REDSTRENGTH 0.5
#define GREENSTRENGTH 1.2
#define BLUESTRENGTH 0.1

void main(){
    //Reducing the channels of the red and blue colors
    //Increasing the channel of green colors just to be a sign for getting boost
    frag_color = vec4(
    texture(tex, tex_coord).r*REDSTRENGTH,
    texture(tex, tex_coord).g*GREENSTRENGTH,
    texture(tex, tex_coord).r*BLUESTRENGTH,
    1.0);
}