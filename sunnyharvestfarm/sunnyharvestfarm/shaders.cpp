#include "shaders.h"
#include <iostream>
#include <fstream>
#include <sstream>

// 쉐이더 컴파일 에러 체크 함수
bool checkCompileErrors(GLuint shader, std::string type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "| ERROR::SHADER-COMPILATION-ERROR of type: " << type << "|\n"
                << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            return false;
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "| ERROR::PROGRAM-LINKING-ERROR of type: " << type << "|\n"
                << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            return false;
        }
    }
    return true;
}

// 쉐이더 소스 파일 로드 함수
std::string loadShaderSource(const char* filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open shader file: " << filePath << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// 쉐이더 프로그램 생성 함수
GLuint createShaderProgram(const char* vertexPath, const char* fragmentPath) {
    // 쉐이더 소스 읽기
    std::string vertexCode = loadShaderSource(vertexPath);
    std::string fragmentCode = loadShaderSource(fragmentPath);

    const char* vertexSource = vertexCode.c_str();
    const char* fragmentSource = fragmentCode.c_str();

    // 정점 쉐이더 컴파일
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);
    if (!checkCompileErrors(vertexShader, "VERTEX")) {
        return 0;
    }

    // 프래그먼트 쉐이더 컴파일
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);
    if (!checkCompileErrors(fragmentShader, "FRAGMENT")) {
        return 0;
    }

    // 쉐이더 프로그램 생성 및 링크
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    if (!checkCompileErrors(shaderProgram, "PROGRAM")) {
        return 0;
    }

    // 쉐이더 객체 삭제
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}
