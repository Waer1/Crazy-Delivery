#version 330

// The texture holding the scene pixels
uniform sampler2D tex;

// ref : https://github.com/thoxey/ezPP/blob/master/ezPP/src/ezsharpness.cpp
in vec2 tex_coord;

out vec4 frag_color;
#define STRENGTH 0.009

void main(){
    //Moving red channel coordiante right
    //Keeping green channel coordiante at its position
    //Moving blue channel coordinate left
    frag_color = vec4(
        texture(tex, tex_coord + vec2(-STRENGTH,-STRENGTH)).r,
        texture(tex, tex_coord).g,
        texture(tex, tex_coord + vec2(STRENGTH, STRENGTH)).b,
        1.0);
}