#include "rt.h"
#include "CObject.hpp"


float CSphere::intersect(const CRay& ray) {
    glm::vec3 v = ray.pos - this->pos;

    float A = glm::dot(ray.dir, ray.dir);
    float B = 2.0f * glm::dot(v, ray.dir);
    float C = glm::dot(v, v) - (this->r * this->r);
    float delta = (B * B) - 4.0f * A * C;

    if (delta < 0.0f) {
        return -1.0f; // no intersection
    }

    float sqrtDelta = sqrtf(delta);
    float t1 = (-B - sqrtDelta) / (2.0f * A);
    float t2 = (-B + sqrtDelta) / (2.0f * A);

    if (t1 > 0 && t2 > 0) {
        return t1 < t2 ? t1 : t2;
    } else if (t1 > 0) {
        return t1;
    } else if (t2 > 0) {
        return t2;
    }

    return -1.0f; // both t1 and t2 are negative
}

glm::vec3 CSphere::normal(const glm::vec3& hit_pos) {
    glm::vec3 n;

    n = glm::normalize(hit_pos - this->pos);

    return n;
}

glm::vec2 CSphere::textureMapping(const glm::vec3& normal_vec) {
    glm::vec2 uv = {0,0};
    uv[0] = 0.5 + asin(normal_vec.x) / M_PI;
    uv[1] = 0.5 - asin(normal_vec.y) / M_PI;

    return uv;
}



float CTriangle::intersect(const CRay& ray) {
    float t = -1;
    glm::vec3 bary;

    bool hit = glm::intersectRayTriangle(ray.pos, ray.dir, v0, v1, v2, bary);

    if (hit) {
        t = bary.z;

        return t;
    }


    return t;
}

glm::vec3 CTriangle::normal(const glm::vec3& hit_pos) {
    glm::vec3 n;
    glm::vec3 u = this->v1 - this->v0;
    glm::vec3 v = this->v2 - this->v0;
    n = glm::normalize(glm::cross(u, v));

    return n;
}



