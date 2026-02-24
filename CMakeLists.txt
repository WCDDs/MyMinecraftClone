cmake_minimum_required(VERSION 3.20)
project(MyMinecraftClone)

# ===== 修改为你的lib路径 =====
set(LIB_ROOT "D:/xian_mu/MyMinecraftClone/lib")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 设置VS调试工作目录
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

# ===== 包含所有头文件目录 =====
include_directories(
    ${LIB_ROOT}/include
    ${LIB_ROOT}/include/glm
    ${LIB_ROOT}/lib/SOIL2          # 重要：添加SOIL2源码目录到头文件搜索路径
)

# ===== 链接库目录 =====
link_directories(
    ${LIB_ROOT}/lib
)

# ===== 收集所有源文件 =====
# 首先收集你自己写的源文件
file(GLOB_RECURSE SRC_LIST 
    ${CMAKE_SOURCE_DIR}/src/*.cpp
    ${CMAKE_SOURCE_DIR}/src/*.c
)

# 然后添加SOIL2的所有源文件
file(GLOB SOIL2_SOURCES 
    ${LIB_ROOT}/lib/SOIL2/*.c      # 添加所有.c文件
    ${LIB_ROOT}/lib/SOIL2/*.cpp    # 如果有.cpp文件
)

# 合并源文件列表
list(APPEND SRC_LIST ${SOIL2_SOURCES})

add_executable(${PROJECT_NAME} ${SRC_LIST})

# ===== 链接其他库 =====
target_link_libraries(${PROJECT_NAME}
    glew32          # 如果是静态链接用glew32s，动态用glew32
    glfw3
    opengl32
)

# ===== 复制DLL文件（如果有） =====
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${LIB_ROOT}/bin/glew32.dll
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/
    COMMENT "Copying DLLs to output directory"
)

message(STATUS "SOIL2 sources added: ${SOIL2_SOURCES}")