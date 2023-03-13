#version 330 core

// This shader is designed to work with "triangle.vert" and it receives an
// interpolated varying which represents the vertex color.

in Varyings {
    vec3 color;
} fs_in;

out vec4 frag_color;

uniform vec4 red;
uniform vec4 green;
uniform vec4 blue;

// currently the shader just returns the interpalated color varying.
// However, we want to mix the color channels around. We can do this using a 
// color matrix which we will send to the shader as 3 uniforms: red, green, blue.
// Each of these 3 variables will be a vec4. To apply the channel mixing for a
// certain channel (e.g. red), we apply this linear transformation:
// frag_color.r = red.r * fs_in.color.r + red.g + fs_in.color.g + red.b + fs_in.color.b + red.a;
// However, this line is too long to write, so we can simplify it using a dot product
// (which is defined in the "dot" function).

//TODO: (Req 1) Finish this shader and apply the channel mixing using the "dot" function.

void main(){
    //Converting fs_in to vec4 to be able to use dot function
    //MAY BE EDITED as the color here is sent as a float from trangle.vert, for optimization we can convert it to int in the upcoming stages.
    vec4 temp=vec4(fs_in.color.r,fs_in.color.g,fs_in.color.b,1.0);
    //Dot function between the temp variable that we created from fs_in and the corresponding channel vector
    frag_color.r = dot(red,temp);
    frag_color.g =dot(green,temp);
    frag_color.b = dot(blue,temp);
}