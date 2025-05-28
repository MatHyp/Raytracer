#include "./rt.h"
#include "./BVH.hpp"

void BVH::build(std::vector<CObject *> &objects)
{
    // std::cout << "Building BVH with " << objects.size() << " objects\n";
    root = buildRecursive(objects, 0);
}

std::unique_ptr<BVHNode> BVH::buildRecursive(std::vector<CObject *> &objects, int depth)
{
    // Debug indentation based on depth
    std::string indent(depth * 2, ' ');

    // std::cout << indent << "🛠️ Depth " << depth
    //           << ", processing " << objects.size()
    //           << " objects\n";

    auto node = std::make_unique<BVHNode>();

    // Base case: leaf node
    if (objects.size() <= 2)
    {
        node->objects = objects;

        // Handle empty case
        if (objects.empty())
        {
            // std::cout << indent << "  🍂 Empty leaf node!\n";
            node->box = AABB(glm::vec3(0), glm::vec3(0));
        }
        else
        {
            // Compute and print AABB for leaf
            node->box = computeSurroundingBox(objects);
            // std::cout << indent << "  🍂 Leaf node with " << objects.size()
            //           << " objects\n";
            // std::cout << indent << "  📦 AABB min: ("
            //           << node->box.min.x << ", "
            //           << node->box.min.y << ", "
            //           << node->box.min.z << ")\n";
            // std::cout << indent << "  📦 AABB max: ("
            //           << node->box.max.x << ", "
            //           << node->box.max.y << ", "
            //           << node->box.max.z << ")\n";
        }
        return node;
    }

    std::vector<std::pair<CObject *, glm::vec3>> objCentroids;
    for (CObject *obj : objects)
    {
        AABB box = obj->getBoundingBox();
        glm::vec3 centroid = 0.5f * (box.min + box.max);
        objCentroids.emplace_back(obj, centroid);
    }

    // 2. Choose split axis
    std::vector<glm::vec3> centroidsOnly;
    for (auto &pair : objCentroids)
        centroidsOnly.push_back(pair.second);

    int axis = chooseSplitAxis(centroidsOnly);
    // std::cout << indent << "  🔀 Splitting along axis " << axis
    //           << " (0=X, 1=Y, 2=Z)\n";

    // 3. Sort objects
    std::sort(objCentroids.begin(), objCentroids.end(),
              [axis](const auto &a, const auto &b)
              {
                  return a.second[axis] < b.second[axis];
              });

    // 4. Split into left/right
    size_t mid = objCentroids.size() / 2;
    std::vector<CObject *> leftObjects, rightObjects;
    for (size_t i = 0; i < mid; ++i)
        leftObjects.push_back(objCentroids[i].first);
    for (size_t i = mid; i < objCentroids.size(); ++i)
        rightObjects.push_back(objCentroids[i].first);

    // std::cout << indent << "  ⇥ Left child: " << leftObjects.size() << " objects\n";
    // std::cout << indent << "  ⇥ Right child: " << rightObjects.size() << " objects\n";

    // 5. Recursive build
    node->left = buildRecursive(leftObjects, depth + 1);
    node->right = buildRecursive(rightObjects, depth + 1);

    // 6. Merge AABBs
    node->box = AABB::surroundingBox(node->left->box, node->right->box);

    // std::cout << indent << "  🧩 Merged AABB:\n";
    // std::cout << indent << "    min: (" << node->box.min.x << ", "
    //           << node->box.min.y << ", " << node->box.min.z << ")\n";
    // std::cout << indent << "    max: (" << node->box.max.x << ", "
    //           << node->box.max.y << ", " << node->box.max.z << ")\n";

    return node;
}

AABB BVH::computeSurroundingBox(const std::vector<CObject *> &objects)
{
    if (objects.empty())
    {
        // Handle empty input gracefully
        return AABB(glm::vec3(0), glm::vec3(0));
    }

    AABB box = objects[0]->getBoundingBox();

    for (size_t i = 1; i < objects.size(); ++i)
    {
        AABB currentBox = objects[i]->getBoundingBox();
        box = AABB::surroundingBox(box, currentBox);
    }

    return box;
}

int BVH::chooseSplitAxis(const std::vector<glm::vec3> &centroids)
{
    if (centroids.empty())
        return 0; // default fallback

    glm::vec3 minPoint = centroids[0];
    glm::vec3 maxPoint = centroids[0];

    for (const auto &c : centroids)
    {
        minPoint = glm::min(minPoint, c);
        maxPoint = glm::max(maxPoint, c);
    }

    glm::vec3 extent = maxPoint - minPoint;

    // Choose the axis with the largest extent
    if (extent.x > extent.y && extent.x > extent.z)
        return 0; // X
    else if (extent.y > extent.z)
        return 1; // Y
    else
        return 2; // Z
}

AABB AABB::surroundingBox(const AABB &a, const AABB &b)
{
    glm::vec3 small = glm::min(a.min, b.min);
    glm::vec3 big = glm::max(a.max, b.max);
    return AABB(small, big);
}

// Travers

bool BVH::intersectRecursive(BVHNode *node, const CRay &ray, float &closestT, CObject *&hitObj, glm::vec3 &hitPoint, glm::vec3 &normal, float tMin, float tMax) const
{
    if (!node->box.intersect(ray, tMin, tMax))
        return false;

    bool hit = false;

    if (node->isLeaf())
    {
        for (CObject *obj : node->objects)
        {
            float t = obj->intersect(ray);
            if (t > 0.0001f && t < closestT)
            {
                closestT = t;
                hitObj = obj;
                hitPoint = ray.pos + ray.dir * t;
                normal = obj->normal(hitPoint);
                hit = true;
            }
        }
    }
    else
    {
        bool hitLeft = intersectRecursive(node->left.get(), ray, closestT, hitObj, hitPoint, normal, tMin, tMax);
        bool hitRight = intersectRecursive(node->right.get(), ray, closestT, hitObj, hitPoint, normal, tMin, tMax);
        hit = hitLeft || hitRight;
    }

    return hit;
}

bool BVH::intersect(const CRay &ray, float &closestT, CObject *&hitObj, glm::vec3 &hitPoint, glm::vec3 &normal) const
{
    closestT = FLT_MAX;
    hitObj = nullptr;

    return intersectRecursive(root.get(), ray, closestT, hitObj, hitPoint, normal, 0.001f, FLT_MAX);
}

bool AABB::intersect(const CRay &ray, float tMin, float tMax) const
{
    for (int a = 0; a < 3; a++)
    {
        float invD = 1.0f / ray.dir[a];
        float t0 = (min[a] - ray.pos[a]) * invD;
        float t1 = (max[a] - ray.pos[a]) * invD;

        if (invD < 0.0f)
            std::swap(t0, t1);

        tMin = t0 > tMin ? t0 : tMin;
        tMax = t1 < tMax ? t1 : tMax;

        if (tMax <= tMin)
            return false;
    }
    return true;
}
