#include "rt.h"

void CScene::create() {
    lightList.clear(); // clears vector with the light data
    objectList.clear(); // clears vector of pointers to objects


  // Kamera
    cam.eyep = {0, -20, 10}; // Poprawiono pozycję eyep
    cam.lookp = {0,0,0};
    cam.up = {0, 0, 1};
    cam.fov = 50;
    cam.width = 900;
    cam.height = 800;


    // Źródła światła
    CLight light1(glm::vec3(-3,-2,8));
    light1.color = {0.6,0.6,0.6};
    lightList.push_back(light1);

    // Drugie źródło światła (dodane)
    CLight light2(glm::vec3(5,-2,8));
    light2.color = {0.3,0.3,0.3};
    lightList.push_back(light2);


    //
    WaveFrontParser parser("../obj-models/cube.obj");
    parser.parseWavefront();

    const std::vector<glm::vec3>& triangleVertices  = parser.getTriangleVertices();

    for (size_t i = 0; i + 2 < triangleVertices.size(); i += 3) {
        const glm::vec3& v1 = triangleVertices[i];
        const glm::vec3& v2 = triangleVertices[i + 1];
        const glm::vec3& v3 = triangleVertices[i + 2];

        // Debug output
        // std::cout << "Creating triangle with vertices:\n"
        //           << "  v1: (" << v1.x << ", " << v1.y << ", " << v1.z << ")\n"
        //           << "  v2: (" << v2.x << ", " << v2.y << ", " << v2.z << ")\n"
        //           << "  v3: (" << v3.x << ", " << v3.y << ", " << v3.z << ")\n";

        // Create and configure triangle
        CTriangle* triangle = new CTriangle(v1, v2, v3);
        triangle->matAmbient = {0.1f, 0.1f, 0.1f};
        triangle->matDiffuse = {0.4f, 0.4f, 0.4f};
        triangle->matSpecular = {0.0f, 0.0f, 0.0f};
        triangle->matShininess = 0.0f;
        triangle->reflectance = 1.0f;

        objectList.push_back(triangle);
    }


    // Pierwsza kula (zielona)
    CSphere* sphere_green = new CSphere({-1, 0, 3}, 0.4);
    sphere_green->matAmbient = {0, 0.1, 0}; // Poprawiono kolor ambient
    sphere_green->matDiffuse = {0, 0.6, 0}; // Poprawiono kolor diffuse
    sphere_green->matSpecular = {0.7, 0.7, 0.7};
    sphere_green->matShininess = 30;
    sphere_green->reflectance = 0;
    objectList.push_back(sphere_green);
    //
    CSphere* sphere_red = new CSphere({-5, 0, 0}, 1.6);
    sphere_red->matAmbient = {0.1, 0.0, 0.0}; // Poprawiono zapis (3 składowe)
    sphere_red->matDiffuse = {0.6, 0.0, 0.0};
    sphere_red->matSpecular = {0.7, 0.7, 0.7};
    sphere_red->matShininess = 30;
    sphere_red->reflectance = 0;
    // Usunięto teksturę (nie ma w tabeli)
    objectList.push_back(sphere_red);
    //
    CSphere* sphere_blue = new CSphere({-3, -2, -2}, 0.6);
    sphere_blue->matAmbient = {0, 0, 0.1};
    sphere_blue->matDiffuse = {0, 0, 0.6};
    sphere_blue->matSpecular = {0.7, 0.7, 0.7};
    sphere_blue->matShininess = 30;
    sphere_blue->reflectance = 0;
    objectList.push_back(sphere_blue);

    CTriangle* triangle_background_left = new CTriangle({5, 5, -5}, {-5, 5, -5}, {-5, -5, -5});
    triangle_background_left->matAmbient = {0.1, 0.1, 0.1};
    triangle_background_left->matDiffuse = {0.4, 0.4, 0.4};
    triangle_background_left->matSpecular = {0, 0, 0};
    triangle_background_left->matShininess = 0;
    triangle_background_left->reflectance = 1; // Poprawiono z 0 na 1
    objectList.push_back(triangle_background_left);

    CTriangle* triangle_background_right = new CTriangle({5, 5, -5}, {-5, -5, -5}, {5, -5, -5});
    triangle_background_right->matAmbient = {0.1, 0.1, 0.1};
    triangle_background_right->matDiffuse = {0.4, 0.4, 0.4};
    triangle_background_right->matSpecular = {0, 0, 0};
    triangle_background_right->matShininess = 0;
    triangle_background_right->reflectance = 1; // Poprawiono z 0 na 1
    objectList.push_back(triangle_background_right);



}
