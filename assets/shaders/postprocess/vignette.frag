#version 330

// The texture holding the scene pixels
uniform sampler2D tex;

// Read "assets/shaders/fullscreen.vert" to know what "tex_coord" holds;
in vec2 tex_coord;

out vec4 frag_color;

// Vignette is a postprocessing effect that darkens the corners of the screen
// to grab the attention of the viewer towards the center of the screen

void main(){
    //TODO: Modify this shader to apply vignette
    // To apply vignette, divide the scene color
    // by 1 + the squared length of the 2D pixel location the NDC space
    // Hint: remember that the NDC space ranges from -1 to 1 -> then it's float
    // while the texture coordinate space ranges from 0 to 1
    // We have the pixel's texture coordinate, how can we compute its location in the NDC space?
    frag_color = texture(tex, tex_coord);
    //Then we will need to divide the scene color by (1+ squared length of the 2D pixel location) as declared above
    //To get the value needed then we need to get the equation of the following line
    // which contains the following points (0,-1) (1,1)
    // so if the value is 0 then we need to get -1 , if the value is 1 so we need to get 1
    // then the equation is like 2*X - 1 
    // if the X = 0 then we would have -1
    // if the X = 1 then we would have 1
    // then our equation is right, then we need to get the square of the length of the new point
    frag_color /= (1+(length(2*tex_coord -1 )*length(2*tex_coord -1 )));
}