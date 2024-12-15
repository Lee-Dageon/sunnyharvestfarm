#pragma once
#ifndef COLLISION_OBJECT_H
#define COLLISION_OBJECT_H

#include "common.h"

class CollisionObject {
public:
    CollisionObject(glm::vec3 position, glm::vec3 size)
        : position(position), size(size) {}

    glm::vec3 getPosition() const { return position; }
    glm::vec3 getSize() const { return size; }

protected:
    glm::vec3 position;
    glm::vec3 size;
};

class CollisionManager {
public:
    void addCollisionObject(const CollisionObject& obj) {
        collisionObjects.push_back(obj);
    }

    bool checkCollision(const glm::vec3& position, const glm::vec3& size) const {
        for (const auto& obj : collisionObjects) {
            if (checkAABBCollision(position, size, obj.getPosition(), obj.getSize())) {
                return true;
            }
        }
        return false;
    }

private:
    std::vector<CollisionObject> collisionObjects;

    bool checkAABBCollision(glm::vec3 pos1, glm::vec3 size1, glm::vec3 pos2, glm::vec3 size2) const {
        bool collisionX = pos1.x + size1.x >= pos2.x - size2.x && pos1.x - size1.x <= pos2.x + size2.x;
        bool collisionY = pos1.y + size1.y >= pos2.y - size2.y && pos1.y - size1.y <= pos2.y + size2.y;
        bool collisionZ = pos1.z + size1.z >= pos2.z - size2.z && pos1.z - size1.z <= pos2.z + size2.z;

        return collisionX && collisionY && collisionZ;
    }
};

#endif // COLLISION_OBJECT_H
#pragma once
