cmake_minimum_required(VERSION 3.20)
project(MyMinecraftClone)

# ===== 修改为你的lib路径 =====
set(LIB_ROOT "D:/xian_mu/MyMinecraftClone/lib")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 设置VS调试工作目录
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# ===== 包含所有头文件目录 =====
include_directories(
    ${LIB_ROOT}/include
    ${LIB_ROOT}/include/glm
    ${LIB_ROOT}/lib/SOIL2
)

# ===== 列出所有SOIL2源文件 =====
set(SOIL2_SOURCES
    ${LIB_ROOT}/lib/SOIL2/SOIL2.c
    ${LIB_ROOT}/lib/SOIL2/image_DXT.c
    ${LIB_ROOT}/lib/SOIL2/image_helper.c
    ${LIB_ROOT}/lib/SOIL2/wfETC.c
)

# ===== 手动列出所有源文件 =====
add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/render/Camera.cpp
    src/world/Block.cpp
    src/core/Chunk.cpp
    
    src/render/Utils.cpp
    ${SOIL2_SOURCES}
  "src/core/PerlinNoise3D.h" "src/core/PerlinNoise3D.cpp")

# ===== 只对SOIL2.c定义STB_IMAGE_IMPLEMENTATION =====
set_source_files_properties(${LIB_ROOT}/lib/SOIL2/SOIL2.c PROPERTIES
    COMPILE_DEFINITIONS "STB_IMAGE_IMPLEMENTATION"
)

# ===== 编译定义 =====
target_compile_definitions(${PROJECT_NAME} PRIVATE
    ASSETS_DIR="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/"
    SHADER_DIR="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/shaders/"
    TEXTURE_DIR="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/textures/"
)

# ===== 链接所有需要的库 =====
target_link_libraries(${PROJECT_NAME}
    ${LIB_ROOT}/lib/glew32.lib
    ${LIB_ROOT}/lib/glfw3.lib
    opengl32.lib
)

# ===== 复制DLL文件（只复制glew32.dll，因为glfw3.dll可能不存在） =====
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${LIB_ROOT}/bin/glew32.dll
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/
    COMMENT "Copying glew32.dll to output directory"
)

# ===== 资源文件复制 =====
set(ASSETS_SOURCE_DIR "${CMAKE_SOURCE_DIR}/assets")
set(ASSETS_OUTPUT_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets")

if(EXISTS ${ASSETS_SOURCE_DIR}/shaders)
    file(COPY ${ASSETS_SOURCE_DIR}/shaders/ DESTINATION ${ASSETS_OUTPUT_DIR}/shaders/)
endif()

if(EXISTS ${ASSETS_SOURCE_DIR}/textures)
    file(COPY ${ASSETS_SOURCE_DIR}/textures/ DESTINATION ${ASSETS_OUTPUT_DIR}/textures/)
endif()

message(STATUS "========================================")
message(STATUS "✅ 项目配置完成")
message(STATUS "========================================")