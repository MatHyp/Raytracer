#ifndef BVH_HPP
#define BVH_HPP

#include "rt.h"

class AABB
{
public:
    glm::vec3 min;
    glm::vec3 max;

    AABB() = default;
    AABB(const glm::vec3 &min, const glm::vec3 &max) : min(min), max(max) {}
    bool intersect(const CRay &ray, float tMin, float tMax) const;

    static AABB surroundingBox(const AABB &a, const AABB &b);
};

class BVHNode
{
public:
    AABB box;
    std::unique_ptr<BVHNode> left;
    std::unique_ptr<BVHNode> right;
    std::vector<CObject *> objects;

    bool isLeaf() const
    {
        return left == nullptr && right == nullptr;
    }
};

class BVH
{
public:
    std::unique_ptr<BVHNode> root;
    void build(std::vector<CObject *> &objects);
    bool intersect(const CRay &ray, float &closestT, CObject *&hitObj, glm::vec3 &hitPoint, glm::vec3 &normal) const;

private:
    AABB computeSurroundingBox(const std::vector<CObject *> &objects);
    int chooseSplitAxis(const std::vector<glm::vec3> &centroids);
    std::unique_ptr<BVHNode> buildRecursive(std::vector<CObject *> &objects, int depth);

    bool intersectRecursive(BVHNode *node, const CRay &ray, float &closestT, CObject *&hitObj, glm::vec3 &hitPoint, glm::vec3 &normal, float tMin, float tMax) const;
};

#endif
