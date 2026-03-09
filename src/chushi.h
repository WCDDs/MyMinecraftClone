#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL2.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <filesystem>
#include <stb_image.h>
#include "world/Block.h"
#include "render/Utils.h"
#include "render/Camera.h"

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = 400, lastY = 300;// 鼠标初始位置（窗口中心）
bool firstMouse = true;// 鼠标第一次移动的标志
float deltaTime = 0.0f;// 当前帧与上一帧的时间差
float lastFrame = 0.0f;// 上一帧的时间

GLuint textureID;// 纹理ID
int Texwidth, Texheight, nrChannels;
unsigned char* Texdata;
GLuint texturezuobiao;// 纹理坐标纹理

const int instanceCount = 50;

std::vector<glm::mat4> instanceMatrices;  // 存储所有实例的变换矩阵
size_t maxInstances = 20000;  // 预分配的最大实例数
size_t ringIndex = 0;  // 环形缓冲区索引
const int shenchenqukuaidaxiao = 0;// 生成的区块大小
const int jianchaqukuaifanwui = 8;// 检查区块范围（单位：方块）

unsigned int skyboxVAO, skyboxVBO;// 天空盒顶点数组对象和顶点缓冲对象
unsigned int cubemapTexture;// 立方体贴图纹理ID
unsigned int skyboxShaderProgram;// 天空盒着色器程序ID

float skyboxVertices[] = {
    // 背面 (z负方向) - 正确
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    // 左面 (x负方向) - 正确
    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    // 右面 (x正方向) - 正确
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

     // 前面 (z正方向) - 正确
     -1.0f, -1.0f,  1.0f,
     -1.0f,  1.0f,  1.0f,
      1.0f,  1.0f,  1.0f,
      1.0f,  1.0f,  1.0f,
      1.0f, -1.0f,  1.0f,
     -1.0f, -1.0f,  1.0f,

     // 顶面 (y正方向) - 需要顺时针旋转90度
     -1.0f,  1.0f, -1.0f,
     -1.0f,  1.0f,  1.0f,
      1.0f,  1.0f,  1.0f,
      1.0f,  1.0f,  1.0f,
      1.0f,  1.0f, -1.0f,
     -1.0f,  1.0f, -1.0f,

     // 底面 (y负方向) - 需要顺时针旋转90度
     -1.0f, -1.0f, -1.0f,
     -1.0f, -1.0f,  1.0f,
      1.0f, -1.0f,  1.0f,
      1.0f, -1.0f,  1.0f,
      1.0f, -1.0f, -1.0f,
     -1.0f, -1.0f, -1.0f
};

std::vector<std::string> faces{
    "assets/textures/bluecloud_ft.jpg",   // +X
    "assets/textures/bluecloud_bk.jpg",    // -X
    "assets/textures/bluecloud_up.jpg",     // +Y
    "assets/textures/bluecloud_dn.jpg",  // -Y
    "assets/textures/bluecloud_rt.jpg",   // +Z
    "assets/textures/bluecloud_lf.jpg"     // -Z
};// 立方体贴图的六个面对应的纹理文件名
class ResourceTester {
public:
    static void testAllResources() {
        std::cout << "\n========== 资源测试开始 ==========\n";

        // 测试宏定义
        std::cout << "宏定义测试:\n";
#ifdef ASSETS_DIR
        std::cout << "  ASSETS_DIR: " << ASSETS_DIR << "\n";
#else
        std::cout << "  ASSETS_DIR 未定义!\n";
#endif

#ifdef SHADER_DIR
        std::cout << "  SHADER_DIR: " << SHADER_DIR << "\n";
#endif

#ifdef TEXTURE_DIR
        std::cout << "  TEXTURE_DIR: " << TEXTURE_DIR << "\n";
#endif

        // 测试当前工作目录
        std::cout << "\n当前工作目录: "
            << std::filesystem::current_path() << "\n";

        // 测试各个资源目录
        testDirectory("Assets", ASSETS_DIR);
        testDirectory("Shaders", SHADER_DIR);
        testDirectory("Textures", TEXTURE_DIR);

        // 测试具体文件
        testFile("顶点着色器", std::string(SHADER_DIR) + "vertShader.glsl");
        testFile("片段着色器", std::string(SHADER_DIR) + "fragShader.glsl");
        testFile("纹理图片", std::string(TEXTURE_DIR) + "brick1.jpg");

        std::cout << "========== 资源测试结束 ==========\n\n";
    }

private:
    static void testDirectory(const std::string& name, const std::string& path) {
        std::cout << "\n测试 " << name << " 目录 [" << path << "]: ";

        if (std::filesystem::exists(path)) {
            std::cout << "存在\n";

            // 列出目录内容
            int count = 0;
            for (const auto& entry : std::filesystem::directory_iterator(path)) {
                std::cout << "    " << (count + 1) << ". "
                    << entry.path().filename().string() << "\n";
                count++;
            }

            if (count == 0) {
                std::cout << "    目录为空!\n";
            }
            else {
                std::cout << "    共 " << count << " 个文件\n";
            }
        }
        else {
            std::cout << "不存在!\n";

            // 尝试创建目录
            try {
                std::filesystem::create_directories(path);
                std::cout << "    已尝试创建目录\n";
            }
            catch (...) {
                std::cout << "    无法创建目录\n";
            }
        }
    }

    static void testFile(const std::string& name, const std::string& path) {
        std::cout << "测试 " << name << " [" << path << "]: ";

        std::ifstream file(path);
        if (file.is_open()) {
            std::cout << "可读\n";
            file.close();
        }
        else {
            std::cout << "无法打开\n";
        }
    }
};

void processInput(GLFWwindow* window)// 键盘输入处理函数
{
    // 获取帧时间
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // 处理键盘输入
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(CameraMovement::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(CameraMovement::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(CameraMovement::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(CameraMovement::RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.ProcessKeyboard(CameraMovement::UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(CameraMovement::DOWN, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)// 鼠标移动回调函数
{
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xOffset = xpos - lastX;
    float yOffset = lastY - ypos;  // 反向Y轴
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)// 鼠标滚轮回调函数
{
    camera.ProcessMouseScroll(yOffset);
}

// 键盘回调函数
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);// 按下ESC键关闭窗口
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// 创建一个简单的立方体贴图纹理，用于测试天空盒渲染
unsigned int loadCubemap(std::vector<std::string> faces) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++) {
        // 使用stb_image加载图片
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);

        if (data) {
            // 根据图片通道数判断格式
            GLenum format;
            if (nrChannels == 1)
                format = GL_RED;
            else if (nrChannels == 3)
                format = GL_RGB;
            else if (nrChannels == 4)
                format = GL_RGBA;

            // 上传到OpenGL立方体贴图的对应面
            // GL_TEXTURE_CUBE_MAP_POSITIVE_X + i 会依次对应6个面
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

            stbi_image_free(data); // 释放图片内存
        }
        else {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }

    // 设置纹理参数
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}