#include "customer.h"
#include "draw_cube.h"
#include "common.h"
#include "item.h"

Customer::Customer(glm::vec3 position, glm::vec3 size, glm::vec3 color)
    : position(position), size(size), color(color), rotationY(0.0f), speed(1.0f),
    leftArmSwing(0.0f), rightArmSwing(0.0f), leftLegSwing(0.0f), rightLegSwing(0.0f),
    swingDirection(true), isMoving(true), isResetting(false), isCarryingEggs(false),
    isRotating(false), rotationStage(0), intermediateTarget(glm::vec3(0.0f)) { // 초기화 추가

    targetPosition = glm::vec3(0.0f, 0.5f, 5.0f);
    spawnPosition = position; // 스폰 위치 저장

    // 신체 파트 초기화
    bodyOffset = glm::vec3(position.x, position.y, 0.0f);
    bodySize = glm::vec3(1.0f, 1.0f, 0.5f);

    headOffset = glm::vec3(0.0f, position.y + 1.25f, 0.0f);
    headSize = glm::vec3(1.5f, 1.5f, 1.5f);

    armSize = glm::vec3(0.25f, 1.0f, 0.3f);
    leftArmOffset = glm::vec3(position.x - 0.75f, position.y, 0.0f);
    rightArmOffset = glm::vec3(position.x + 0.75f, position.y, 0.0f);

    legSize = glm::vec3(0.3f, 0.5f, 0.3f);
    leftLegOffset = glm::vec3(position.x - 0.35f, position.y - 0.75f, 0.0f);
    rightLegOffset = glm::vec3(position.x + 0.35f, position.y - 0.75f, 0.0f);
}


void Customer::updatePosition(float deltaTime) {
    if (!isMoving && !isRotating) return;

    if (isRotating) {
        // 회전 애니메이션
        float rotationSpeed = 180.0f * deltaTime; // 180도 회전
        rotationY += rotationSpeed;

        if (rotationStage == 1 && rotationY >= 180.0f) {
            std::cout << "[DEBUG] 180-degree rotation complete." << std::endl;
            //std::cout << "[DEBUG] 180-degree rotation complete." << std::endl;
            rotationY = 180.0f;
            isRotating = false; // 회전 종료
            isMoving = true;    // 이동 시작
            targetPosition = spawnPosition; // 원래 스폰 위치로 이동
        }
    }
    else if (isMoving) {
        glm::vec3 direction = targetPosition - position;
        float distance = glm::length(direction);

        if (distance > 0.01f) { // 목표 지점에 도달하지 않았다면
            glm::vec3 moveDirection = glm::normalize(direction);
            position += moveDirection * speed * deltaTime;
        }
        else {
            position = targetPosition; // 목표 위치에 정확히 도달
            isMoving = false;

            if (targetPosition == spawnPosition) {
                std::cout << "[DEBUG] Returned to spawn position." << std::endl;
                // std::cout << "[DEBUG] Returned to spawn position." << std::endl;
                isCarryingEggs = false; // 달걀 전달 완료
                rotationY = 0.0f; // 원래 방향으로 초기화
            }
        }
    }

    if (isCarryingEggs && !isRotating && !isMoving) {
        std::cout << "[DEBUG] Starting 180-degree rotation." << std::endl;
        //std::cout << "[DEBUG] Starting 180-degree rotation." << std::endl;
        isRotating = true;
        rotationStage = 1;
    }
}





void Customer::takeEggs(std::vector<DropEgg>& dropEggs) {
    if (dropEggs.empty() || isCarryingEggs) return; // 이미 달걀을 들고 있으면 중단
    if (isCarryingEggs) {
        std::cout << "[DEBUG] Already carrying eggs, cannot take more." << std::endl;
        return; // 이미 달걀을 들고 있는 상태라면 중단
    }

    if (dropEggs.empty()) {
        std::cout << "[DEBUG] No eggs available to take." << std::endl;
        isCarryingEggs = false; // 가져갈 달걀이 없으면 상태 갱신
        return; // 달걀이 없으면 동작 중단
    }


    int takeCount = std::min(rand() % 8 + 3, (int)dropEggs.size()); // 3~10개의 랜덤 개수

    for (int i = 0; i < takeCount; ++i) {
        heldEggs.push_back(dropEggs.back()); // 맨 위 달걀 추가
        dropEggs.pop_back();                // 가져간 달걀 제거
    }

    isCarryingEggs = true; // 달걀 들기 상태 활성화
    isMoving = true;       // 스폰 위치로 이동 시작
    targetPosition = spawnPosition; // 목표를 스폰 위치로 변경
    std::cout << "[DEBUG] Customer took " << takeCount << " eggs and is returning." << std::endl;
}


void Customer::printState() const {
    std::cout << "Customer State:" << std::endl;
    std::cout << "  Position: (" << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;
    std::cout << "  Target Position: (" << targetPosition.x << ", " << targetPosition.y << ", " << targetPosition.z << ")" << std::endl;
    std::cout << "  Spawn Position: (" << spawnPosition.x << ", " << spawnPosition.y << ", " << spawnPosition.z << ")" << std::endl;
    std::cout << "  Rotation Y: " << rotationY << " degrees" << std::endl;
    std::cout << "  Is Moving: " << (isMoving ? "true" : "false") << std::endl;
    std::cout << "  Is Rotating: " << (isRotating ? "true" : "false") << std::endl;
    std::cout << "  Is Resetting: " << (isResetting ? "true" : "false") << std::endl;
    std::cout << "  Is Carrying Eggs: " << (isCarryingEggs ? "true" : "false") << std::endl;
    std::cout << "  Rotation Stage: " << rotationStage << std::endl;
    std::cout << "  Held Eggs: " << heldEggs.size() << std::endl;
    std::cout << "-----------------------------------------" << std::endl;
}

// 손님이 스폰 위치로 돌아왔는지 확인
bool Customer::isAtSpawnPosition() const {
    return glm::length(position - spawnPosition) < 0.1f;
}
void Customer::updateAnimation(float deltaTime) {
    float swingSpeed = 50.0f * deltaTime;

    if (isResetting) {
        // 팔다리 초기화 애니메이션
        leftArmSwing = glm::mix(leftArmSwing, 0.0f, 0.2f);
        rightArmSwing = glm::mix(rightArmSwing, 0.0f, 0.2f);
        leftLegSwing = glm::mix(leftLegSwing, 0.0f, 0.2f);
        rightLegSwing = glm::mix(rightLegSwing, 0.0f, 0.2f);

        if (std::abs(leftArmSwing) < 1.0f && std::abs(rightArmSwing) < 1.0f &&
            std::abs(leftLegSwing) < 1.0f && std::abs(rightLegSwing) < 1.0f) {
            leftArmSwing = rightArmSwing = 0.0f;
            leftLegSwing = rightLegSwing = 0.0f;
            isResetting = false; // 초기화 종료
        }
    }
    else if (isMoving || isCarryingEggs) {
        // 걷기 애니메이션
        if (swingDirection) {
            leftArmSwing += swingSpeed;
            rightArmSwing -= swingSpeed;
            leftLegSwing -= swingSpeed;
            rightLegSwing += swingSpeed;
        }
        else {
            leftArmSwing -= swingSpeed;
            rightArmSwing += swingSpeed;
            leftLegSwing += swingSpeed;
            rightLegSwing -= swingSpeed;
        }

        if (leftArmSwing >= 50.0f || leftArmSwing <= -50.0f) {
            swingDirection = !swingDirection;
        }
    }
}




bool Customer::isAtTarget() const {
    return glm::length(position - targetPosition) < 0.1f;
}

void Customer::render(glm::vec3 lightPos, glm::vec3 viewPos) const {
    // 모델 변환 행렬 생성 (몸통 기준)
    std::cout << "[DEBUG] Rendering customer at: ("
        << position.x << ", "
        << position.y << ", "
        << position.z << ")" << std::endl;

    glm::mat4 baseModel = glm::mat4(1.0f);
    baseModel = glm::translate(baseModel, position);
    baseModel = glm::rotate(baseModel, glm::radians(rotationY), glm::vec3(0.0f, 1.0f, 0.0f));

    // 몸통 렌더링
    glm::mat4 bodyModel = glm::translate(baseModel, bodyOffset);
    bodyModel = glm::scale(bodyModel, bodySize);
    drawCube(position + bodyOffset, bodySize, glm::vec3(0.9f, 0.5f, 0.2f), lightPos, viewPos, bodyModel); // 주황색 몸통

    // 머리 렌더링
    glm::mat4 headModel = glm::translate(baseModel, headOffset);
    headModel = glm::scale(headModel, headSize);
    drawCube(position + headOffset, headSize, glm::vec3(0.8f, 0.6f, 0.4f), lightPos, viewPos, headModel);

    // 왼팔 렌더링
    glm::mat4 leftArmModel = glm::translate(baseModel, leftArmOffset);
    leftArmModel = glm::translate(leftArmModel, glm::vec3(0.0f, armSize.y / 2.0f, 0.0f));
    leftArmModel = glm::rotate(leftArmModel, glm::radians(leftArmSwing), glm::vec3(1.0f, 0.0f, 0.0f));
    leftArmModel = glm::translate(leftArmModel, glm::vec3(0.0f, -armSize.y / 2.0f, 0.0f));
    leftArmModel = glm::scale(leftArmModel, armSize);
    drawCube(position + leftArmOffset, armSize, glm::vec3(0.9f, 0.4f, 0.2f), lightPos, viewPos, leftArmModel);

    // 오른팔 렌더링
    glm::mat4 rightArmModel = glm::translate(baseModel, rightArmOffset);
    rightArmModel = glm::translate(rightArmModel, glm::vec3(0.0f, armSize.y / 2.0f, 0.0f));
    rightArmModel = glm::rotate(rightArmModel, glm::radians(rightArmSwing), glm::vec3(1.0f, 0.0f, 0.0f));
    rightArmModel = glm::translate(rightArmModel, glm::vec3(0.0f, -armSize.y / 2.0f, 0.0f));
    rightArmModel = glm::scale(rightArmModel, armSize);
    drawCube(position + rightArmOffset, armSize, glm::vec3(0.9f, 0.4f, 0.2f), lightPos, viewPos, rightArmModel);

    // 왼다리 렌더링
    glm::mat4 leftLegModel = glm::translate(baseModel, leftLegOffset);
    leftLegModel = glm::translate(leftLegModel, glm::vec3(0.0f, legSize.y / 2.0f, 0.0f));
    leftLegModel = glm::rotate(leftLegModel, glm::radians(leftLegSwing), glm::vec3(1.0f, 0.0f, 0.0f));
    leftLegModel = glm::translate(leftLegModel, glm::vec3(0.0f, -legSize.y / 2.0f, 0.0f));
    leftLegModel = glm::scale(leftLegModel, legSize);
    drawCube(position + leftLegOffset, legSize, glm::vec3(0.7f, 0.3f, 0.2f), lightPos, viewPos, leftLegModel);

    // 오른다리 렌더링
    glm::mat4 rightLegModel = glm::translate(baseModel, rightLegOffset);
    rightLegModel = glm::translate(rightLegModel, glm::vec3(0.0f, legSize.y / 2.0f, 0.0f));
    rightLegModel = glm::rotate(rightLegModel, glm::radians(rightLegSwing), glm::vec3(1.0f, 0.0f, 0.0f));
    rightLegModel = glm::translate(rightLegModel, glm::vec3(0.0f, -legSize.y / 2.0f, 0.0f));
    rightLegModel = glm::scale(rightLegModel, legSize);
    drawCube(position + rightLegOffset, legSize, glm::vec3(0.7f, 0.3f, 0.2f), lightPos, viewPos, rightLegModel);

    // 들고 있는 달걀 렌더링
    glm::vec3 eggBaseOffset = glm::vec3(0.0f, size.y + 1.0f, 0.0f); // 머리 위
    float eggStackOffset = 0.6f;
    for (size_t i = 0; i < heldEggs.size(); ++i) {
        glm::vec3 eggPosition = position + eggBaseOffset + glm::vec3(0.0f, i * eggStackOffset, 0.0f);
        heldEggs[i].renderAtPosition(eggPosition, lightPos, viewPos);
    }
}
glm::vec3 Customer::getPosition() const {
    return position; // 손님의 현재 위치를 반환
}
