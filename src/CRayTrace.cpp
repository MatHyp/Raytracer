#include "rt.h"

using namespace glm;
using namespace std;


bool CRayTrace::compPrimaryRayMatrix(const CCamera& cam, glm::mat3& m) {
    vec3 look = cam.lookp - cam.eyep;

    vec3 u = cross(cam.up, look) / length(cross(cam.up, look));

    vec3 v = cross(u, look) / length(cross(u, look));

    vec3 o = (look / length(look)) * (cam.width / (2.0f * tan(radians(cam.fov / 2.0f)))) - ((cam.width / 2.0f) * u + (cam.height / 2.0f) * v);



    m = mat3{u, v, o};

    return true;
}


bool CRayTrace::rayTrace(const CScene& scene, CRay& ray, COutput& out) {

    float tmin = FLT_MAX;
    float EPS = 0.0001;
    bool is_intersection = false;
    CObject *hitObj = nullptr;
    float t = 0.0f;


    for (CObject * obj : scene.objectList) {

        t = obj->intersect(ray);

        if(t > EPS && t < tmin) {
            tmin = t;
            hitObj = obj;
            is_intersection = true;
        }

    }

    if (is_intersection) {
        out.col = {0,0.5,0};
    }else {
        out.col = {0,0,0};
        return false;
    }

    glm::vec3 p  = ray.pos + tmin * ray.dir;

    glm::vec3 color = {0,0,0};
    glm::vec3 n = hitObj->normal(p);
    glm::vec3 V = -ray.dir;

    for (const auto& light : scene.lightList) {
        glm::vec3 l = glm::normalize(light.pos - p);
        color += out.energy * light.color * hitObj->matAmbient;

        CRay shadowRay;

        shadowRay.pos = p + l * EPS;
        shadowRay.dir = l;

        float distanceToLight = glm::length(light.pos - shadowRay.pos);
        bool isShadow = false;

        for (CObject * obj : scene.objectList) {

            float shadowT = obj->intersect(shadowRay);

            if(shadowT > EPS && shadowT < distanceToLight) {
                isShadow = true;
                break;
            }
        }

        if(!isShadow) {
            float cos_angle = glm::dot(n, l);

            if(cos_angle > 0.001) {

                color += out.energy * light.color * hitObj->matDiffuse * cos_angle;
            }
        }
        glm::vec3 h = glm::normalize(l + V);

        float cos_beta = glm::dot(n, h);
        if(cos_beta > 0.001) {
            color += out.energy * light.color * hitObj->matSpecular * pow(cos_beta, hitObj->matShininess);
        }
    }



    if (hitObj->isTexture) {
        glm::vec2 uv = hitObj->textureMapping(hitObj->normal(p));
        glm::vec3 tex_col = CImage::getTexel(hitObj->texture, uv.x, uv.y);
        color *= tex_col;
    }



    if(hitObj->reflectance > 0 && out.tree < 1 && out.energy > 0.01) {
        COutput reflectOut = out;
        reflectOut.tree++;
        reflectOut.energy *= hitObj->reflectance;

        CRay secondary_ray = reflectedRay(ray,n,p);

        out.col = color;

        rayTrace(scene, secondary_ray, reflectOut);
        color += reflectOut.col * hitObj->reflectance;
    }

    out.col = color;

    return true;
}

CRay CRayTrace::reflectedRay(const CRay& ray,const glm::vec3& n,const glm::vec3& pos) {
    CRay reflected_ray;
    glm::vec3 v = ray.dir;
    glm::vec3 r = v - 2.0f * glm::dot(v, n) * n;


    reflected_ray.pos = pos + n * 0.001f;
    reflected_ray.dir = glm::normalize(r);

    return reflected_ray;
}

