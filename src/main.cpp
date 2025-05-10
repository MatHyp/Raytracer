#include <iostream>
#include "rt.h"


int main (int argc, char * const argv[]) {
    CScene currentScene;

    currentScene.create();

    if(currentScene.cam.width == 0) {
        std::cout << "WARNING: scene not defined" << std::endl;
        return -1;
    }

    CRayTrace tracer;
    CRay mainRay;
    COutput results;

    glm::mat3 ray_matrix;
    CRayTrace::compPrimaryRayMatrix(currentScene.cam, ray_matrix);

    mainRay.dir = {0.5,0.5,-0.0};
    mainRay.pos = {0,0,0};

    glm::vec3 n(0,1.0,0);
    glm::vec3 pos = {0,0,0};

//  dla 0 wynik 0.07
//  dla 0.25 wynik 3.08
    //  dla 0.5 wynik 15.2
    //  dla 0.75 wynik 17.2
    //  dla 1 wynik 39.6

    CImage imageX(currentScene.cam.width, currentScene.cam.height);


    /// main loop
    for(int j = 0; j < currentScene.cam.height; j++) {
        for(int i = 0; i < currentScene.cam.width; i++) {

            /// position of the image point
            float fx = (float)i + 0.5f / float(currentScene.cam.width) * 2.0f - 1.0f;
            float fy = (float)j + 0.5f / float(currentScene.cam.height) * 2.0f - 1.0f;

            /// primary ray
            mainRay.pos = currentScene.cam.eyep;
            mainRay.dir = glm::normalize(ray_matrix * glm::vec3(fx,fy,1.0f));

            results.col = {0,0,0};
            results.energy = 1.0f;
            results.tree = 0;

            tracer.rayTrace(currentScene,mainRay,results);

            if(results.col.x > 1 || results.col.y > 1 || results.col.z > 1) {
                results.col = {1,1,1};
            }
            imageX.setPixel(i, j, results.col);

        }
    }



    imageX.save("../images/with-wavefront-obj.png", true);

    cv::imshow("intersection", imageX.getImage());
    cv::waitKey();
    return 0;
}
