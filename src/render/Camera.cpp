// Camera.cpp
#include "Camera.h"
#include <algorithm>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
    MovementSpeed(SPEED),//移动速度
	MouseSensitivity(SENSITIVITY),//鼠标灵敏度
	Zoom(ZOOM)//缩放
{
	Position = position;//相机位置
    WorldUp = up;//世界坐标系中的上向量
	Yaw = yaw;//偏航角
	Pitch = pitch;//俯仰角
    UpdateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ,
    float upX, float upY, float upZ,
    float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
	MovementSpeed(SPEED),//移动速度
	MouseSensitivity(SENSITIVITY),// 鼠标灵敏度
	Zoom(ZOOM)//缩放
{
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
	Pitch = pitch;// 更新相机向量
    UpdateCameraVectors(); 
}

glm::mat4 Camera::GetViewMatrix()//获取视图矩阵
{
    return glm::lookAt(Position, Position + Front, Up);
}

glm::mat4 Camera::GetProjectionMatrix(float aspectRatio, float nearPlane, float farPlane)//获取投影矩阵
{
    return glm::perspective(glm::radians(Zoom), aspectRatio, nearPlane, farPlane);
}

void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    glm::vec3 FrontXZ = glm::normalize(glm::vec3(Front.x, 0.0f, Front.z));
    switch (direction) {
    case CameraMovement::FORWARD:
		Position += FrontXZ * velocity;// 前进
        break;
    case CameraMovement::BACKWARD:
		Position -= FrontXZ * velocity;// 后退
        break;
    case CameraMovement::LEFT:
		Position -= Right * velocity;// 左移
        break;
    case CameraMovement::RIGHT:
		Position += Right * velocity;// 右移
        break;
    case CameraMovement::UP:
		Position += glm::normalize(WorldUp) * velocity;// 上移
        break;
    case CameraMovement::DOWN:
		Position -= glm::normalize(WorldUp) * velocity;// 下移
        break;
    }
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch)
{
    xOffset *= MouseSensitivity;
    yOffset *= MouseSensitivity;

    Yaw += xOffset;
    Pitch += yOffset;

    if (constrainPitch) {
        ConstrainPitch();
    }

    UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float yOffset)
{
    Zoom -= yOffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}

void Camera::UpdateCameraVectors()
{
    // 计算新的Front向量
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);

    // 重新计算Right和Up向量
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}

void Camera::ConstrainPitch()
{
    if (Pitch > 89.5f)
        Pitch = 89.5f;
    if (Pitch < -89.5f)
        Pitch = -89.5f;
}

glm::vec3 Camera::GetScreenCenterRayDirection(int Width, int Height) {
    int screenWidth = Width;
    int screenHeight = Height;

    // 屏幕中心NDC
    float ndcX = 0.0f;  // 中心点的X NDC坐标是0
    float ndcY = 0.0f;  // 中心点的Y NDC坐标是0

    // 方法1：通过矩阵计算（最准确）
    glm::mat4 view = GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f),
        (float)screenWidth / screenHeight,
        0.1f, 100.0f);

    glm::mat4 inverseVP = glm::inverse(projection * view);

    glm::vec4 rayStart_NDC(ndcX, ndcY, -1.0f, 1.0f);
    glm::vec4 rayEnd_NDC(ndcX, ndcY, 1.0f, 1.0f);

    glm::vec4 rayStart_World = inverseVP * rayStart_NDC;
    glm::vec4 rayEnd_World = inverseVP * rayEnd_NDC;

    rayStart_World /= rayStart_World.w;
    rayEnd_World /= rayEnd_World.w;

    return glm::normalize(glm::vec3(rayEnd_World) - glm::vec3(rayStart_World));
}

glm::vec3 Camera::GetCorrectRayDirection() {
    float pitchRad = glm::radians(Pitch);
    float yawRad = glm::radians(Yaw);

    // 原始方向
    glm::vec3 originalDir;
    originalDir.x = cos(pitchRad) * sin(yawRad);
    originalDir.y = sin(pitchRad);
    originalDir.z = cos(pitchRad) * cos(yawRad);

    // 计算校正量（三次函数）
    float a = 0.1f;
    float b = 0.00f;
    float c = -0.3f;
    float d = 0.0f;
    float deltaPitch = 0.0f;

    deltaPitch = a * pitchRad * pitchRad * pitchRad
        + b * pitchRad * pitchRad
        + c * pitchRad
        + d;

    //if (pitchRad > 1.55f || pitchRad < -1.55f) {
    //    deltaPitch = 0.0f; // 超过±90度后不再校正
    //}

    // 调试输出（可以注释掉）
    static int frameCount = 0;
    if (++frameCount % 60 == 0) {  // 每秒输出几次
        printf("Pitch: %.2f° -> Correction: %.4f rad (%.2f°)\n",
            Pitch, deltaPitch, glm::degrees(deltaPitch));
    }
    //deltaPitch = 0.0f;
    // 使用校正后的俯仰角重新计算
    float correctedPitch = pitchRad + deltaPitch;
    std::cout << pitchRad << "俯仰角" << std::endl;

    glm::vec3 direction;
    direction.x = cos(correctedPitch) * sin(yawRad);
    direction.y = sin(correctedPitch);
    direction.z = cos(correctedPitch) * cos(yawRad);

    return glm::normalize(direction);
}

// 或者如果你习惯另一种坐标系
glm::vec3 Camera::GetCorrectRayDirectionAlt() {
    float pitchRad = glm::radians(Pitch);
    float yawRad = glm::radians(Yaw);
    float pitchCorrection = glm::radians(0.0f);
    glm::vec3 direction;
    direction.x = cos(yawRad) * cos(pitchRad);
    direction.y = sin(pitchRad)+ pitchCorrection;
    direction.z = sin(yawRad) * cos(pitchRad);

    return glm::normalize(direction);
}


