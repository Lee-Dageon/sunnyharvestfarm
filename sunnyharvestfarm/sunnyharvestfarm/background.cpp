#include "common.h"
#include "globals.h"
#include "background.h"
#include "draw_cube.h"

// 발판 리스트
std::vector<glm::vec3> platforms;

// 발판 추가 함수
void addPlatform(glm::vec3 position) {
    platforms.push_back(position);
}

void drawPlatformXZ(glm::vec3 position, float radius, GLuint shaderProgram) {
    // 쉐이더 프로그램 활성화
    glUseProgram(shaderProgram);

    // 모델 행렬 생성 (위치 이동 및 회전)
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position); // 위치로 이동
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // X축 기준 -90도 회전

    // 쉐이더로 모델 행렬 전달
    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    // 쉐이더로 색상 전달 (빨간색)
    GLuint colorLoc = glGetUniformLocation(shaderProgram, "objectColor");
    glUniform4f(colorLoc, 1.0f, 0.0f, 0.0f, 1.0f); // RGBA: 빨간색, 불투명

    // 원형 발판 생성
    GLUquadric* quad = gluNewQuadric();
    gluDisk(quad, 0.0f, radius, 36, 1); // 반지름 설정, 분할 36
    gluDeleteQuadric(quad);
}

void renderPlatforms(float radius, GLuint shaderProgram) {
    for (const auto& position : platforms) {
        drawPlatformXZ(position, radius, shaderProgram); // 쉐이더 사용
    }
}


void checkPlayerOnPlatform(glm::vec3 playerPosition, glm::vec3 playerSize, float platformRadius) {
    dropping = false; // 기본값은 떨어지는 상태

    for (const auto& platform : platforms) {
        float distanceX = std::abs(playerPosition.x - platform.x);
        float distanceZ = std::abs(playerPosition.z - platform.z);

        // 플랫폼 위에 있는지 확인 (단순 원형 충돌)
        if (distanceX <= platformRadius + playerSize.x / 2 &&
            distanceZ <= platformRadius + playerSize.z / 2 &&
            playerPosition.y <= platform.y + 0.6f && // 플랫폼 위
            playerPosition.y >= platform.y) {       // 플랫폼 바로 위
            dropping = true; // 플랫폼 위에 있으므로 떨어지는 상태 해제
            return;
        }
    }
}


// 바닥을 그리는 함수
void drawGround(GLuint shaderProgram) {
    glUseProgram(shaderProgram);
    // 색상 고유 설정
    GLuint colorLoc = glGetUniformLocation(shaderProgram, "objectColor");
    glUniform4f(colorLoc, 0.3f, 0.6f, 0.3f, 1.0f); // 녹색

    // 사각형 정점 데이터 (XZ 평면)
    float vertices[] = {
        // Positions           // Colors
        -ground_size, -0.01f, -ground_size,  0.3f, 0.6f, 0.3f, // Bottom-left
         ground_size, -0.01f, -ground_size,  0.3f, 0.6f, 0.3f, // Bottom-right
         ground_size, -0.01f,  ground_size,  0.3f, 0.6f, 0.3f, // Top-right
        -ground_size, -0.01f,  ground_size,  0.3f, 0.6f, 0.3f  // Top-left
    };

    // 사각형 인덱스 데이터
    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // VAO 설정
    glBindVertexArray(VAO);

    // VBO에 정점 데이터 바인딩
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // EBO에 인덱스 데이터 바인딩
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 위치 속성
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 색상 속성
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // 그리기
    glUseProgram(shaderProgram);

    glm::mat4 model = glm::mat4(1.0f);
    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // 정리
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

// Counter를 그리는 함수
void drawCounter(GLuint shaderProgram) {
    glUseProgram(shaderProgram);

    // 색상 고유 설정
    GLuint colorLoc = glGetUniformLocation(shaderProgram, "objectColor");
    glUniform4f(colorLoc, 0.2f, 0.8f, 0.8f, 0.5f); // 청록색, 반투명

    // 블렌딩 활성화 (반투명)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // 모델 변환 행렬
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.5f, 1.5f)); // 위치 설정
    model = glm::scale(model, glm::vec3(5.0f, 1.0f, 2.0f));     // 크기 설정

    // Counter 색상 및 투명도 설정
    glm::vec3 color(0.2f, 0.6f, 0.8f); // 청록색
    float transparency = 0.2f;         // 반투명도

    // drawghostCube 호출
    drawCube(glm::vec3(0.0f, 0.5f, 1.5f), glm::vec3(5.0f, 1.0f, 2.0f), color, lightPos, viewPos, model);

    // 블렌딩 해제
    glDisable(GL_BLEND);
}

void drawSun(GLuint shaderProgram) {
    glUseProgram(shaderProgram);

    // 색상 고유 설정
    GLuint colorLoc = glGetUniformLocation(shaderProgram, "objectColor");
    glUniform4f(colorLoc, 1.0f, 1.0f, 0.0f, 1.0f); // 청록색, 반투명

    glPushMatrix();

    // 태양 위치 설정
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    // 조명을 비활성화하여 태양 렌더링
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 0.0f); // 태양 색상: 밝은 노란색
    GLUquadric* quad = gluNewQuadric();
    gluSphere(quad, 0.5, 32, 32); // 반지름 0.5, 세그먼트 32x32
    gluDeleteQuadric(quad);

    // 조명 다시 활성화
    glEnable(GL_LIGHTING);

    glPopMatrix();
}

ChickenPlace::ChickenPlace(glm::vec3 point1, glm::vec3 point2)
    : point1(point1), point2(point2) {}

void ChickenPlace::render(unsigned int shaderProgram) {
    glUseProgram(shaderProgram);
    // 색상 설정
    glm::vec3 color(0.5f, 0.8f, 0.5f); // 연한 녹색
    GLuint colorLoc = glGetUniformLocation(shaderProgram, "objectColor");
    glUniform4f(colorLoc, color.r, color.g, color.b, 1.0f);

    // 영역의 중심 계산
    glm::vec3 center = glm::vec3(
        (point1.x + point2.x) / 2.0f,
        (point1.y + point2.y) / 2.0f,
        (point1.z + point2.z) / 2.0f
    );

    // 영역의 크기 계산
    glm::vec3 size = glm::vec3(
        std::abs(point2.x - point1.x),
        std::abs(point2.y - point1.y),
        std::abs(point2.z - point1.z)
    );

    // 색상 및 모델 변환 행렬 설정

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, center);
    model = glm::scale(model, size);

    // `drawCube` 호출로 영역 렌더링
    drawCube(center, size, color, lightPos, viewPos, model);
}
