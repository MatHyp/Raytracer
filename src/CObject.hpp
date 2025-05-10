#ifndef CObject_hpp
#define CObject_hpp

#include<fstream>
#include<iostream>

class CObject {
public:
    glm::vec3 matAmbient; ///< Ambient material parameter (<0,1> range).
    glm::vec3 matDiffuse; ///< Diffuce material parameter (<0,1> range).
    glm::vec3 matSpecular; ///< Specular material parameter (<0,1> range).
    float matShininess; ///< Glossines material parameter ((1,n> range).
    float reflectance; ///< Material reflectance (0 - no reflectance, 1 - full reflectance).
    bool isTexture; ///< Set to true for objects with textures.
    CImage texture; ///< Texture data.
    
    CObject() {
        reflectance = 0.0f;
        matAmbient = {0,0,0};
        matDiffuse = {0,0,0};
        matSpecular = {0,0,0};
        matShininess = 0;
        isTexture = false;
    }

    void wavefrontParser();

    // Intersection with ray.
    virtual float intersect(const CRay& ray) {
        return -1.0f;
    }
    
    // Normal vector at intersection point.
    virtual glm::vec3 normal(const glm::vec3& hit_pos) {
        glm::vec3 mret = glm::vec3(0,0,0);
        return mret;
    }
    
    // Normal vector at intersection point.
    virtual glm::vec3 normal() {
        glm::vec3 mret = glm::vec3(0,0,0);
        return mret;
    }

    // Texture mapping.
    virtual glm::vec2 textureMapping(const glm::vec3& hit_pos) {
        glm::vec2 uv = {0,0};
        return uv;
    }
    
};


class CSphere : public CObject {
public:
    float r; ///< Sphere radius.
    glm::vec3 pos; ///< Position of the sphere center.
    
    CSphere(const glm::vec3& pos, float r) {
        this->pos = pos;
        this->r = r;
    }
    CSphere() {
        r = 5.0f;
        pos = {0,0,0};
    }
    
    float intersect(const CRay& ray) override; // ray-sphere intersection
    glm::vec3 normal(const glm::vec3& hit_pos) override; //  normal vector
    glm::vec2 textureMapping(const glm::vec3& hit_pos) override;

};

class CTriangle : public CObject {
public:
    glm::vec3 v0;
    glm::vec3 v1;
    glm::vec3 v2;
    glm::vec3 n0, n1, n2;

    CTriangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2) {
        this->v0 = v0;
        this->v1 = v1;
        this->v2 = v2;
    }
    CTriangle() {
        v0 = {0,0,0};
        v1 = {0,0,0};
        v2 = {0,0,0};
    }
    
    float intersect(const CRay& ray) override; // ray-sphere intersection
    glm::vec3 normal(const glm::vec3& pos) override; //  normal vector
    
};


#endif