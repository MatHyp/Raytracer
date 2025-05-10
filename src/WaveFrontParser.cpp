#include "WaveFrontParser.hpp"

WaveFrontParser::WaveFrontParser(std::string path) : _path(std::move(path)) {}

void WaveFrontParser::parseWavefront()
{
    std::ifstream in(_path);
    if (!in.is_open())
    {
        std::cerr << "[WaveFrontParser] ERROR: Could not open file: " << _path << std::endl;
        return;
    }

    std::string line;
    while (std::getline(in, line))
    {
        if (line.rfind("v ", 0) == 0)
        {
            std::istringstream ss(line.substr(2));
            float x, y, z;
            ss >> x >> y >> z;
            v.emplace_back(x, y, z);
        }

        else if (line.rfind("vn ", 0) == 0)
        {
            std::istringstream ss(line.substr(3));
            float x, y, z;
            ss >> x >> y >> z;
            vn_interpolation.emplace_back(x, y, z);
        }
        if (line.rfind("vn ", 0) == 0)
        {
            std::istringstream ss(line.substr(3));
            float x, y, z;

            ss >> x >> y >> z;

            vn_interpolation.emplace_back(x, y, z);
        }
        else if (line.rfind("f ", 0) == 0)
        {
            std::istringstream ss(line.substr(2));
            std::string token;
            std::vector<int> faceVertices;

            while (ss >> token)
            {
                std::replace(token.begin(), token.end(), '/', ' ');
                std::istringstream vss(token);

                int vIdx, vtIdx, vnIdx;

                vss >> vIdx >> vtIdx >> vnIdx;


                if (vIdx < 1 || vIdx > v.size())
                {
                    std::cerr << "Invalid vertex index in face: " << vIdx << std::endl;
                    continue;
                }

                faceVertices.push_back(vIdx - 1); // Convert to 0-based index
            }

            if (faceVertices.size() >= 3)
            {
                for (size_t i = 1; i < faceVertices.size() - 1; ++i)
                {
                    triangle_vertices.push_back(v[faceVertices[0]]);
                    triangle_vertices.push_back(v[faceVertices[i]]);
                    triangle_vertices.push_back(v[faceVertices[i + 1]]);
                }
            }
            else
            {
                std::cerr << "Face with < 3 vertices ignored." << std::endl;
            }

        }
    }
}

// Getter implementations
const std::vector<glm::vec3> &WaveFrontParser::getTriangleVertices() const
{
    return triangle_vertices;
}

const std::vector<glm::vec3> &WaveFrontParser::getNormals() const
{
    return vn_interpolation;
}

const std::vector<glm::vec3> &WaveFrontParser::getVertices() const
{
    return v;
}

const std::string &WaveFrontParser::getPath() const
{
    return _path;
}

