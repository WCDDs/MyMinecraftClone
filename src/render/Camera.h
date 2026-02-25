#pragma once
// Camera.h
#ifndef CAMERA_H
#define CAMERA_H
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <GLFW/glfw3.h>

enum class CameraMovement {
	FORWARD,//前进
	BACKWARD,//后退
	LEFT,//左移
	RIGHT,//右移
	UP,//上移
    DOWN//下移
};

// 默认相机参数
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 5.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera {
public:
    // 相机属性
	glm::vec3 Position;//相机位置
	glm::vec3 Front;//相机的前向量
	glm::vec3 Up;//相机的上向量
	glm::vec3 Right;//相机的右向量
	glm::vec3 WorldUp;//世界坐标系中的上向量

    // 欧拉角
	float Yaw;//偏航角
	float Pitch;//俯仰角

    // 相机选项
	float MovementSpeed;//移动速度
	float MouseSensitivity;//鼠标灵敏度
	float Zoom;//缩放

    // 构造函数 - 向量形式
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw = YAW,
        float pitch = PITCH);

    // 构造函数 - 标量形式
    Camera(float posX, float posY, float posZ,
        float upX, float upY, float upZ,
        float yaw, float pitch);

    // 获取视图矩阵
    glm::mat4 GetViewMatrix();

    // 获取投影矩阵
    glm::mat4 GetProjectionMatrix(float aspectRatio, float nearPlane = 0.1f, float farPlane = 100.0f);

    // 键盘输入处理
    void ProcessKeyboard(CameraMovement direction, float deltaTime);

    // 鼠标输入处理
    void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);

    // 鼠标滚轮处理
    void ProcessMouseScroll(float yOffset);

    // 获取相机信息
    glm::vec3 GetPosition() const { return Position; }
    float GetZoom() const { return Zoom; }

    // 设置相机位置
    void SetPosition(const glm::vec3& position) { Position = position; }

    // 更新相机向量（根据欧拉角重新计算）
    void UpdateCameraVectors();

private:
    // 限制俯仰角
    void ConstrainPitch();
};

#endif