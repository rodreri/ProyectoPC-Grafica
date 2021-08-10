/*
Minero Pineda Erick Rodrigo
Zarco Mu�oz Gabriel 
Semestre 2021-2
Grupo de lab: 07
Grupo de teor�a: 05
*/

//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"


//para iluminaci�n
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Material.h"

//incluimos la clase animacion
#include "anima.h"

//Para el sonido
#include <Windows.h>
#include <mmsystem.h>


const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture dadoTexture;
Texture pisoTexture;
Texture Tagave;

//______________________________AQUI VAN LAS VARIABLES DE MIS MODELOS
Model Esta;
Model Piramide;
Model Souv;
Model Soll;
Model Lunaa;
Model Car;
Model Car2;
Model Arbol;
Model Banio;
Model Humano;
Model Mesa;
Model Quetzal;
Model Taquilla;

Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;

//luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;


// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//c�lculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int * indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}




void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
		0, 1, 2,
		0, 2, 3,
		4,5,6,
		4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
	};
	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

	Mesh *obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

}

void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		4, 5, 6,
		6, 7, 4,
		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,
	};


	GLfloat cubo_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.27f,  0.35f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.48f,	0.35f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.48f,	0.64f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.27f,	0.64f,		0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.52f,  0.35f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	0.73f,	0.35f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	0.73f,	0.64f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.52f,	0.64f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.5f, -0.5f, -0.5f,	0.77f,	0.35f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		0.98f,	0.35f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		0.98f,	0.64f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.77f,	0.64f,		0.0f,	0.0f,	1.0f,

		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.0f,	0.35f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	0.23f,  0.35f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	0.23f,	0.64f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.0f,	0.64f,		1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.27f,	0.02f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	0.48f,  0.02f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	0.48f,	0.31f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.27f,	0.31f,		0.0f,	1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.27f,	0.68f,		0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	0.48f,  0.68f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	0.48f,	0.98f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.27f,	0.98f,		0.0f,	-1.0f,	0.0f,

	};

	Mesh *cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(cubo);

}



void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


//---------------------------------------ANIMACIONES---------------------------------------------
//creo mis variables una por cada objeto que voy a mover

anima Sol;
bool noche=false;



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CrearCubo();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 1.0f, 0.5f);

	//_________________________________________________AQUI CARGAMOS EL PISO
	pisoTexture = Texture("Textures/pasto.tga");
	pisoTexture.LoadTextureA();


	
	//___________________________________________________AQUI CREAMOS VARIABLES DE TIPO MODEL
	Soll = Model();
	Soll.LoadModel("Models/Sol2.obj");
	Lunaa = Model();
	Lunaa.LoadModel("Models/Luna2.obj");

	Piramide = Model();
	Piramide.LoadModel("Models/piramid.obj");

	Souv = Model();
	Souv.LoadModel("Models/Souv.obj");

	Car = Model();
	Car.LoadModel("Models/huracan2.obj");

	Car2 = Model();
	Car2.LoadModel("Models/mustang.obj");

	Arbol = Model();
	Arbol.LoadModel("Models/arbol.obj");

	Banio = Model();
	Banio.LoadModel("Models/banio.obj");

	Humano = Model();
	Humano.LoadModel("Models/humano3.obj");

	Mesa = Model();
	Mesa.LoadModel("Models/mesa3.obj");

	Quetzal = Model();
	Quetzal.LoadModel("Models/quetzalcoatl3.obj");

	Taquilla = Model();
	Taquilla.LoadModel("Models/Taquilla.obj");



	//____________________________________________________TERMINA INSTANCIAS DE MODELOS

	//__________________________________________________________SKYBOX
	std::vector<std::string> skyboxFaces;	
	skyboxFaces.push_back("Textures/Skybox/Verde/left.tga");
	skyboxFaces.push_back("Textures/Skybox/Verde/right.tga");
	skyboxFaces.push_back("Textures/Skybox/Verde/down.tga");
	skyboxFaces.push_back("Textures/Skybox/Verde/up.tga");
	skyboxFaces.push_back("Textures/Skybox/Verde/back.tga");
	skyboxFaces.push_back("Textures/Skybox/Verde/front.tga");
	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	

	//luz direccional, s�lo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;

	//______________________________________________________A PARTIR DE AQUI VAN LAS LUCES

	unsigned int spotLightCount = 0;
	//linterna
	//spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f, //Color
	//	0.0f, 2.0f,
	//	-225.0f, 0.0f, -5.5f, //Posici�n
	//	0.0f, -1.0f, 0.0f, //Hacia d�nde alumbra
	//	1.0f, 0.0f, 0.0f,
	//	5.0f);
	//spotLightCount++;
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		20.0f, 40.0f,
		-225.0f, 0.0f, -5.5f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;
	

	

	//_____________________________________________________AQUI TERMINAN LAS LUCES

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 300.0f);
	

	//_______________________________________________________AQUI DIGO COMO SE VA A MOVER ANIMACION
	
	// KEY FRAMES SOL

	Sol.KeyFrame[0].X = 0.0f; 
	Sol.KeyFrame[0].Y = 0.0f;

	Sol.KeyFrame[1].X = 0.0f;
	Sol.KeyFrame[1].Y = 20.0f;

	Sol.KeyFrame[2].X = 0.0f;
	Sol.KeyFrame[2].Y = 40.0f;

	Sol.KeyFrame[3].X = 0.0f;
	Sol.KeyFrame[3].Y = 20.0f;

	Sol.KeyFrame[4].X = 0.0f;
	Sol.KeyFrame[4].Y = 0.0f;


	/************NOTA
		UNA VEZ TERMINADA LA ANIMACION SE REGRESA AL PUNTO INICIAL
		SI NO QUIERES QUE PASE ESTO COMENTA LA LINEA 
		resetall()
		en la funcion animacion en el doc Anima.cpp
	*/

	//Sonido de fondo
	//PlaySound("Sounds/Aves.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);


	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		glfwPollEvents();

		//Manejo de camaras
		if (mainWindow.getNumCamara() == 0) {

			camera.keyControl(mainWindow.getsKeys(), deltaTime);
			camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		}
		else if (mainWindow.getNumCamara() == 2) {
			//CAMARA DEL AVATAR
			camera.setPosicion(glm::vec3(30.0f, 5.0f, -5.5f));
			camera.setDireccion(glm::vec3(-1.0f, 0.0f, 0.0f));
			camera.setGiro(0,-15);

		}
		else if (mainWindow.getNumCamara() == 4) {
			camera.setPosicion(glm::vec3(0.0 + camera.getCameraPosition().x, 100.0f, 0.0f + camera.getCameraPosition().z));
			camera.setDireccion(glm::vec3(-1.0f, 0.0f, 0.0f));
			camera.setGiro(0, -90);
			camera.keyControl2(mainWindow.getsKeys(), deltaTime);
		}


		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

	


		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();

		//informaci�n en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);		
		
		//informaci�n al shader de fuentes de iluminaci�n
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);


		glm::mat4 model(1.0);
		glm::mat4 modeaux(1.0);

		//Cargando el piso
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pisoTexture.UseTexture();
		//agregar material al plano de piso
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		//________________________________________________A PARTIR DE AQUI CARGO MODELOS

		//Cargando las piramides
		{
			//Cargando Piramide del SOL
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(170.0f, 0.0f, 10.0f));
			model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Piramide.RenderModel();

			//Cargando Piramide de la LUNA
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -200.0f));
			model = glm::scale(model, glm::vec3(11.0f, 11.0f, 11.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Piramide.RenderModel();
		}

		//Vegetacion borde
		{		
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-290.0f, 0.0f, 35.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-290.0f, 0.0f, 70.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-290.0f, 0.0f, 105.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-290.0f, 0.0f, 140.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-290.0f, 0.0f, 175.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-290.0f, 0.0f, 210.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-290.0f, 0.0f, 245.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-290.0f, 0.0f, 280.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//-----------------------------------------------------------------------------
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-290.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-290.0f, 0.0f, -35.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-290.0f, 0.0f, -70.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-290.0f, 0.0f, -105.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-290.0f, 0.0f, -140.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-290.0f, 0.0f, -175.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-290.0f, 0.0f, -210.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-290.0f, 0.0f, -245.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-290.0f, 0.0f, -280.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//___________________________________________________________________________
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(290.0f, 0.0f, 35.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(290.0f, 0.0f, 70.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(290.0f, 0.0f, 105.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(290.0f, 0.0f, 140.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(290.0f, 0.0f, 175.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(290.0f, 0.0f, 210.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(290.0f, 0.0f, 245.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(290.0f, 0.0f, 280.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//-----------------------------------------------------------------------------
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(290.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(290.0f, 0.0f, -35.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(290.0f, 0.0f, -70.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(290.0f, 0.0f, -105.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(290.0f, 0.0f, -140.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(290.0f, 0.0f, -175.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(290.0f, 0.0f, -210.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(290.0f, 0.0f, -245.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(290.0f, 0.0f, -280.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//___________________________________________________________________________


			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0, 0.0f, -280.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(35.0f, 0.0f, -280.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(70.0f, 0.0f, -280.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(105.f, 0.0f, -280.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(140.0, 0.0f, -280.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(175.0f, 0.0f, -280.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(210.0f, 0.0f, -280.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(245.f, 0.0f, -280.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//----------------------------------------------------------------------
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-35.0f, 0.0f, -280.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-70.0f, 0.0f, -280.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-105.f, 0.0f, -280.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-140.0, 0.0f, -280.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-175.0f, 0.0f, -280.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-210.0f, 0.0f, -280.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-245.f, 0.0f, -280.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//_-_----________________----____________________________________________________
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0, 0.0f, 280.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(35.0f, 0.0f, 280.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(70.0f, 0.0f, 280.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(105.f, 0.0f, 280.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(140.0, 0.0f, 280.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(175.0f, 0.0f, 280.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(210.0f, 0.0f, 280.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(245.f, 0.0f, 280.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//----------------------------------------------------------------------
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-35.0f, 0.0f, 280.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-70.0f, 0.0f, 280.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-105.f, 0.0f, 280.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-140.0, 0.0f, 280.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-175.0f, 0.0f, 280.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-210.0f, 0.0f, 280.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
			//Cargando el �rbol
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-245.f, 0.0f, 280.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Arbol.RenderModel();
		}

		
		//Cargando estacionamiento derecho
		{
			//Vegetacion
			{
				//Zona del estacionamiento 
				{
					//Cargando el �rbol
					model = glm::mat4(1.0);
					model = glm::translate(model, glm::vec3(-230.0f, 0.0f, -250.0f));
					model = glm::scale(model, glm::vec3(0.025f, 0.03f, 0.025f));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
					Arbol.RenderModel();
					//Cargando el �rbol
					model = glm::mat4(1.0);
					model = glm::translate(model, glm::vec3(-230.0f, 0.0f, -245.0f));
					model = glm::scale(model, glm::vec3(0.025f, 0.03f, 0.025f));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
					Arbol.RenderModel();
					//Cargando el �rbol
					model = glm::mat4(1.0);
					model = glm::translate(model, glm::vec3(-230.0f, 0.0f, -240.0f));
					model = glm::scale(model, glm::vec3(0.025f, 0.03f, 0.025f));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
					Arbol.RenderModel();
					//Cargando el �rbol
					model = glm::mat4(1.0);
					model = glm::translate(model, glm::vec3(-230.0f, 0.0f, -235.0f));
					model = glm::scale(model, glm::vec3(0.025f, 0.03f, 0.025f));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
					Arbol.RenderModel();
					//Cargando el �rbol
					model = glm::mat4(1.0);
					model = glm::translate(model, glm::vec3(-230.0f, 0.0f, -230.0f));
					model = glm::scale(model, glm::vec3(0.025f, 0.03f, 0.025f));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
					Arbol.RenderModel();
					//Cargando el �rbol
					model = glm::mat4(1.0);
					model = glm::translate(model, glm::vec3(-230.0f, 0.0f, -225.0f));
					model = glm::scale(model, glm::vec3(0.025f, 0.03f, 0.025f));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
					Arbol.RenderModel();
					//Cargando el �rbol
					model = glm::mat4(1.0);
					model = glm::translate(model, glm::vec3(-230.0f, 0.0f, -220.0f));
					model = glm::scale(model, glm::vec3(0.025f, 0.03f, 0.025f));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
					Arbol.RenderModel();
					//Cargando el �rbol
					model = glm::mat4(1.0);
					model = glm::translate(model, glm::vec3(-230.0f, 0.0f, -215.0f));
					model = glm::scale(model, glm::vec3(0.025f, 0.03f, 0.025f));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
					Arbol.RenderModel();
					//Cargando el �rbol
					model = glm::mat4(1.0);
					model = glm::translate(model, glm::vec3(-230.0f, 0.0f, -210.0f));
					model = glm::scale(model, glm::vec3(0.025f, 0.03f, 0.025f));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
					Arbol.RenderModel();
					//Cargando el �rbol
					model = glm::mat4(1.0);
					model = glm::translate(model, glm::vec3(-230.0f, 0.0f, -205.0f));
					model = glm::scale(model, glm::vec3(0.025f, 0.03f, 0.025f));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
					Arbol.RenderModel();
					//Cargando el �rbol
					model = glm::mat4(1.0);
					model = glm::translate(model, glm::vec3(-230.0f, 0.0f, -200.0f));
					model = glm::scale(model, glm::vec3(0.025f, 0.03f, 0.025f));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
					Arbol.RenderModel();

					//llllllllllllllll
					//Cargando el �rbol
					model = glm::mat4(1.0);
					model = glm::translate(model, glm::vec3(-175.0f, 0.0f, -250.0f));
					model = glm::scale(model, glm::vec3(0.025f, 0.03f, 0.025f));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
					Arbol.RenderModel();
					//Cargando el �rbol
					model = glm::mat4(1.0);
					model = glm::translate(model, glm::vec3(-175.0f, 0.0f, -245.0f));
					model = glm::scale(model, glm::vec3(0.025f, 0.03f, 0.025f));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
					Arbol.RenderModel();
					//Cargando el �rbol
					model = glm::mat4(1.0);
					model = glm::translate(model, glm::vec3(-175.0f, 0.0f, -240.0f));
					model = glm::scale(model, glm::vec3(0.025f, 0.03f, 0.025f));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
					Arbol.RenderModel();
					//Cargando el �rbol
					model = glm::mat4(1.0);
					model = glm::translate(model, glm::vec3(-175.0f, 0.0f, -235.0f));
					model = glm::scale(model, glm::vec3(0.025f, 0.03f, 0.025f));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
					Arbol.RenderModel();
					//Cargando el �rbol
					model = glm::mat4(1.0);
					model = glm::translate(model, glm::vec3(-175.0f, 0.0f, -230.0f));
					model = glm::scale(model, glm::vec3(0.025f, 0.03f, 0.025f));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
					Arbol.RenderModel();
					//Cargando el �rbol
					model = glm::mat4(1.0);
					model = glm::translate(model, glm::vec3(-175.0f, 0.0f, -225.0f));
					model = glm::scale(model, glm::vec3(0.025f, 0.03f, 0.025f));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
					Arbol.RenderModel();
					//Cargando el �rbol
					model = glm::mat4(1.0);
					model = glm::translate(model, glm::vec3(-175.0f, 0.0f, -220.0f));
					model = glm::scale(model, glm::vec3(0.025f, 0.03f, 0.025f));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
					Arbol.RenderModel();
					//Cargando el �rbol
					model = glm::mat4(1.0);
					model = glm::translate(model, glm::vec3(-175.0f, 0.0f, -215.0f));
					model = glm::scale(model, glm::vec3(0.025f, 0.03f, 0.025f));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
					Arbol.RenderModel();
					//Cargando el �rbol
					model = glm::mat4(1.0);
					model = glm::translate(model, glm::vec3(-175.0f, 0.0f, -210.0f));
					model = glm::scale(model, glm::vec3(0.025f, 0.03f, 0.025f));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
					Arbol.RenderModel();
					//Cargando el �rbol
					model = glm::mat4(1.0);
					model = glm::translate(model, glm::vec3(-175.0f, 0.0f, -205.0f));
					model = glm::scale(model, glm::vec3(0.025f, 0.03f, 0.025f));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
					Arbol.RenderModel();
					//Cargando el �rbol
					model = glm::mat4(1.0);
					model = glm::translate(model, glm::vec3(-175.0f, 0.0f, -200.0f));
					model = glm::scale(model, glm::vec3(0.025f, 0.03f, 0.025f));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
					Arbol.RenderModel();
					//oooooooooooooooooooooooooooooooooooo
					//Cargando el �rbol
					model = glm::mat4(1.0);
					model = glm::translate(model, glm::vec3(-230.0f, 0.0f, -255.0f));
					model = glm::scale(model, glm::vec3(0.025f, 0.03f, 0.025f));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
					Arbol.RenderModel();
					//Cargando el �rbol
					model = glm::mat4(1.0);
					model = glm::translate(model, glm::vec3(-225.0f, 0.0f, -255.0f));
					model = glm::scale(model, glm::vec3(0.025f, 0.03f, 0.025f));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
					Arbol.RenderModel();
					//Cargando el �rbol
					model = glm::mat4(1.0);
					model = glm::translate(model, glm::vec3(-220.0f, 0.0f, -255.0f));
					model = glm::scale(model, glm::vec3(0.025f, 0.03f, 0.025f));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
					Arbol.RenderModel();
					//Cargando el �rbol
					model = glm::mat4(1.0);
					model = glm::translate(model, glm::vec3(-215.0f, 0.0f, -255.0f));
					model = glm::scale(model, glm::vec3(0.025f, 0.03f, 0.025f));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
					Arbol.RenderModel();
					//Cargando el �rbol
					model = glm::mat4(1.0);
					model = glm::translate(model, glm::vec3(-210.0f, 0.0f, -255.0f));
					model = glm::scale(model, glm::vec3(0.025f, 0.03f, 0.025f));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
					Arbol.RenderModel();
					//Cargando el �rbol
					model = glm::mat4(1.0);
					model = glm::translate(model, glm::vec3(-205.0f, 0.0f, -255.0f));
					model = glm::scale(model, glm::vec3(0.025f, 0.03f, 0.025f));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
					Arbol.RenderModel();
					//Cargando el �rbol
					model = glm::mat4(1.0);
					model = glm::translate(model, glm::vec3(-200.0f, 0.0f, -255.0f));
					model = glm::scale(model, glm::vec3(0.025f, 0.03f, 0.025f));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
					Arbol.RenderModel();
					//Cargando el �rbol
					model = glm::mat4(1.0);
					model = glm::translate(model, glm::vec3(-195.0f, 0.0f, -255.0f));
					model = glm::scale(model, glm::vec3(0.025f, 0.03f, 0.025f));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
					Arbol.RenderModel();
					//Cargando el �rbol
					model = glm::mat4(1.0);
					model = glm::translate(model, glm::vec3(-190.0f, 0.0f, -255.0f));
					model = glm::scale(model, glm::vec3(0.025f, 0.03f, 0.025f));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
					Arbol.RenderModel();
					//Cargando el �rbol
					model = glm::mat4(1.0);
					model = glm::translate(model, glm::vec3(-185.0f, 0.0f, -255.0f));
					model = glm::scale(model, glm::vec3(0.025f, 0.03f, 0.025f));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
					Arbol.RenderModel();
					//Cargando el �rbol
					model = glm::mat4(1.0);
					model = glm::translate(model, glm::vec3(-180.0f, 0.0f, -255.0f));
					model = glm::scale(model, glm::vec3(0.025f, 0.03f, 0.025f));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
					Arbol.RenderModel();

					//Cargando el �rbol
					model = glm::mat4(1.0);
					model = glm::translate(model, glm::vec3(-175.0f, 0.0f, -255.0f));
					model = glm::scale(model, glm::vec3(0.025f, 0.03f, 0.025f));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
					Arbol.RenderModel();
				}
			}

			//Fila 1
			{
				//Cargando Lamborghini
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-180.0f, 0.0f, -250.0f));
				model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
				model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Car.RenderModel();
				//Cargando el mustang
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-185.0f, 0.0f, -250.0));
				model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
				model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Car2.RenderModel();

				//Cargando Lamborghini
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-200.0f, 0.0f, -250.0f));
				model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
				model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Car.RenderModel();
				//Cargando el mustang
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-205.0f, 0.0f, -250.0));
				model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
				model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Car2.RenderModel();

				//Cargando Lamborghini
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-220.0f, 0.0f, -250.0f));
				model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
				model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Car.RenderModel();
				//Cargando el mustang
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-225.0f, 0.0f, -250.0));
				model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
				model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Car2.RenderModel();

			}

			//Fila 2
			{
				//Cargando Lamborghini
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-180.0f, 0.0f, -240.0f));
				model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
				model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Car.RenderModel();
				//Cargando el mustang
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-185.0f, 0.0f, -240.0));
				model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
				model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Car2.RenderModel();

				//Cargando Lamborghini
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-200.0f, 0.0f, -240.0f));
				model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
				model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Car.RenderModel();
				//Cargando el mustang
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-205.0f, 0.0f, -240.0));
				model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
				model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Car2.RenderModel();

				//Cargando Lamborghini
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-220.0f, 0.0f, -240.0f));
				model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
				model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Car.RenderModel();
				//Cargando el mustang
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-225.0f, 0.0f, -240.0));
				model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
				model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Car2.RenderModel();

			}

			//Fila 3
			{
				//Cargando Lamborghini
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-180.0f, 0.0f, -230.0f));
				model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
				model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Car.RenderModel();
				//Cargando el mustang
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-185.0f, 0.0f, -230.0));
				model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
				model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Car2.RenderModel();

				//Cargando Lamborghini
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-200.0f, 0.0f, -230.0f));
				model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
				model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Car.RenderModel();
				//Cargando el mustang
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-205.0f, 0.0f, -230.0));
				model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
				model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Car2.RenderModel();

				//Cargando Lamborghini
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-220.0f, 0.0f, -230.0f));
				model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
				model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Car.RenderModel();
				//Cargando el mustang
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-225.0f, 0.0f, -230.0));
				model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
				model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Car2.RenderModel();

			}

			//Fila 4
			{
				//Cargando Lamborghini
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-180.0f, 0.0f, -220.0f));
				model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
				model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Car.RenderModel();
				//Cargando el mustang
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-185.0f, 0.0f, -220.0));
				model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
				model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Car2.RenderModel();

				//Cargando Lamborghini
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-200.0f, 0.0f, -220.0f));
				model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
				model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Car.RenderModel();
				//Cargando el mustang
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-205.0f, 0.0f, -220.0));
				model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
				model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Car2.RenderModel();

				//Cargando Lamborghini
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-220.0f, 0.0f, -220.0f));
				model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
				model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Car.RenderModel();
				//Cargando el mustang
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-225.0f, 0.0f, -220.0));
				model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
				model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Car2.RenderModel();

			}

			//Fila 5
			{
				//Cargando Lamborghini
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-180.0f, 0.0f, -210.0f));
				model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
				model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Car.RenderModel();
				//Cargando el mustang
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-185.0f, 0.0f, -210.0));
				model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
				model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Car2.RenderModel();

				//Cargando Lamborghini
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-200.0f, 0.0f, -210.0f));
				model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
				model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Car.RenderModel();
				//Cargando el mustang
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-205.0f, 0.0f, -210.0));
				model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
				model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Car2.RenderModel();

				//Cargando Lamborghini
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-220.0f, 0.0f, -210.0f));
				model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
				model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Car.RenderModel();
				//Cargando el mustang
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-225.0f, 0.0f, -210.0));
				model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
				model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Car2.RenderModel();

			}

			//Fila 6
			{
				//Cargando Lamborghini
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-180.0f, 0.0f, -200.0f));
				model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
				model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Car.RenderModel();
				//Cargando el mustang
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-185.0f, 0.0f, -200.0));
				model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
				model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Car2.RenderModel();

				//Cargando Lamborghini
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-200.0f, 0.0f, -200.0f));
				model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
				model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Car.RenderModel();
				//Cargando el mustang
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-205.0f, 0.0f, -200.0));
				model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
				model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Car2.RenderModel();

				//Cargando Lamborghini
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-220.0f, 0.0f, -200.0f));
				model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
				model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Car.RenderModel();
				//Cargando el mustang
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-225.0f, 0.0f, -200.0));
				model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
				model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Car2.RenderModel();

			}
			

		}
		
		

		//Taquilla, ba�os y Souvenirs
		{
			//Cargando los ba�os
			{
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-195.0f, 0.0f, -5.5f));
				model = glm::scale(model, glm::vec3(60.0f, 60.0f, 60.0f));
				model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Banio.RenderModel();

				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-200.0f, 0.0f, -5.5f));
				model = glm::scale(model, glm::vec3(60.0f, 60.0f, 60.0f));
				model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Banio.RenderModel();

				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-205.0f, 0.0f, -5.5f));
				model = glm::scale(model, glm::vec3(60.0f, 60.0f, 60.0f));
				model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Banio.RenderModel();

				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-210.0f, 0.0f, -5.5f));
				model = glm::scale(model, glm::vec3(60.0f, 60.0f, 60.0f));
				model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Banio.RenderModel();

				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-215.0f, 0.0f, -5.5f));
				model = glm::scale(model, glm::vec3(60.0f, 60.0f, 60.0f));
				model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Banio.RenderModel();
			}
			

			//Cargando la taquilla
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-225.0f, 0.0f, -5.5f));
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Taquilla.RenderModel();

			//cargando huamno taquilla
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-225.0f, 0.0f, -5.5f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Humano.RenderModel();

			//Cargando Souvenirs
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(- 180.0f, 0.0f, -5.5f));
			model = glm::scale(model, glm::vec3(1.3f, 1.3f, 1.3f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Souv.RenderModel();
			//cargando huamno souvenirs
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-185.0f, 0.0f, -8.5f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Humano.RenderModel(); 
		}
		

		//Cargando el quetzalcoatl
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(170.0f, 50.0f, 10.5f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Quetzal.RenderModel();


		//Zona de comida
		{
			//Cargando las Mesas
			{
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-195.0f, 0.0f, -25.0f));
				model = glm::scale(model, glm::vec3(1.25f, 1.25f, 1.25f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Mesa.RenderModel();

				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-200.0f, 0.0f, -25.0f));
				model = glm::scale(model, glm::vec3(1.25f, 1.25f, 1.25f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Mesa.RenderModel();

				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-205.0f, 0.0f, -25.0f));
				model = glm::scale(model, glm::vec3(1.25f, 1.25f, 1.25f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Mesa.RenderModel();

				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-210.0f, 0.0f, -25.0f));
				model = glm::scale(model, glm::vec3(1.25f, 1.25f, 1.25f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Mesa.RenderModel();

				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-215.0f, 0.0f, -25.0f));
				model = glm::scale(model, glm::vec3(1.25f, 1.25f, 1.25f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Mesa.RenderModel();
			}
			{
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-195.0f, 0.0f, -30.0f));
				model = glm::scale(model, glm::vec3(1.25f, 1.25f, 1.25f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Mesa.RenderModel();

				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-200.0f, 0.0f, -30.0f));
				model = glm::scale(model, glm::vec3(1.25f, 1.25f, 1.25f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Mesa.RenderModel();

				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-205.0f, 0.0f, -30.0f));
				model = glm::scale(model, glm::vec3(1.25f, 1.25f, 1.25f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Mesa.RenderModel();

				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-210.0f, 0.0f, -30.0f));
				model = glm::scale(model, glm::vec3(1.25f, 1.25f, 1.25f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Mesa.RenderModel();

				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-215.0f, 0.0f, -30.0f));
				model = glm::scale(model, glm::vec3(1.25f, 1.25f, 1.25f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Mesa.RenderModel();
			}

			{
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-195.0f, 0.0f, -35.0f));
				model = glm::scale(model, glm::vec3(1.25f, 1.25f, 1.25f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Mesa.RenderModel();

				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-200.0f, 0.0f, -35.0f));
				model = glm::scale(model, glm::vec3(1.25f, 1.25f, 1.25f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Mesa.RenderModel();

				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-205.0f, 0.0f, -35.0f));
				model = glm::scale(model, glm::vec3(1.25f, 1.25f, 1.25f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Mesa.RenderModel();

				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-210.0f, 0.0f, -35.0f));
				model = glm::scale(model, glm::vec3(1.25f, 1.25f, 1.25f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Mesa.RenderModel();

				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-215.0f, 0.0f, -35.0f));
				model = glm::scale(model, glm::vec3(1.25f, 1.25f, 1.25f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Mesa.RenderModel();
			}

			
		}


		//Personas
		{
			//Cargando el humano
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(51.0f, 0.0f, -5.0f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Humano.RenderModel();

			//Cargando el humano
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(48.0f, 0.0f, -5.0f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Humano.RenderModel();

			//Cargando el humano
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(45.0f, 0.0f, -5.0f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Humano.RenderModel();

			//Cargando el humano
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(51.0f, 0.0f, -5.5f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Humano.RenderModel();

			//Cargando el humano
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(48.0f, 0.0f, -5.5f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Humano.RenderModel();

			//Cargando el humano
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(45.0f, 0.0f, -5.5f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Humano.RenderModel();

			//Cargando el humano
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(51.0f, 0.0f, -6.0f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Humano.RenderModel();

			//Cargando el humano
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(48.0f, 0.0f, -6.0f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Humano.RenderModel();

			//Cargando el humano
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(45.0f, 0.0f, -6.0f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Humano.RenderModel();

			//Cargando el humano
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(51.0f, 0.0f, -6.5f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Humano.RenderModel();

			//Cargando el humano
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(48.0f, 0.0f, -6.5f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Humano.RenderModel();

			//Cargando el humano
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(45.0f, 0.0f, -6.5f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Humano.RenderModel();

			//Cargando el humano
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(51.0f, 0.0f, -7.0f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Humano.RenderModel();

			//Cargando el humano
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(48.0f, 0.0f, -7.0f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Humano.RenderModel();

			//Cargando el humano
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(45.0f, 0.0f, -7.0f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Humano.RenderModel();

			//Cargando el humano
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(51.0f, 0.0f, -7.5f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Humano.RenderModel();

			//Cargando el humano
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(48.0f, 0.0f, -7.5f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Humano.RenderModel();

			//Cargando el humano
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(45.0f, 0.0f, -7.5f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Humano.RenderModel();

			//Cargando el humano
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(51.0f, 0.0f, -8.0f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Humano.RenderModel();

			//Cargando el humano
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(48.0f, 0.0f, -8.0f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Humano.RenderModel();

			//Cargando el humano
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(45.0f, 0.0f, -8.0f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Humano.RenderModel();

			//Cargando el humano
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(51.0f, 0.0f, -8.5f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Humano.RenderModel();

			//Cargando el humano
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(48.0f, 0.0f, -8.5f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Humano.RenderModel();

			//Cargando el humano
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(45.0f, 0.0f, -8.5f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Humano.RenderModel();
			//;lllllllllllllllllllllllllllllllllllllllllllllll
			//Cargando el humano
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(51.0f, 0.0f, -9.0f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Humano.RenderModel();

			//Cargando el humano
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(48.0f, 0.0f, -9.0f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Humano.RenderModel();

			//Cargando el humano
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(45.0f, 0.0f, -9.0f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Humano.RenderModel();

			//Cargando el humano
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(51.0f, 0.0f, -9.5f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Humano.RenderModel();

			//Cargando el humano
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(48.0f, 0.0f, -9.5f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Humano.RenderModel();

			//Cargando el humano
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(45.0f, 0.0f, -9.5f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Humano.RenderModel();

			//Cargando el humano
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(51.0f, 0.0f, -10.0f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Humano.RenderModel();

			//Cargando el humano
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(48.0f, 0.0f, -10.0f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Humano.RenderModel();

			//Cargando el humano
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(45.0f, 0.0f, -10.0f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Humano.RenderModel();
		}
		
		

		
		
		//KEYFRAMES QUE GENERAdos
		Sol.setMaxIndex(5);

		//iniciamos animacion
		Sol.animacion();


		//Sol
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(170.0f, 80.0f + Sol.Y, -13.0f));
		model = glm::rotate(model, Sol.GY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Soll.RenderModel();

		//luna
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 40.0f + Sol.Y, -215.0f));
		model = glm::rotate(model, Sol.GY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lunaa.RenderModel();


		//TOMANDO ESTO COMO EJEMPLO PARA PONER LA ANIMACION LUNA SOL
		////logica para el cambio dia noche

		//if (!Sol.play) {
		//	 noche = !noche;
		//	 Sol.play = true;
		//}


		//if (!noche) {
		//	//render del modelo del sol

		//	model = glm::mat4(1.0);
		//	//En esta linea agregamos lo que se va ir sumando al traslate
		//	model = glm::translate(model, glm::vec3(0.0f+Sol.X, 0.0f+Sol.Y, 0.0f+Sol.Z));
		//	/*Para el giro solo se puede manejar asi  en cualquier modelo:
		//	model = glm::rotate(model, Sol.GX * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		//	model = glm::rotate(model, Sol.GY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//	model = glm::rotate(model, Sol.GZ * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		//	*/
		//	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//	Soll.RenderModel();
		//}
		//else {
		//	//render del moderlo de Luna

		//	model = glm::mat4(1.0);
		//	//En esta linea agregamos lo que se va ir sumando al traslate
		//	model = glm::translate(model, glm::vec3(0.0f + Sol.X, 0.5f + Sol.Y, 0.0f + Sol.Z));
		//	model = glm::scale(model, glm::vec3(1.3f, 1.3f, 1.3f));
		//	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//	Lunaa.RenderModel();
		//}

		

		
		//__________________________________AQUI DEJO DE CARGAR MODELOS
		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}
