#include"chushi.h"

using namespace std;

#define numVAOs 1
#define numVBOs 2

Utils util = Utils();
float cameraX, cameraY, cameraZ;
float cubeLocX, cubeLocY, cubeLocZ;
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

// variable allocation for display
GLuint mvLoc, projLoc;
int width, height;
float aspect; GLuint aspectLoc;
glm::mat4 pMat, vMat, mMat, mvMat;

glm::vec2 wenlizuobiao_xy;

glm::ivec3 wuizhi;
glm::ivec3 wuizhi1;

void CleanupOpenGLResources() {
	// 1. 先解除绑定（虽然不是必须，但是好习惯）
	glUseProgram(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// 2. 删除着色器程序 (Shader Program)
	if (renderingProgram != 0) {
		glDeleteProgram(renderingProgram);
		renderingProgram = 0;
	}

	// 3. 删除顶点数组对象 (VAO) - 通常先于VBO/EBO删除
	if (vao != 0) {
		glDeleteVertexArrays(1, &vao[0]);
		vao[0] = 0;
	}

	// 4. 删除缓冲对象 (VBO, EBO)
	if (vbo[0] != 0) {
		glDeleteBuffers(1, &vbo[0]);
		vbo[0] = 0;
	}
	if (vbo[1] != 0) {
		glDeleteBuffers(1, &vbo[1]);
		vbo[1] = 0;
	}

	// 5. 删除纹理 (Texture)
	if (textureID != 0) {
		glDeleteTextures(1, &textureID);
		textureID = 0;
	}

	// ... 继续删除其他所有用glGen*创建的对象
}

void setupVertices(void) {

	Select_Material();
	generateInstances(1, 0, 0, shenchenqukuaidaxiao);// 生成实例数据的函数 

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(numVBOs, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, cubeVertices.size() * sizeof(Vertex), cubeVertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3)));
	glEnableVertexAttribArray(1);


	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(glm::vec3)));
	glEnableVertexAttribArray(2);


	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, maxInstances * sizeof(InstanceData) * 3, nullptr, GL_STATIC_DRAW);

	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)0);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(sizeof(glm::vec4)));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(2 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(3 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(7, 2, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(4 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(7);
	// 关键：设置属性为实例化模式
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);
	glVertexAttribDivisor(7, 1);


	glGenTextures(1, &textureID); // 创建一个纹理ID
	glBindTexture(GL_TEXTURE_2D, textureID); // 绑定纹理对象

	// 设置纹理环绕方式：在S和T轴上设置为重复
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// 设置纹理过滤方式：缩小和放大时都使用线性过滤
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//使像素更加清晰

	Texdata = stbi_load("assets/textures/texture.png", &width, &height, &nrChannels, 4);//4通道（RGBA）用于加载.png纹理

	if (Texdata) {
		// 上传纹理数据到GPU
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
			GL_UNSIGNED_BYTE, Texdata);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(Texdata);

	wuizhi.x = int(camera.Position.x) / 16;
	wuizhi.y = int(camera.Position.y) / 16;
	wuizhi.z = int(camera.Position.z) / 16;
	wuizhi.x = wuizhi1.x;
	wuizhi.y = wuizhi1.y;
	wuizhi.z = wuizhi1.z;
}

void init(GLFWwindow* window) {
	cout << 1 << std::endl;
	renderingProgram = Utils::createShaderProgram("assets/shaders/vertShader.glsl", "assets/shaders/fragShader.glsl");
	cout << 2;
	cameraX = 0.0f; cameraY = 0.0f; cameraZ = 8.0f;
	cubeLocX = 0.0f; cubeLocY = -2.0f; cubeLocZ = 0.0f;
	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
	texturezuobiao = glGetUniformLocation(renderingProgram, "texturezuobiao");
	setupVertices();

}

void display(GLFWwindow* window, double currentTime) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	wuizhi1.x = camera.Position.x > 0 ? int(camera.Position.x) / 16 : int(camera.Position.x) / 16 - 1;
	wuizhi1.y = camera.Position.y > 0 ? int(camera.Position.y) / 16 : int(camera.Position.y) / 16 - 1;
	wuizhi1.z = camera.Position.z > 0 ? int(camera.Position.z) / 16 : int(camera.Position.z) / 16 - 1;
	if(wuizhi.x != wuizhi1.x || wuizhi.y!=wuizhi1.y || wuizhi.z!=wuizhi1.z){
		wuizhi.x = wuizhi1.x;
		wuizhi.y = wuizhi1.y;
		wuizhi.z = wuizhi1.z;
		instances.clear();
		generateInstances(wuizhi.x, wuizhi.z, 0, shenchenqukuaidaxiao);
	}

	processInput(window);// 键盘输入处理

	glUseProgram(renderingProgram);

	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = camera.GetProjectionMatrix(aspect);// 投影矩阵 

	vMat = camera.GetViewMatrix();// 视图矩阵
	mMat = glm::translate(glm::mat4(1.0f), glm::vec3(cubeLocX, cubeLocY, cubeLocZ));// 模型矩阵
	mvMat = vMat * mMat;
	

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

	wenlizuobiao_xy = getTextureCoords(216);
	glUniform2f(texturezuobiao, wenlizuobiao_xy.x / chang, wenlizuobiao_xy.y / kuan);

	glActiveTexture(GL_TEXTURE0);// 激活纹理单元0
	glBindTexture(GL_TEXTURE_2D, textureID);// 绑定纹理对象到当前激活的纹理单元

	// 设置着色器中的纹理单元
	glUniform1i(glGetUniformLocation(renderingProgram, "ourTexture"), 0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	size_t writeOffset = (ringIndex % 3) * maxInstances * sizeof(InstanceData);

	glBufferSubData(GL_ARRAY_BUFFER,writeOffset, instances.size() * sizeof(InstanceData), instances.data());

	
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(writeOffset));
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(writeOffset + 1*sizeof(glm::vec4)));
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(writeOffset + 2*sizeof(glm::vec4)));
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(writeOffset + 3*sizeof(glm::vec4)));
	glVertexAttribPointer(7, 2, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(writeOffset + 4*sizeof(glm::vec4))); 
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glDrawArraysInstanced(GL_TRIANGLES, 0, 36, shilihuashulian);

	ringIndex++;
}

int main(void) {
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(1800, 1200, "Chapter 4 - program 1a", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);
	glEnable(GL_DEPTH_TEST);

	init(window);

	// 创建并初始化天空盒

	// 必须设置：将鼠标设置为禁用模式（隐藏并捕获）
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// 设置回调函数
	glfwSetCursorPosCallback(window, mouse_callback);// 鼠标移动回调函数
	glfwSetScrollCallback(window, scroll_callback);// 鼠标滚轮回调函数
	glfwSetKeyCallback(window, key_callback);// 键盘输入回调函数

	while (!glfwWindowShouldClose(window)) {
		
		display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	CleanupOpenGLResources();
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}