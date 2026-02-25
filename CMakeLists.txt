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

add_executable(${PROJECT_NAME} ${SRC_LIST} "src/chushi.h" )

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

# 定义资源目录
set(ASSETS_SOURCE_DIR "${CMAKE_SOURCE_DIR}/assets")
set(ASSETS_OUTPUT_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets")

# 详细的路径检查
message(STATUS "========================================")
message(STATUS "资源路径检查:")
message(STATUS "  CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
message(STATUS "  ASSETS_SOURCE_DIR: ${ASSETS_SOURCE_DIR}")
message(STATUS "  ASSETS_OUTPUT_DIR: ${ASSETS_OUTPUT_DIR}")
message(STATUS "========================================")

# 检查assets目录是否存在
if(NOT EXISTS ${ASSETS_SOURCE_DIR})
    message(FATAL_ERROR 
        "错误：找不到assets目录！\n"
        "期望路径: ${ASSETS_SOURCE_DIR}\n"
        "请在项目根目录创建assets文件夹"
    )
endif()

# ===== 1. 复制着色器文件 =====
set(SHADER_SOURCE_DIR "${ASSETS_SOURCE_DIR}/shaders")
set(SHADER_OUTPUT_DIR "${ASSETS_OUTPUT_DIR}/shaders")

if(EXISTS ${SHADER_SOURCE_DIR})
    # 收集所有着色器文件
    file(GLOB_RECURSE SHADER_FILES
        ${SHADER_SOURCE_DIR}/*.glsl
        ${SHADER_SOURCE_DIR}/*.vert
        ${SHADER_SOURCE_DIR}/*.frag
        ${SHADER_SOURCE_DIR}/*.geom
        ${SHADER_SOURCE_DIR}/*.vs
        ${SHADER_SOURCE_DIR}/*.fs
    )
    
    list(LENGTH SHADER_FILES SHADER_COUNT)
    message(STATUS "找到 ${SHADER_COUNT} 个着色器文件")
    
    if(SHADER_FILES)
        # 创建输出目录并复制文件
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADER_OUTPUT_DIR}
            COMMAND ${CMAKE_COMMAND} -E copy ${SHADER_FILES} ${SHADER_OUTPUT_DIR}/
            COMMENT "复制着色器文件到: ${SHADER_OUTPUT_DIR}"
        )
        
        # 配置时也复制
        file(COPY ${SHADER_FILES} DESTINATION ${SHADER_OUTPUT_DIR})
        
        message(STATUS "✅ 着色器文件已准备复制到: ${SHADER_OUTPUT_DIR}")
    endif()
else()
    message(WARNING "⚠️ 着色器目录不存在: ${SHADER_SOURCE_DIR}")
endif()

# ===== 2. 复制纹理文件 =====
set(TEXTURE_SOURCE_DIR "${ASSETS_SOURCE_DIR}/textures")
set(TEXTURE_OUTPUT_DIR "${ASSETS_OUTPUT_DIR}/textures")

if(EXISTS ${TEXTURE_SOURCE_DIR})
    # 收集所有纹理文件（支持常见图片格式）
    file(GLOB_RECURSE TEXTURE_FILES
        ${TEXTURE_SOURCE_DIR}/*.png
        ${TEXTURE_SOURCE_DIR}/*.jpg
        ${TEXTURE_SOURCE_DIR}/*.jpeg
        ${TEXTURE_SOURCE_DIR}/*.bmp
        ${TEXTURE_SOURCE_DIR}/*.tga
        ${TEXTURE_SOURCE_DIR}/*.gif
        ${TEXTURE_SOURCE_DIR}/*.psd
        ${TEXTURE_SOURCE_DIR}/*.hdr
        ${TEXTURE_SOURCE_DIR}/*.pic
        ${TEXTURE_SOURCE_DIR}/*.pnm
    )
    
    list(LENGTH TEXTURE_FILES TEXTURE_COUNT)
    message(STATUS "找到 ${TEXTURE_COUNT} 个纹理文件")
    
    if(TEXTURE_FILES)
        # 显示找到的纹理文件（调试用）
        foreach(TEXTURE ${TEXTURE_FILES})
            message(STATUS "  - 纹理: ${TEXTURE}")
        endforeach()
        
        # 创建输出目录并复制文件
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory ${TEXTURE_OUTPUT_DIR}
            COMMAND ${CMAKE_COMMAND} -E copy ${TEXTURE_FILES} ${TEXTURE_OUTPUT_DIR}/
            COMMENT "复制纹理文件到: ${TEXTURE_OUTPUT_DIR}"
        )
        
        # 配置时也复制
        file(COPY ${TEXTURE_FILES} DESTINATION ${TEXTURE_OUTPUT_DIR})
        
        message(STATUS "✅ 纹理文件已准备复制到: ${TEXTURE_OUTPUT_DIR}")
    endif()
else()
    message(WARNING "⚠️ 纹理目录不存在: ${TEXTURE_SOURCE_DIR}")
endif()

# ===== 3. 如果需要，也可以复制其他资源 =====
# 复制models文件夹
set(MODEL_SOURCE_DIR "${ASSETS_SOURCE_DIR}/models")
set(MODEL_OUTPUT_DIR "${ASSETS_OUTPUT_DIR}/models")
if(EXISTS ${MODEL_SOURCE_DIR})
    file(GLOB_RECURSE MODEL_FILES ${MODEL_SOURCE_DIR}/*)
    if(MODEL_FILES)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory ${MODEL_OUTPUT_DIR}
            COMMAND ${CMAKE_COMMAND} -E copy ${MODEL_FILES} ${MODEL_OUTPUT_DIR}/
            COMMENT "复制模型文件到: ${MODEL_OUTPUT_DIR}"
        )
        file(COPY ${MODEL_FILES} DESTINATION ${MODEL_OUTPUT_DIR})
        message(STATUS "✅ 模型文件已准备复制")
    endif()
endif()

# 复制sounds文件夹
set(SOUND_SOURCE_DIR "${ASSETS_SOURCE_DIR}/sounds")
set(SOUND_OUTPUT_DIR "${ASSETS_OUTPUT_DIR}/sounds")
if(EXISTS ${SOUND_SOURCE_DIR})
    file(GLOB_RECURSE SOUND_FILES ${SOUND_SOURCE_DIR}/*)
    if(SOUND_FILES)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory ${SOUND_OUTPUT_DIR}
            COMMAND ${CMAKE_COMMAND} -E copy ${SOUND_FILES} ${SOUND_OUTPUT_DIR}/
            COMMENT "复制音效文件到: ${SOUND_OUTPUT_DIR}"
        )
        file(COPY ${SOUND_FILES} DESTINATION ${SOUND_OUTPUT_DIR})
        message(STATUS "✅ 音效文件已准备复制")
    endif()
endif()

# 定义宏，方便在代码中定位资源
target_compile_definitions(${PROJECT_NAME} PRIVATE
    ASSETS_DIR="${ASSETS_OUTPUT_DIR}/"
    SHADER_DIR="${ASSETS_OUTPUT_DIR}/shaders/"
    TEXTURE_DIR="${ASSETS_OUTPUT_DIR}/textures/"
)

# 最终调试信息
message(STATUS "========================================")
message(STATUS "✅ 项目配置完成：")
message(STATUS "  项目根目录: ${CMAKE_SOURCE_DIR}")
message(STATUS "  资源源目录: ${ASSETS_SOURCE_DIR}")
message(STATUS "  资源输出目录: ${ASSETS_OUTPUT_DIR}")
message(STATUS "  调试工作目录: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "========================================")