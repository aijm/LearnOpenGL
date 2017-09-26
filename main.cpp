#include <iostream>
#include "shader.h"
#include "Camera.h"
#include <SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
// GLEW
//#define GLEW_STATIC
#include <GL/glew.h>
//加一句话试试
// GLFW
#include <GLFW/glfw3.h>
// Light attributes
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
GLfloat deltaTime = 0.0f;   // 当前帧遇上一帧的时间差
GLfloat lastFrame = 0.0f;   // 上一帧的时间
GLfloat lastxpos;
GLfloat lastypos;
bool firstMouse = true;
bool jumped = false;
GLfloat fov = 45.0f;
bool keys[1024];
float mixvalue = 0.2f;
Camera camera;
float r = 5.0f;
float xrotate = 0;
float yrotate = 90.0f;
float zrotate = 0;
// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void do_movement();
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// The MAIN function, from here we start the application and run the game loop
int main()
{

	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	// Define the viewport dimensions
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);
	//shader
	Shader ourShader = Shader("shader.vs", "shader.frag");
	Shader lightingShader = Shader("lighting.vs", "lighting.frag");
	Shader lampShader = Shader("lamp.vs", "lamp.frag");

	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices1[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	GLfloat vertices[] = {
		-0.5f, -0.5f, -0.5f, 
		0.5f, -0.5f, -0.5f, 
		0.5f,  0.5f, -0.5f,  
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f, 
		0.5f,  0.5f,  0.5f, 
		0.5f,  0.5f,  0.5f, 
		-0.5f,  0.5f,  0.5f, 
		-0.5f, -0.5f,  0.5f, 

		-0.5f,  0.5f,  0.5f, 
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f, 
		-0.5f, -0.5f, -0.5f, 
		-0.5f, -0.5f,  0.5f, 
		-0.5f,  0.5f,  0.5f,

		0.5f,  0.5f,  0.5f, 
		0.5f,  0.5f, -0.5f, 
		0.5f, -0.5f, -0.5f,  
		0.5f, -0.5f, -0.5f,  
		0.5f, -0.5f,  0.5f,  
		0.5f,  0.5f,  0.5f, 

		-0.5f, -0.5f, -0.5f, 
		0.5f, -0.5f, -0.5f,  
		0.5f, -0.5f,  0.5f, 
		0.5f, -0.5f,  0.5f,  
		-0.5f, -0.5f,  0.5f,  
		-0.5f, -0.5f, -0.5f,  

		-0.5f,  0.5f, -0.5f, 
		0.5f,  0.5f, -0.5f, 
		0.5f,  0.5f,  0.5f, 
		0.5f,  0.5f,  0.5f,  
		-0.5f,  0.5f,  0.5f, 
		-0.5f,  0.5f, -0.5f
	};
	//设置索引
	GLint indices[] = {
		0,1,3,
		1,2,3
	};
	GLuint VBO[2], VAO[2],EBO[1];
	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);
	glGenBuffers(1, EBO);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));
	//glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//灯
	glBindVertexArray(VAO[1]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));
	//glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	
	
	//纹理
	GLuint texture[2];
	glGenTextures(2, texture);
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	// Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//使用SOIL加载图像
	unsigned char* image = SOIL_load_image("container.jpg", &width, &height, 0, SOIL_LOAD_RGB);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	//释放图像，并解绑
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindTexture(GL_TEXTURE_2D, texture[1]);

	// Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	image = SOIL_load_image("awesomeface.png", &width, &height, 0, SOIL_LOAD_RGB);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	//释放图像，并解绑
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	//glm::mat4 trans = glm::rotate(glm::mat4(1.0f), (float)glm::radians(45.0), glm::vec3(0.0f, 0.0f, 1.0f));
	//trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	
	camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
	
						  // Game loop
	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		//cout << "deltatime" << deltaTime << endl;
		lastFrame = currentFrame;
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		do_movement();

		// Render
		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		 //Draw our first triangle
		//ourShader.Use(); //使用着色器程序
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture[0]);
		//glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture1"), 0);

		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, texture[1]);
		//glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture2"), 1);

		//glUniform1f(glGetUniformLocation(ourShader.Program, "mixvalue"), mixvalue);

		lightingShader.Use();
		glBindVertexArray(VAO[0]);
		GLint objectColorLoc = glGetUniformLocation(lightingShader.Program, "objectColor");
		GLint lightColorLoc = glGetUniformLocation(lightingShader.Program, "lightColor");
		glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
		glUniform3f(lightColorLoc, 1.0f, 0.5f, 1.0f);
		for (GLuint i = 0; i < 10; i++)
		{
			model = glm::mat4(1.0f);
			view = glm::mat4(1.0f);
			projection = glm::mat4(1.0f);

			model = glm::translate(model, cubePositions[i]);
			//model = glm::rotate(model, glm::radians(20.0f)*(GLfloat)glfwGetTime()*(i+1), glm::vec3(0.5f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(20.0f)*(i + 1), glm::vec3(0.5f, 1.0f, 0.0f));
			/*model = glm::rotate(model, glm::radians(zrotate), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::rotate(model, glm::radians(yrotate), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(xrotate), glm::vec3(1.0f, 0.0f, 0.0f));*/
			/*glm::quat quatx(cos(glm::radians(xrotate)/2), glm::vec3(sin(glm::radians(xrotate) / 2), 0.0f, 0.0f));
			glm::quat quaty(cos(glm::radians(yrotate) / 2), glm::vec3(0.0f, sin(glm::radians(yrotate) / 2), 0.0f));
			glm::quat quatz(cos(glm::radians(zrotate) / 2), glm::vec3(0.0f, 0.0f,sin(glm::radians(zrotate) / 2)));
			glm::quat myquat = quatz*quaty*quatx;
			model = glm::toMat4(myquat);*/
			//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -7.0f));
			view = camera.GetViewMatrix();
			projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)(WIDTH / HEIGHT), 0.1f, 100.0f);

			/*GLuint modelLoc = glGetUniformLocation(ourShader.Program, "model");
			GLuint viewLoc = glGetUniformLocation(ourShader.Program, "view");
			GLuint projectionLoc = glGetUniformLocation(ourShader.Program, "projection");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));*/
			GLuint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
			GLuint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
			GLuint projectionLoc = glGetUniformLocation(lightingShader.Program, "projection");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
			

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		lampShader.Use();
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.2f));
		model = glm::translate(model, lightPos);
		/*view = glm::mat4(1.0f);
		projection = glm::mat4(1.0f);*/
		view = glm::lookAt(glm::vec3(1.2f, 1.0f, 4.0f), lightPos, glm::vec3(0.0f,1.0f,0.0f));
		projection = glm::perspective(glm::radians(45.0f), (GLfloat)(WIDTH / HEIGHT), 0.1f, 100.0f);
		

		GLuint modelLoc = glGetUniformLocation(lampShader.Program, "model");
		GLuint viewLoc = glGetUniformLocation(lampShader.Program, "view");
		GLuint projectionLoc = glGetUniformLocation(lampShader.Program, "projection");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(2, VAO);
	glDeleteBuffers(2, VBO);
	glDeleteBuffers(1, EBO);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key==GLFW_KEY_ESCAPE&&action==GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if(action==GLFW_PRESS)
	{
		keys[key] = true;
	}
	else if(action==GLFW_RELEASE)
	{
		keys[key] = false;
	}
	else{}


}

void do_movement()
{
	GLfloat sensitivity = 5.0f;
	
	GLfloat jumpstep = 50.0*deltaTime;
	if (keys[GLFW_KEY_UP])
	{
		mixvalue += 0.1f;
		if (mixvalue > 1) mixvalue = 1;

	}
	if (keys[GLFW_KEY_DOWN])
	{
		mixvalue -= 0.1f;
		if (mixvalue < 0) mixvalue = 0;
	}
	if (keys[GLFW_KEY_W])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (keys[GLFW_KEY_S])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (keys[GLFW_KEY_A])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (keys[GLFW_KEY_D])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
	if(keys[GLFW_KEY_SPACE])
	{
		
	}
	if(!keys[GLFW_KEY_SPACE]&&jumped)
	{
		
	}
	if(keys[GLFW_KEY_X])
	{
		xrotate += sensitivity;
		
	}
	if (keys[GLFW_KEY_Y])
	{
		yrotate += sensitivity;
		

	}
	if (keys[GLFW_KEY_Z])
	{
		zrotate += sensitivity;
		

	}
	
	
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if(firstMouse)
	{
		lastxpos = xpos;
		lastypos = ypos;
		firstMouse = false;
	
	}
	GLfloat xoffset = xpos - lastxpos;
	GLfloat yoffset = ypos - lastypos;
	lastxpos = xpos;
	lastypos = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
	
	

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);

}