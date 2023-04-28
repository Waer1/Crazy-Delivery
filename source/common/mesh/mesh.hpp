#pragma once

#include <glad/gl.h>
#include "vertex.hpp"

namespace our {

    #define ATTRIB_LOC_POSITION 0
    #define ATTRIB_LOC_COLOR    1
    #define ATTRIB_LOC_TEXCOORD 2
    #define ATTRIB_LOC_NORMAL   3

    class Mesh {
        // Here, we store the object names of the 3 main components of a mesh:
        // A vertex array object, A vertex buffer and an element buffer
        unsigned int VBO, EBO; // Vertex Buffer Object, Element Buffer Object
        unsigned int VAO; // Vertex Array Object
        // We need to remember the number of elements that will be draw by glDrawElements 
        GLsizei elementCount;
    public:

        // The constructor takes two vectors:
        // - vertices which contain the vertex data.
        // - elements which contain the indices of the vertices out of which each rectangle will be constructed.
        // The mesh class does not keep a these data on the RAM. Instead, it should create
        // a vertex buffer to store the vertex data on the VRAM,
        // an element buffer to store the element data on the VRAM,
        // a vertex array object to define how to read the vertex & element buffer during rendering 
        Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& elements)
        {
            // Generate the VAO, VBO and EBO
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            // Bind the VAO
            glBindVertexArray(VAO);

            // Bind the VBO
            glBindBuffer(GL_ARRAY_BUFFER, VBO);

            // Copy the vertex data to the VBO by using glBufferData & passing the array buffer, vertices size
            // by multiplying the size of each Vertex with their count, then pass the data itself & finally set
            // the usage hint to GL_STATIC_DRAW
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

            // Bind the EBO
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

            // Copy the elements data to the EBO and pass the same parameters as done with vertices
            // except the Element array buffer as the first argument & that each element in the elements
            // data is of type unsigned int
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(unsigned int), elements.data(), GL_STATIC_DRAW);

						// ============================================================================
            // ===============Set the Attribute Pointers for the vertex data===============

						// Position
            // Using the glVertexAttribPointer
            // Set the first argument which is the index as the position location
            // Set the size of each entry where the position size is 3
            // Set the type of each entry in the array as GL_FLOAT
            // False for the normalized boolean (Not needed)
            // Bit size of the Vertex is set in the 5th argument
            // Finally, start from the position offset in the Vertex
            glVertexAttribPointer(ATTRIB_LOC_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
            glEnableVertexAttribArray(ATTRIB_LOC_POSITION);

						// Color
            glVertexAttribPointer(ATTRIB_LOC_COLOR, 3, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
            glEnableVertexAttribArray(ATTRIB_LOC_COLOR);

						// Texture Coordinates
            glVertexAttribPointer(ATTRIB_LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coord));
            glEnableVertexAttribArray(ATTRIB_LOC_TEXCOORD);

						// Normal
            glVertexAttribPointer(ATTRIB_LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
            glEnableVertexAttribArray(ATTRIB_LOC_NORMAL);
						// ============================================================================

            // Unbind the VAO, VBO and EBO
            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            // Store the number of elements in elementCount
            elementCount = elements.size();
        }

        // This function should render the mesh
        void draw() 
        {
            // Bind the VAO
            glBindVertexArray(VAO);

            // Draw the elements by specifying that we want to draw triangles and give
            // the number of elements. Then, the type is unsigned int & the final argument as 0 for indices
            glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, 0);

            // Unbind the VAO
            glBindVertexArray(0);
        }

        // This function should delete the vertex & element buffers and the vertex array object
        ~Mesh(){
            //TODO: (Req 2) Write this function
						
						// Delete vertex and element buffers
						glDeleteBuffers(1, &VBO);
						glDeleteBuffers(1, &EBO);

						// Delete vertex array
						glDeleteVertexArrays(1, &VAO);
        }

        // Delete the Mesh copy constructor & assignment operator
        Mesh(Mesh const &) = delete;
        Mesh &operator=(Mesh const &) = delete;
    };

}