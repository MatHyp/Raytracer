//
// Created by mathy on 07.05.2025.
//

#ifndef WAVEFRONTPARSER_HPP
#define WAVEFRONTPARSER_HPP

#include "rt.h"

class WaveFrontParser {
    std::vector<glm::vec3> v;                   // Raw vertices (from "v" lines)
    std::vector<glm::vec3> vn_interpolation;    // Vertex normals (from "vn" lines)
    std::vector<glm::vec3> triangle_vertices;   // Processed triangles (from "f" lines)


    std::string _path;

public:
    // Constructors
    explicit WaveFrontParser(std::string path);

    // Parse the OBJ file
    void parseWavefront();

    // Getters
    const std::vector<glm::vec3>& getTriangleVertices() const;
    const std::vector<glm::vec3>& getNormals() const;
    const std::vector<glm::vec3>& getVertices() const;
    const std::string& getPath() const;
};



#endif //WAVEFRONTPARSER_HPP
