#include "character.h"
#include "collision_object.h"
// 생성자: 몸통과 각 부위의 상대적 위치 및 크기 초기화
Character::Character(glm::vec3 position, glm::vec3 size, glm::vec3 color)
    : position(position), size(size), color(color), rotationY(0.0f), targetRotationY(0.0f) {

    bodyOffset = glm::vec3(position.x, position.y, 0.0f); // 몸통 위
    bodySize = glm::vec3(1.0f, 1.0f, 0.5f);
    // 머리
    headOffset = glm::vec3(0.0f, position.y + 1.25f, 0.0f); // 몸통 위
    headSize = glm::vec3(1.5f, 1.5f, 1.5f);

    // 팔
    armSize = glm::vec3(0.25f, 1.0f, 0.3f);
    leftArmOffset = glm::vec3(position.x - 0.75f, position.y, 0.0f); // 몸통 왼쪽
    rightArmOffset = glm::vec3(position.x + 0.75f, position.y, 0.0f); // 몸통 오른쪽

    // 다리
    legSize = glm::vec3(0.3f, 0.5f, 0.3f);
    leftLegOffset = glm::vec3(position.x - 0.35f, position.y - 0.75, 0.0f); // 몸통 왼쪽 아래
    rightLegOffset = glm::vec3(position.x + 0.35f, position.y - 0.75, 0.0f); // 몸통 오른쪽 아래
}

glm::vec3 Character::getPosition() const {
    return position; // position 멤버 변수 반환
}
// 크기 반환
glm::vec3 Character::getSize() const {
    return size;
}
void Character::move(float dx, float dy, float dz, const CollisionManager& collisionManager) {
    glm::vec3 newPosition = position;

    // 이동 계산
    float radians = glm::radians(rotationY);
    newPosition.x += dx * cos(radians) - dz * sin(radians);
    newPosition.z += dx * sin(radians) + dz * cos(radians);
    newPosition.y += dy;

    // 충돌 감지
    if (!collisionManager.checkCollision(newPosition, size)) {
        position = newPosition; // 충돌이 없으면 이동
    }
}



void Character::setTargetRotation(float angle) {
    targetRotationY = angle;
}

void Character::updateRotation() {
    float rotationSpeed = 10.0f; // 회전 속도
    if (std::abs(rotationY - targetRotationY) > 0.1f) {
        // 서서히 목표 각도로 이동
        rotationY += (targetRotationY - rotationY) * 0.2f;
    }
    else {
        rotationY = targetRotationY; // 목표 각도에 도달하면 정지
    }
}

void Character::render(glm::vec3 lightPos, glm::vec3 viewPos) {

    // 모델 변환 행렬 생성 (몸통 기준)
    glm::mat4 baseModel = glm::mat4(1.0f);
    baseModel = glm::translate(baseModel, position);
    baseModel = glm::rotate(baseModel, glm::radians(rotationY), glm::vec3(0.0f, 1.0f, 0.0f));

    // 몸통 렌더링
    glm::mat4 bodyModel = glm::translate(baseModel, bodyOffset);
    bodyModel = glm::scale(bodyModel, bodySize);
    drawCube(position + bodyOffset, bodySize, glm::vec3(0.8f, 0.8f, 0.8f), lightPos, viewPos, bodyModel);

    // 머리 렌더링
    glm::mat4 headModel = glm::translate(baseModel, headOffset);
    headModel = glm::scale(headModel, headSize);
    drawCube(position + headOffset, headSize, glm::vec3(0.8f, 0.8f, 0.8f), lightPos, viewPos, headModel);

    // 왼팔 렌더링
    glm::mat4 leftArmModel = glm::translate(baseModel, leftArmOffset);
    leftArmModel = glm::translate(leftArmModel, glm::vec3(0.0f, +armSize.y / 2.0f, 0.0f)); // 회전축 이동
    leftArmModel = glm::rotate(leftArmModel, glm::radians(leftArmSwing), glm::vec3(1.0f, 0.0f, 0.0f));
    leftArmModel = glm::translate(leftArmModel, glm::vec3(0.0f, -armSize.y / 2.0f, 0.0f)); // 회전축 복원
    leftArmModel = glm::scale(leftArmModel, armSize);
    drawCube(position + leftArmOffset, armSize, glm::vec3(0.6f, 0.6f, 0.8f), lightPos, viewPos, leftArmModel);

    // 오른팔 렌더링
    glm::mat4 rightArmModel = glm::translate(baseModel, rightArmOffset);
    rightArmModel = glm::translate(rightArmModel, glm::vec3(0.0f, armSize.y / 2.0f, 0.0f)); // 회전축 이동
    rightArmModel = glm::rotate(rightArmModel, glm::radians(rightArmSwing), glm::vec3(1.0f, 0.0f, 0.0f));
    rightArmModel = glm::translate(rightArmModel, glm::vec3(0.0f, -armSize.y / 2.0f, 0.0f)); // 회전축 복원
    rightArmModel = glm::scale(rightArmModel, armSize);
    drawCube(position + rightArmOffset, armSize, glm::vec3(0.6f, 0.6f, 0.8f), lightPos, viewPos, rightArmModel);

    // 왼다리 렌더링
    glm::mat4 leftLegModel = glm::translate(baseModel, leftLegOffset);
    leftLegModel = glm::translate(leftLegModel, glm::vec3(0.0f, legSize.y / 2.0f, 0.0f)); // 회전축 이동
    leftLegModel = glm::rotate(leftLegModel, glm::radians(leftLegSwing), glm::vec3(1.0f, 0.0f, 0.0f));
    leftLegModel = glm::translate(leftLegModel, glm::vec3(0.0f, -legSize.y / 2.0f, 0.0f)); // 회전축 복원
    leftLegModel = glm::scale(leftLegModel, legSize);
    drawCube(position + leftLegOffset, legSize, glm::vec3(0.4f, 0.4f, 0.6f), lightPos, viewPos, leftLegModel);

    // 오른다리 렌더링
    glm::mat4 rightLegModel = glm::translate(baseModel, rightLegOffset);
    rightLegModel = glm::translate(rightLegModel, glm::vec3(0.0f, +legSize.y / 2.0f, 0.0f)); // 회전축 이동
    rightLegModel = glm::rotate(rightLegModel, glm::radians(rightLegSwing), glm::vec3(1.0f, 0.0f, 0.0f));
    rightLegModel = glm::translate(rightLegModel, glm::vec3(0.0f, -legSize.y / 2.0f, 0.0f)); // 회전축 복원
    rightLegModel = glm::scale(rightLegModel, legSize);
    drawCube(position + rightLegOffset, legSize, glm::vec3(0.4f, 0.4f, 0.6f), lightPos, viewPos, rightLegModel);
}

