#include "rt.h"

void CScene::create()
{
  lightList.clear();  // clears vector with the light data
  objectList.clear(); // clears vector of pointers to objects

  // Kamera
  cam.eyep = {0, -10, 50}; // Poprawiono pozycję eyep
  cam.lookp = {0, 0, 0};
  cam.up = {0, 0, 1};
  cam.fov = 10;
  cam.width = 900;
  cam.height = 800;

  // Źródła światła
  CLight light1(glm::vec3(-3, -2, 8));
  light1.color = {0.6, 0.6, 0.6};
  lightList.push_back(light1);

  // Drugie źródło światła (dodane)
  CLight light2(glm::vec3(5, -2, 8));
  light2.color = {0.3, 0.3, 0.3};
  lightList.push_back(light2);

  //
  WaveFrontParser parser("../obj-models/Suzanne.obj");
  parser.parseWavefront();

  const std::vector<glm::vec3> &triangleVertices = parser.getTriangleVertices();

  for (size_t i = 0; i + 2 < triangleVertices.size(); i += 3)
  {
    const glm::vec3 &v1 = triangleVertices[i];
    const glm::vec3 &v2 = triangleVertices[i + 1];
    const glm::vec3 &v3 = triangleVertices[i + 2];

    CTriangle *triangle = new CTriangle(v1, v2, v3);
    triangle->matAmbient = {0, 0.1, 0};
    triangle->matDiffuse = {0, 0.6, 0};
    triangle->matSpecular = {0.7, 0.7, 0.7};
    triangle->matShininess = 30;
    triangle->reflectance = 0;

    objectList.push_back(triangle);
  }

  bvh.build(objectList);
}
