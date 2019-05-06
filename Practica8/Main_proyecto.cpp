//HERNANDEZ LOPEZ AXEL MAURCIO
//GRUPO 1
//PROYECTO

#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();
void animacion();
void animacion2();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(-5.0f, 5.0f, 8.0f),glm::vec3(0.0f,1.0f,0.0f),-45.0f);
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
float range = 0.0f;
float spotAngle = 0.0f;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};

glm::vec3 LightP1;
glm::vec3 LightP2;
glm::vec3 LightP3;
glm::vec3 LightP4;

float movCangreZ = -8.0f;
float movCangreX = 6.5f;
bool recorrido1 = true;
bool recorrido2 = false;
bool recorrido3 = false;
bool recorrido4 = false;
bool dibuja1 = false;
bool dibuja2 = false;

float angGrub = 0.0f;
float movGrubX = 4.0f;
bool vuelta1 = true;
bool vuelta2 = false;

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

//char* arreglo_modelos[4] = { (char*)"Models/Fishes/TropicalFish02.obj",
//							(char*)"Models/Fishes/TropicalFish03.obj",
//							(char*)"Models/Fishes/TropicalFish15.obj",
//							(char*)"Models/Fishes/TropicalFish05.obj" };
//int indice_modelos = 0;

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");
	Shader SkyBoxshader("Shaders/SkyBox.vs", "Shaders/SkyBox.frag");

	//Model Mymodel(arreglo_modelos[indice_modelos]);

	Model mesa((char*)"models/misModelos/mesa2.obj");
	Model bote((char*)"models/misModelos/OldBoat.obj");
	Model barril((char*)"models/misModelos/barrel_obj.obj");
	Model cangre((char*)"models/misModelos/hamburgeres.obj");

	// Build and compile our shader program


	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] =
	{
		// Positions            // Normals              // Texture Coords
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,  	1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f
	};

	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	GLuint indices[] =
	{  // Note that we start from 0!
		0,1,2,3,
		4,5,6,7,
		8,9,10,11,
		12,13,14,15,
		16,17,18,19,
		20,21,22,23,
		24,25,26,27,
		28,29,30,31,
		32,33,34,35
	};

	// Positions all containers
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


	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Texture Coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	// Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Set the vertex attributes (only position data for the lamp))
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0); // Note that we skip over the other data in our buffer object (we don't need the normals/textures, only positions).
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);


	//SkyBox
	GLuint skyboxVBO, skyboxVAO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);

	// Load textures
	vector<const GLchar*> faces;
	faces.push_back("SkyBox/right.tga");
	faces.push_back("SkyBox/left.tga");
	faces.push_back("SkyBox/top.tga");
	faces.push_back("SkyBox/bottom.tga");
	faces.push_back("SkyBox/back.tga");
	faces.push_back("SkyBox/front.tga");

	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);



	// Load textures
	GLuint texture1, texture2, texture3, texture4, texture5, texture6, texture7,
			texture8,texture9,texture10,texture11,
			kilo, uniform, zurich, india, romeo, caja, anuncio, crustaceo;
	glGenTextures(1, &texture1);
	glGenTextures(1, &texture2);
	glGenTextures(1, &texture3);
	glGenTextures(1, &texture4);
	glGenTextures(1, &texture5);
	glGenTextures(1, &texture6);
	glGenTextures(1, &texture7);
	glGenTextures(1, &texture8);
	glGenTextures(1, &texture9);
	glGenTextures(1, &texture10);
	glGenTextures(1, &texture11);
	glGenTextures(1, &kilo);
	glGenTextures(1, &uniform);
	glGenTextures(1, &zurich);
	glGenTextures(1, &india);
	glGenTextures(1, &romeo);
	glGenTextures(1, &caja);
	glGenTextures(1, &anuncio);
	glGenTextures(1, &crustaceo);

	int textureWidth, textureHeight, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *image;

	//Madera
	texture1 = TextureLoading::LoadTexture((GLchar *)"images/madera.jpg");

	// Specular map
	texture2 = TextureLoading::LoadTexture((GLchar *)"images/container2_specular.png");

	//Textura piso
	texture3 = TextureLoading::LoadTexture((GLchar *)"images/piso6.jpg");

	//Textura ventanas
	texture4 = TextureLoading::LoadTexture((GLchar *)"images/glass2.jpg");

	//Textura metal
	texture5 = TextureLoading::LoadTexture((GLchar *)"images/metal6.jpg");

	//Textura cuerda
	texture6 = TextureLoading::LoadTexture((GLchar *)"images/cuerda.png");

	//Textura metal puertas
	texture7 = TextureLoading::LoadTexture((GLchar *)"images/metal5.jpg");

	//Textura galley grub
	texture8 = TextureLoading::LoadTexture((GLchar *)"images/galley.png");

	//Textura order here
	texture9 = TextureLoading::LoadTexture((GLchar *)"images/order.jpg");

	//Textura fondo puertas
	texture10 = TextureLoading::LoadTexture((GLchar *)"images/metal1.jpg");

	//Textura fondo cocina
	texture11 = TextureLoading::LoadTexture((GLchar *)"images/cocina.jpg");

	//Textura caja
	caja = TextureLoading::LoadTexture((GLchar *)"images/container2.png");
	
	//Texturas banderas
	romeo = TextureLoading::LoadTexture((GLchar *)"images/romeo.png");
	india = TextureLoading::LoadTexture((GLchar *)"images/india.jpg");
	zurich = TextureLoading::LoadTexture((GLchar *)"images/zurich.jpg");
	uniform = TextureLoading::LoadTexture((GLchar *)"images/uniform.png");
	kilo = TextureLoading::LoadTexture((GLchar *)"images/kilo.png");

	//Anuncio
	anuncio = TextureLoading::LoadTexture((GLchar *)"images/krabby.png");

	//Crustaceo
	crustaceo = TextureLoading::LoadTexture((GLchar *)"images/krusty.jpg");


	glBindTexture(GL_TEXTURE_2D, 0);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);
	
	float angulo = 45.0f;

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		animacion();
		animacion2();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	    //Load Model
	

		//// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 50.0f);
		// == ==========================
		// Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
		// the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
		// by defining light types as classes and set their values in there, or by using a more efficient uniform approach
		// by using 'Uniform buffer objects', but that is something we discuss in the 'Advanced GLSL' tutorial.
		// == ==========================
		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), 1.0f, 1.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 1.0f,1.0f,1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);


		// Point light 1
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), LightP1.x, LightP1.y, LightP1.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), LightP1.x, LightP1.y, LightP1.z);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.032f);



		// Point light 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), LightP2.x, LightP2.y, LightP2.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), LightP2.x, LightP2.y, LightP2.z);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.032f);

		// Point light 3
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), LightP3.x, LightP3.y, LightP3.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), LightP3.x, LightP3.y, LightP3.z);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.032f);

		// Point light 4
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), LightP4.x, LightP4.y, LightP4.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), LightP4.x, LightP4.y, LightP4.z);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.032f);

		//SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.032f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

		////Set material properties
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 1.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");
		//GLint uniformColor = lightingShader.uniformColor;

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));



		// Bind diffuse map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		//// Bind specular map
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture2);


		glBindVertexArray(VAO);
		glm::mat4 model(1);

		//Dibujo de los cubos
		/*for (GLuint i = 0; i < 10; i++)
		{
			model = glm::mat4(1);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			if (i == 2)
				model = glm::scale(model, glm::vec3(1.0f,1.0f,2.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}*/
		
		//Frente
		//Base frente izquierda
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(2.5f, 0.5f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.0f, 5.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Base frente derecha
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(10.5f, 0.5f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.0f, 5.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Columna frente izquierda
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(1.0f, 3.5f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 5.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Columna frente derecha
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(12.0f, 3.5f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 5.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Marco puerta
		//Marco izquierda
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(4.75f, 2.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 3.5f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Marco izquierda
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(8.25f, 2.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 3.5f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Marco arriba
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(6.5f, 4.75f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.5f, 4.0f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Curvas
		//Curvas izquierda
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(1.0f, 6.25f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.5f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(1.0f, 6.5f, 0.0f));
		model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.5f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(1.0f, 6.75f, -0.12f));
		model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.5f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(1.0f, 6.25f, -10.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.5f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(1.0f, 6.5f, -10.0f));
		model = glm::rotate(model, glm::radians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.5f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(1.0f, 6.75f, -9.88f));
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.5f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		for (int i = 0; i < 21; i++)
		{
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(1.0f, (6.5f)*glm::sin(glm::radians(angulo))+2.5f, (6.5f)*glm::cos(glm::radians(angulo))-5.0f));
			model = glm::rotate(model, glm::radians((-1)*angulo+90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);

			angulo += 4.5f;
		}

		angulo = 45.0f;

		//Curvas derecha
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(12.0f, 6.25f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.5f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(12.0f, 6.5f, 0.0f));
		model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.5f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(12.0f, 6.75f, -0.12f));
		model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.5f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(12.0f, 6.25f, -10.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.5f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(12.0f, 6.5f, -10.0f));
		model = glm::rotate(model, glm::radians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.5f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(12.0f, 6.75f, -9.88f));
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.5f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		for (int i = 0; i < 21; i++)
		{
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(12.0f, (6.5f)*glm::sin(glm::radians(angulo)) + 2.5f, (6.5f)*glm::cos(glm::radians(angulo)) - 5.0f));
			model = glm::rotate(model, glm::radians((-1)*angulo + 90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);

			angulo += 4.5f;
		}

		angulo = 45.0f;

		//Curva medio izquierda
		for (int i = 0; i < 21; i++)
		{
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(4.0f, (6.5f)*glm::sin(glm::radians(angulo)) + 2.5f, (6.5f)*glm::cos(glm::radians(angulo)) - 5.0f));
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::rotate(model, glm::radians((-1)*angulo + 90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.5f, 5.0f, 0.5f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);

			angulo += 4.5f;
		}

		angulo = 45.0f;

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(4.0f, 5.25f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.5f, 5.0f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(4.0f, 5.75f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(-7.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 5.0f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(4.0f, 6.25f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 5.0f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(4.0f, 6.75f, -0.12f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 5.0f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

			//Atrás
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(4.0f, 5.25f, -10.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.5f, 5.0f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(4.0f, 5.75f, -10.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(7.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 5.0f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(4.0f, 6.25f, -10.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(15.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 5.0f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(4.0f, 6.75f, -9.88f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 5.0f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Curva medio derecha
		for (int i = 0; i < 21; i++)
		{
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(9.0f, (6.5f)*glm::sin(glm::radians(angulo)) + 2.5f, (6.5f)*glm::cos(glm::radians(angulo)) - 5.0f));
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::rotate(model, glm::radians((-1)*angulo + 90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.5f, 5.0f, 0.5f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);

			angulo += 4.5f;
		}

		angulo = 45.0f;

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(9.0f, 5.25f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.5f, 5.0f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(9.0f, 5.75f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(-7.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 5.0f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(9.0f, 6.25f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 5.0f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(9.0f, 6.75f, -0.12f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 5.0f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Atrás
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(9.0f, 5.25f, -10.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.5f, 5.0f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(9.0f, 5.75f, -10.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(7.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 5.0f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(9.0f, 6.25f, -10.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(15.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 5.0f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(9.0f, 6.75f, -9.88f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 5.0f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		
		//Tras
		//Base tras
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(6.5f, 0.5f, -10.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.0f, 13.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Columna tras izquierda
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(1.0f, 3.5f, -10.0f));
		model = glm::scale(model, glm::vec3(1.0f, 5.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Columna frente derecha
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(12.0f, 3.5f, -10.0f));
		model = glm::scale(model, glm::vec3(1.0f, 5.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Laterales
		//Base lateral derecha
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(12.0f, 0.5f, -5.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 9.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Base lateral izquierda
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(1.0f, 0.5f, -5.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 9.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera lateral izquierda
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(1.0f, 6.0f, -5.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 9.0f, 2.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera2 lateral izquierda
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(1.0f, 7.5f, -5.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 8.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera3 lateral izquierda
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(1.0f, 8.25f, -5.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 7.0f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera lateral derecha
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(12.0f, 6.0f, -5.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 9.0f, 2.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera2 lateral derecha
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(12.0f, 7.5f, -5.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 8.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera3 lateral derecha
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(12.0f, 8.25f, -5.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 7.0f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Piso
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture3);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(6.5f, 0.0f, -5.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(12.0f, 9.0f, 0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Ventanas
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);

		//Ventana frente izquierda
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(3.0f, 3.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 4.0f, 0.01f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Ventana frente derecha
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(10.0f, 3.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 4.0f, 0.01f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Puerta
		//Puerta izquierda
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(5.0f, 2.25f, -0.5f));
		model = glm::rotate(model, glm::radians(angGrub-125.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 4.5f, 0.01f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Puerta derecha
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(8.0f, 2.25f, -0.5f));
		model = glm::rotate(model, glm::radians(-angGrub+125.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 4.5f, 0.01f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Ventana lateral izquierda
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(1.0f, 3.0f, -5.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(9.0f, 4.0f, 0.01f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Ventana lateral derecha
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(12.0f, 3.0f, -5.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(9.0f, 4.0f, 0.01f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Parte trasera Metal
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture5);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(6.5f, 3.0f, -10.0f));
		model = glm::scale(model, glm::vec3(10.0f, 4.0f, 0.01f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Pared Metal
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(6.5f, 2.5f, -9.0f));
		model = glm::scale(model, glm::vec3(10.0f, 5.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Chimenea
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(4.0f, 9.75f, -3.5f));
		model = glm::scale(model, glm::vec3(0.7f, 2.0f, 0.7f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Puertas
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture7);

		//Puerta1
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(2.0f, 1.5f, -8.37f));
		model = glm::scale(model, glm::vec3(0.25f, 3.0f, 0.25f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(4.0f, 1.5f, -8.37f));
		model = glm::scale(model, glm::vec3(0.25f, 3.0f, 0.25f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(3.0f, 3.12f, -8.37f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.25f, 2.25f, 0.25f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Puerta2
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(9.0f, 1.5f, -8.37f));
		model = glm::scale(model, glm::vec3(0.25f, 3.0f, 0.25f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(11.0f, 1.5f, -8.37f));
		model = glm::scale(model, glm::vec3(0.25f, 3.0f, 0.25f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(10.0f, 3.12f, -8.37f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.25f, 2.25f, 0.25f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cocina
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(5.5f, 2.5f, -8.37f));
		model = glm::scale(model, glm::vec3(0.25f, 1.5f, 0.25f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(7.5f, 2.5f, -8.37f));
		model = glm::scale(model, glm::vec3(0.25f, 1.5f, 0.25f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(6.5f, 3.37f, -8.37f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.25f, 2.25f, 0.25f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(6.5f, 1.63f, -8.37f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.25f, 2.25f, 0.25f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Charola
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(movCangreX, 1.98f, movCangreZ));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Fondo cocina
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture11);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(6.5f, 2.5f, -8.37f));
		model = glm::scale(model, glm::vec3(1.75f, 1.5f, 0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Fondo puertas
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture10);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(10.0f, 1.5f, -8.37f));
		model = glm::scale(model, glm::vec3(1.75f, 3.0f, 0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(3.0f, 1.5f, -8.37f));
		model = glm::scale(model, glm::vec3(1.75f, 3.0f, 0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Order here
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture9);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(5.5f, 2.5f, -7.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.5f, 0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//galley
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture8);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(movGrubX, 2.5f, -7.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.5f, 0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cajas
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, caja);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(4.0f, 5.5f, -9.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(5.0f, 5.5f, -9.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(4.0f, 6.5f, -9.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(8.0f, 5.5f, -9.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(9.0f, 5.5f, -9.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(10.0f, 5.5f, -9.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(10.0f, 6.5f, -9.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		
		//Cuerdas
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture6);

		//Cuerda galley
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(3.75f, 6.0f, -7.0f));
		model = glm::rotate(model, glm::radians(angGrub), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.05f, 5.75f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(4.25f, 6.0f, -7.0f));
		model = glm::rotate(model, glm::radians(angGrub), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.05f, 5.75f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cuerda order
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(5.25f, 5.75f, -7.0f));
		model = glm::scale(model, glm::vec3(0.05f, 6.25f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(5.75f, 5.75f, -7.0f));
		model = glm::scale(model, glm::vec3(0.05f, 6.25f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		//Cuerda1
		//Cuerda1 fuera derecha
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(12.1f, 1.5f, -0.4f));
		model = glm::rotate(model, glm::radians(-54.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 10.5f, 0.1f));
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cuerda1 dentro derecha
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(11.9f, 1.5f, -0.4f));
		model = glm::rotate(model, glm::radians(-54.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 10.5f, 0.1f));
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cuerda1 fuera izquierda
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.9f, 1.5f, -0.4f));
		model = glm::rotate(model, glm::radians(-54.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 10.5f, 0.1f));
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cuerda1 dentro izquierda
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(1.1f, 1.5f, -0.4f));
		model = glm::rotate(model, glm::radians(-54.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 10.5f, 0.1f));
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cuerda2
		//Cuerda2 fuera derecha
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(12.1f, 0.9f, -3.5f));
		model = glm::rotate(model, glm::radians(-53.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 7.6f, 0.1f));
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cuerda2 dentro derecha
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(11.9f, 0.9f, -3.5f));
		model = glm::rotate(model, glm::radians(-53.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 7.6f, 0.1f));
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cuerda2 fuera izquierda
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.9f, 0.9f, -3.5f));
		model = glm::rotate(model, glm::radians(-53.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 7.6f, 0.1f));
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cuerda2 dentro izquierda
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(1.1f, 0.9f, -3.5f));
		model = glm::rotate(model, glm::radians(-53.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 7.6f, 0.1f));
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cuerda3
		//Cuerda3 fuera derecha
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(12.1f, 0.9f, -6.5f));
		model = glm::rotate(model, glm::radians(-63.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 3.4f, 0.1f));
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cuerda3 dentro derecha
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(11.9f, 0.9f, -6.5f));
		model = glm::rotate(model, glm::radians(-63.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 3.4f, 0.1f));
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cuerda3 fuera izquierda
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.9f, 0.9f, -6.5f));
		model = glm::rotate(model, glm::radians(-63.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 3.4f, 0.1f));
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cuerda3 dentro izquierda
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(1.1f, 0.9f, -6.5f));
		model = glm::rotate(model, glm::radians(-63.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 3.4f, 0.1f));
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cuerda4
		//Cuerda4 fuera derecha
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(12.1f, 3.5f, -0.4f));
		model = glm::rotate(model, glm::radians(-56.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 8.5f, 0.1f));
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cuerda4 dentro derecha
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(11.9f, 3.5f, -0.4f));
		model = glm::rotate(model, glm::radians(-56.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 8.5f, 0.1f));
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cuerda4 fuera izquierda
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.9f, 3.5f, -0.4f));
		model = glm::rotate(model, glm::radians(-56.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 8.5f, 0.1f));
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cuerda4 dentro izquierda
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(1.1f, 3.5f, -0.4f));
		model = glm::rotate(model, glm::radians(-56.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 8.5f, 0.1f));
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cuerdas inversas
		//Cuerda1
		//Cuerda1 fuera derecha
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(12.1f, 1.5f, -9.6f));
		model = glm::rotate(model, glm::radians(54.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 10.5f, 0.1f));
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cuerda1 dentro derecha
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(11.9f, 1.5f, -9.6f));
		model = glm::rotate(model, glm::radians(54.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 10.5f, 0.1f));
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cuerda1 fuera izquierda
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.9f, 1.5f, -9.6f));
		model = glm::rotate(model, glm::radians(54.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 10.5f, 0.1f));
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cuerda1 dentro izquierda
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(1.1f, 1.5f, -9.6f));
		model = glm::rotate(model, glm::radians(54.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 10.5f, 0.1f));
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cuerda2
		//Cuerda2 fuera derecha
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(12.1f, 0.9f, -6.5f));
		model = glm::rotate(model, glm::radians(53.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 7.6f, 0.1f));
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cuerda2 dentro derecha
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(11.9f, 0.9f, -6.5f));
		model = glm::rotate(model, glm::radians(53.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 7.6f, 0.1f));
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cuerda2 fuera izquierda
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.9f, 0.9f, -6.5f));
		model = glm::rotate(model, glm::radians(53.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 7.6f, 0.1f));
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cuerda2 dentro izquierda
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(1.1f, 0.9f, -6.5f));
		model = glm::rotate(model, glm::radians(53.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 7.6f, 0.1f));
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cuerda3
		//Cuerda3 fuera derecha
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(12.1f, 0.9f, -6.5f));
		model = glm::rotate(model, glm::radians(-63.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 3.4f, 0.1f));
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cuerda3 dentro derecha
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(11.9f, 0.9f, -3.5f));
		model = glm::rotate(model, glm::radians(63.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 3.4f, 0.1f));
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cuerda3 fuera izquierda
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.9f, 0.9f, -3.5f));
		model = glm::rotate(model, glm::radians(63.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 3.4f, 0.1f));
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cuerda3 dentro izquierda
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(1.1f, 0.9f, -3.5f));
		model = glm::rotate(model, glm::radians(63.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 3.4f, 0.1f));
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cuerda4
		//Cuerda4 fuera derecha
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(12.1f, 3.5f, -9.6f));
		model = glm::rotate(model, glm::radians(56.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 8.5f, 0.1f));
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cuerda4 dentro derecha
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(11.9f, 3.5f, -9.6f));
		model = glm::rotate(model, glm::radians(56.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 8.5f, 0.1f));
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cuerda4 fuera izquierda
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.9f, 3.5f, -9.6f));
		model = glm::rotate(model, glm::radians(56.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 8.5f, 0.1f));
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cuerda4 dentro izquierda
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(1.1f, 3.5f, -9.6f));
		model = glm::rotate(model, glm::radians(56.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 8.5f, 0.1f));
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Banderas

		//Romeo
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, romeo);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(2.5f, 6.25f, 0.3f));
		model = glm::rotate(model, glm::radians(-14.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.0f, 0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//India
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, india);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(4.5f, 5.9f, 0.3f));
		model = glm::rotate(model, glm::radians(-7.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.0f, 0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Zurich
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, zurich);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(6.5f, 5.75f, 0.3f));
		model = glm::scale(model, glm::vec3(1.5f, 1.0f, 0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Uniform
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, uniform);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(8.5f, 5.9f, 0.3f));
		model = glm::rotate(model, glm::radians(7.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.0f, 0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Kilo
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, kilo);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(10.5f, 6.25f, 0.3f));
		model = glm::rotate(model, glm::radians(14.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.0f, 0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cuerda
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture6);
		
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(2.0f, 7.0f, 0.3f));
		model = glm::rotate(model, glm::radians(60.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.05f, 1.25f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(11.0f, 7.0f, 0.3f));
		model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.05f, 1.25f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(3.5f, 6.5f, 0.3f));
		model = glm::rotate(model, glm::radians(80.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.05f, 1.75f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(9.5f, 6.5f, 0.3f));
		model = glm::rotate(model, glm::radians(-80.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.05f, 1.75f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(5.5f, 6.25f, 0.3f));
		model = glm::rotate(model, glm::radians(85.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.05f, 1.75f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(7.5f, 6.25f, 0.3f));
		model = glm::rotate(model, glm::radians(-85.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.05f, 1.75f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Anuncio
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, anuncio);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(3.0f, 3.75f, 0.01f));
		model = glm::scale(model, glm::vec3(2.0f, 1.5f, 0.01f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Krusty Krab
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, crustaceo);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-4.5f, 10.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture6);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-4.6f, 4.25f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 8.5f, 0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		
		//Carga de modelos
		//Mesas
        view = camera.GetViewMatrix();	
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(3.0f,1.0f, -5.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.75f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		mesa.Draw(lightingShader);

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(9.0f, 1.0f, -5.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.75f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		mesa.Draw(lightingShader);

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(3.0f, 1.0f, -2.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.75f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		mesa.Draw(lightingShader);
		
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(9.0f, 1.0f, -2.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.75f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		mesa.Draw(lightingShader);

		//Bote
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(6.5f, 0.03f, -8.0f));
		model = glm::scale(model, glm::vec3(0.17f, 0.25f, 0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		bote.Draw(lightingShader);

		//Cangreburguers
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(movCangreX, 2.0f, movCangreZ));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		cangre.Draw(lightingShader);

		if (dibuja1)
		{
			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(9.0f, 2.0f, -5.0f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			cangre.Draw(lightingShader);
		}

		if (dibuja2)
		{
			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(3.0f, 2.0f, -5.0f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			cangre.Draw(lightingShader);
		}

		//Barril
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(4.0f, 0.0f, -2.0f));
		model = glm::scale(model, glm::vec3(0.010f, 0.010f, 0.010f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		barril.Draw(lightingShader);

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(4.0f, 0.0f, -3.0f));
		model = glm::scale(model, glm::vec3(0.010f, 0.010f, 0.010f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		barril.Draw(lightingShader);

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(4.0f, 0.0f, -5.0f));
		model = glm::scale(model, glm::vec3(0.010f, 0.010f, 0.010f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		barril.Draw(lightingShader);

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(8.5f, 0.0f, -2.0f));
		model = glm::scale(model, glm::vec3(0.010f, 0.010f, 0.010f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		barril.Draw(lightingShader);

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(8.5f, 0.0f, -3.0f));
		model = glm::scale(model, glm::vec3(0.010f, 0.010f, 0.010f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		barril.Draw(lightingShader);

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(8.5f, 0.0f, -5.0f));
		model = glm::scale(model, glm::vec3(0.010f, 0.010f, 0.010f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		barril.Draw(lightingShader);


		glBindVertexArray(0);


		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		glBindVertexArray(lightVAO);
		//for (GLuint i = 0; i < 4; i++)
		//{
		//	model = glm::mat4(1);
		//	model = glm::translate(model, pointLightPositions[i]);
		//	model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		//	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//	glDrawArrays(GL_TRIANGLES, 0, 36);
		//}
		/*glBindVertexArray(0);*/


		// Draw skybox as last
		glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
		SkyBoxshader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // Set depth function back to default


		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	/*glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);*/
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
	if (keys[GLFW_KEY_1])
	{
		range += 0.1;
		printf("El rango es %f\n", range);
	}

	if (keys[GLFW_KEY_2])
	{
		range -= 0.1;
		printf("El rango es %f\n", range);
	}

	if (keys[GLFW_KEY_3])
	{
		range += 0.1;
		printf("El spotangle es %f\n", range);
	}

	if (keys[GLFW_KEY_4])
	{
		range -= 0.1;
		printf("El spotangle es %f\n", range);
	}

	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);


	}

	if (keys[GLFW_KEY_T])
	{
		//pointLightPositions[0].x -= 0.1f;
		//pointLightPositions[0].y -= 0.1f;
		pointLightPositions[0].z += 0.1f;
	}
	if (keys[GLFW_KEY_G])
	{
		//pointLightPositions[0].x -= 0.1f;
		//pointLightPositions[0].y -= 0.1f;
		pointLightPositions[0].z -= 0.1f;
	}

	if (keys[GLFW_KEY_Y])
	{
		pointLightPositions[1].x += 0.1f;
		pointLightPositions[1].y += 0.1f;
		pointLightPositions[1].z += 0.1f;
	}

	if (keys[GLFW_KEY_H])
	{
		pointLightPositions[1].x -= 0.1f;
		pointLightPositions[1].y -= 0.1f;
		pointLightPositions[1].z -= 0.1f;
	}
	if (keys[GLFW_KEY_U])
	{
		pointLightPositions[2].x += 0.1f;
		pointLightPositions[2].y += 0.1f;
		pointLightPositions[2].z += 0.1f;
	}
	if (keys[GLFW_KEY_J])
	{
		pointLightPositions[2].x -= 0.1f;
		pointLightPositions[2].y -= 0.1f;
		pointLightPositions[2].z -= 0.1f;
	}
	if (keys[GLFW_KEY_I])
	{
		pointLightPositions[3].x += 0.1f;
		pointLightPositions[3].y += 0.1f;
		pointLightPositions[3].z += 0.1f;
	}
	if (keys[GLFW_KEY_K])
	{
		pointLightPositions[3].x -= 0.1f;
		pointLightPositions[3].y -= 0.1f;
		pointLightPositions[3].z -= 0.1f;
	}

}


// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
		{
			LightP1 = glm::vec3(1.0f, 0.0f, 0.0f);
			LightP2 = glm::vec3(1.0f, 1.0f, 0.0f);
			LightP3 = glm::vec3(0.0f, 1.0f, 1.0f);
			LightP4 = glm::vec3(1.0f, 0.0f, 1.0f);
		}
			
		else
		{
			LightP1 = glm::vec3(0.0f, 0.0f, 0.0f);
			LightP2 = glm::vec3(0.0f, 0.0f, 0.0f);
			LightP3 = glm::vec3(0.0f, 0.0f, 0.0f);
			LightP4 = glm::vec3(0.0f, 0.0f, 0.0f);
		}
	}

	/*if (keys[GLFW_KEY_Z])
	{
		if (indice_modelos < 3)
			indice_modelos++;
		else
			indice_modelos = 0;

		printf("indice = %d", indice_modelos);
	}*/
}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

void animacion()
{
	if (recorrido1)
	{
		movCangreX = 6.5f;
		movCangreZ += 0.01f;
		if (movCangreZ > -5.0f)
		{
			recorrido1 = false;
			recorrido2 = true;
		}
	}
	if (recorrido2)
	{
		movCangreX += 0.01f;
		if (movCangreX > 9.0f)
		{
			recorrido2 = false;
			recorrido3 = true;
			movCangreZ = -8.0f;
			dibuja1 = true;
			dibuja2 = false;

		}
	}
	if (recorrido3)
	{
		movCangreX = 6.5f;
		movCangreZ += 0.01f;
		if (movCangreZ > -5.0f)
		{
			recorrido3 = false;
			recorrido4 = true;
		}
	}
	if (recorrido4)
	{
		movCangreX -= 0.01f;
		if (movCangreX < 3.0f)
		{
			recorrido4 = false;
			recorrido1 = true;
			movCangreZ = -8.0f;
			dibuja2 = true;
			dibuja1 = false;

		}
	}
}

void animacion2()
{
	if (vuelta1)
	{
		angGrub -= 0.05f;
		movGrubX -= 0.0025f;
		if (angGrub < -10.0f)
		{
			vuelta1 = false;
			vuelta2 = true;
		}

	}
	if (vuelta2)
	{
		angGrub += 0.05f;
		movGrubX += 0.0025f;
			if (angGrub > 10.0f)
			{
				vuelta2 = false;
				vuelta1 = true;
			}

	}
}