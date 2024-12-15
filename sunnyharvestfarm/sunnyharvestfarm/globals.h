#pragma once
#ifndef GLOBALS_H
#define GLOBALS_H

#include <gl/glm/glm.hpp>
#include <GL/glew.h>
#include <vector>
#include "character.h"
#include "customer.h"

// OpenGL 관련 전역 변수
extern GLuint shaderProgram;
extern GLuint axisVAO, axisVBO;
extern GLuint VAO, VBO;
extern glm::mat4 view, projection;

// 카메라 관련 전역 변수
extern glm::vec3 lightPos;
extern glm::vec3 viewPos;
extern glm::vec3 cameraPos;
extern glm::vec3 cameraTarget;
extern glm::vec3 cameraUp;

extern float cameraOrbitRadius;
extern float cameraOrbitAngle;
extern float cameraSpeed;
extern float orbitSpeed;

// 로봇 상태 관련 전역 변수
extern glm::vec3 robotPosition;

extern float RobotX, RobotY, RobotZ;
extern bool walking;
extern float leftArmSwing, rightArmSwing;
extern float leftLegSwing, rightLegSwing;
extern bool swingDirection;

// 장애물 관련 전역 변수
extern glm::vec3 obstacles[3];
extern bool onObstacle;


// 기타 애니메이션 및 상태 변수
extern float moveStep;
extern bool key_w, key_a, key_s, key_d, key_j;

//배경 관련 전역 변수
extern float ground_size;
extern bool dropping;
#endif // GLOBALS_H

