#pragma once
#ifndef BACKGROUND_H
#define BACKGROUND_H

void drawGround(GLuint shaderProgram);
void drawSun(GLuint shaderProgram);
void drawCounter(GLuint shaderProgram); // Counter 추가

class ChickenPlace {
public:
    ChickenPlace(glm::vec3 point1, glm::vec3 point2);

    void render(unsigned int shaderProgram);

private:
    glm::vec3 point1; // 첫 번째 점
    glm::vec3 point2; // 두 번째 점
};

// 발판 관련 함수
void addPlatform(glm::vec3 position);
void renderPlatforms(float radius, GLuint shaderProgram);
void checkPlayerOnPlatform(glm::vec3 playerPosition, glm::vec3 playerSize, float platformRadius);

#endif