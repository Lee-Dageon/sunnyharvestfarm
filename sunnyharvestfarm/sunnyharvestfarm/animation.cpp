#include "globals.h"
#include <cmath>
#include "animation.h"

// 태양의 공전 관련 변수
float lightOrbitRadius = 10.0f; // 공전 반경
float lightAngle = 0.0f;        // 공전 각도

// 태양의 위치 업데이트
void updateSunPosition() {
    lightAngle += 0.01f; // 공전 속도
    if (lightAngle > 360.0f) lightAngle -= 360.0f;

    // lightPos를 원형 경로로 갱신
    lightPos.x = lightOrbitRadius * std::cos(lightAngle);
    lightPos.z = lightOrbitRadius * std::sin(lightAngle);
    lightPos.y = 10.0f; // Y축 고정
}
