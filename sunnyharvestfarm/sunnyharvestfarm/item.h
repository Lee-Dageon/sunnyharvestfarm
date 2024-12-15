#ifndef ITEM_H
#define ITEM_H

#include "common.h"
#include "draw_cube.h"

class Egg {
public:
    Egg(glm::vec3 position, glm::vec3 size, glm::vec3 color);

    glm::vec3 getPosition() const;
    glm::vec3 getSize() const;
    glm::vec3 getColor() const; // 색상 반환
    void render(glm::vec3 lightPos, glm::vec3 viewPos) const;


protected: // 자식 클래스에서 접근 가능
    glm::vec3 position; // 위치
    glm::vec3 size;     // 크기
    glm::vec3 color;    // 색상
};

class HoldingEgg {
public:
    HoldingEgg(glm::vec3 startPosition, glm::vec3 targetOffset, glm::vec3 size, glm::vec3 color);

    void updatePosition(glm::vec3 basePosition, float deltaTime);
    void render(glm::vec3 lightPos, glm::vec3 viewPos) const;
    glm::vec3 getPosition() const;
    glm::vec3 getCurrentPosition() const;
    glm::vec3 getSize() const;
    glm::vec3 getColor() const; // 색상 반환

private:
    glm::vec3 currentPosition;
    glm::vec3 targetOffset;
    glm::vec3 size;
    glm::vec3 color;
};

class DropEgg {
public:
    DropEgg(glm::vec3 startPosition, glm::vec3 targetPosition, glm::vec3 size, glm::vec3 color);

    void updatePosition(float deltaTime);
    void render(glm::vec3 lightPos, glm::vec3 viewPos) const;
    bool isAtTarget() const;
    void renderAtPosition(glm::vec3 position, glm::vec3 lightPos, glm::vec3 viewPos) const;

private:
    glm::vec3 currentPosition; // 현재 위치
    glm::vec3 targetPosition;  // 목표 위치
    glm::vec3 size;            // 크기
    glm::vec3 color;           // 색상
};

// DropEgg 리스트 관리
void addDropEgg(glm::vec3 startPosition, glm::vec3 targetPosition, glm::vec3 size, glm::vec3 color);
void updateDropEggs(float deltaTime);
void renderDropEggs(glm::vec3 lightPos, glm::vec3 viewPos);

#endif // ITEM_H
#pragma once
