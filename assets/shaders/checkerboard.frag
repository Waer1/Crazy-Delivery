#version 330 core

out vec4 frag_color;

// In this shader, we want to draw a checkboard where the size of each tile is (size x size).
// The color of the bottom-left most tile should be "colors[0]" and the 2 tiles adjacent to it
// should have the color "colors[1]".

//TODO: (Req 1) Finish this shader.

uniform int size = 32;
uniform vec3 colors[2];

void main(){
    // Calculate the position of the current fragment in screen space
    vec2 position = gl_FragCoord.xy;

    // Calculate which tile this fragment belongs to
    int xTile = int(position.x) / size;
    int yTile = int(position.y) / size;

    // Determine which color this tile should be based on its position
    vec3 tileColor = colors[ (xTile + yTile) % 2 ];

    // Set the fragment color to the determined tile color
    frag_color = vec4(tileColor, 1.0);
}