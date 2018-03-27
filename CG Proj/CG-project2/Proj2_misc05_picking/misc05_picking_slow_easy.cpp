﻿// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <array>
#include <stack>   
#include <sstream>
#include <math.h>
// Include GLEW
#include <GL/glew.h>
// Include GLFW
#include <glfw3.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
using namespace glm;
// Include AntTweakBar
#include <AntTweakBar.h>

#include <common/shader.hpp>
#include <common/controls.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>
#include <common/texture.hpp>

const int window_width = 1024, window_height = 768;

typedef struct Vertex {
	float Position[4];
	float Color[4];
	float Normal[3];
	void SetPosition(float *coords) {
		Position[0] = coords[0];
		Position[1] = coords[1];
		Position[2] = coords[2];
		Position[3] = 1.0;
	}
	void SetColor(float *color) {
		Color[0] = color[0];
		Color[1] = color[1];
		Color[2] = color[2];
		Color[3] = color[3];
	}
	void SetNormal(float *coords) {
		Normal[0] = coords[0];
		Normal[1] = coords[1];
		Normal[2] = coords[2];
	}
};

// function prototypes
int initWindow(void);
void initOpenGL(void);
void loadObject(char*, glm::vec4, Vertex * &, GLushort* &, int);
void createVAOs(Vertex[], GLushort[], int);
void createObjects(void);
void pickObject(void);
void renderScene(void);
void cleanup(void);
static void keyCallback(GLFWwindow*, int, int, int, int);
static void mouseCallback(GLFWwindow*, int, int, int);

GLuint Texture;
GLuint TextureID;

// GLOBAL VARIABLES
GLFWwindow* window;

glm::mat4 gProjectionMatrix;
glm::mat4 gViewMatrix;
glm::mat4x4 TranslationMatrix = glm::mat4(1.0);
glm::mat4x4 TopRotationMatrix = glm::mat4(1.0);
glm::mat4x4 A1RotationMatrix = glm::mat4(1.0);
glm::mat4x4 A2RotationMatrix = glm::mat4(1.0);

GLuint gPickedIndex = -1;
std::string gMessage;

GLuint programID;
GLuint pickingProgramID;

const GLuint NumObjects = 15;	// ATTN: THIS NEEDS TO CHANGE AS YOU ADD NEW OBJECTS
GLuint VertexArrayId[NumObjects] = { 0 };
GLuint VertexBufferId[NumObjects] = { 0 };
GLuint IndexBufferId[NumObjects] = { 0 };

size_t NumIndices[NumObjects] = { 0 };
size_t VertexBufferSize[NumObjects] = { 0 };
size_t IndexBufferSize[NumObjects] = { 0 };

GLuint MatrixID;
GLuint ModelMatrixID;
GLuint TranslationMatrixID;
GLuint TopRotationMatrixID;
GLuint A1RotationMatrixID;
GLuint A2RotationMatrixID;
GLuint ViewMatrixID;
GLuint ProjMatrixID;
GLuint PickingMatrixID;
GLuint pickingColorID;
GLuint LightID;
GLuint LightID2;

glm::vec3 lightPos;
glm::vec3 lightPos2;

GLint gX = 0.0;
GLint gZ = 0.0;

float shift[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
float TopRotate = 0.0f;
float A1Rotate = 0.0f;
float A2Rotate = 0.0f;
float PHRotate = 0.0f;
float PVRotate = 0.0f;
float PTWIST = 0.0f;
float BallPosition[4] = { 0.0f, 0.0f, 2.0f, 1.0f };
float time = 0.0f;
float gravity = 9.8f;
float BallIniSpeed = 2.0f; //initial sppeed of ball
glm::vec4 SpeedVector = glm::vec4(double(0.0), double(0.0), double(2.0), double(1.0));
glm::vec4 BallPosi = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
float mat[16];

// animation control
bool animation = false;
GLfloat phi1 = 45.0;    //moving horizontally
GLfloat phi2 = 35.26438968;    //moving vertically
GLfloat up = 1.0;        //directional of "up"

Vertex* Verts;
GLushort* Idcs;

//Base vertices, ProjID=2
Vertex* BVerts;
GLushort* BIdcs;

//Top vertices, ProjID=3
Vertex* TVerts;
GLushort* TIdcs;

//Arm1 vertices, ProjID=4
Vertex* A1Verts;
GLushort* A1Idcs;

//Joint vertices, ProjID=5
Vertex* JVerts;
GLushort* JIdcs;

//Arm2 vertices, ProjID=6
Vertex* A2Verts;
GLushort* A2Idcs;

//Pen vertices, ProjID=7
Vertex* PVerts;
GLushort* PIdcs;

//Button vertices, ProjID=8
Vertex* BUVerts;
GLushort* BUIdcs;

//Store the number of vertex
size_t BVertexNum = NumIndices[2];
size_t TVertexNum = NumIndices[3];
size_t A1VertexNum = NumIndices[4];
size_t JVertexNum = NumIndices[5];
size_t A2VertexNum = NumIndices[6];
size_t PVertexNum = NumIndices[7];
size_t BUVertexNum = NumIndices[8];

void loadObject(char* file, glm::vec4 color, Vertex * &out_Vertices, GLushort* &out_Indices, int ObjectId)
{
	// Read our .obj file
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	bool res = loadOBJ(file, vertices, normals);

	std::vector<GLushort> indices;
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;
	indexVBO(vertices, normals, indices, indexed_vertices, indexed_normals);

	const size_t vertCount = indexed_vertices.size();
	const size_t idxCount = indices.size();

	// populate output arrays
	out_Vertices = new Vertex[vertCount];
	for (int i = 0; i < vertCount; i++) {
		out_Vertices[i].SetPosition(&indexed_vertices[i].x);
		out_Vertices[i].SetNormal(&indexed_normals[i].x);
		out_Vertices[i].SetColor(&color[0]);
	}
	out_Indices = new GLushort[idxCount];
	for (int i = 0; i < idxCount; i++) {
		out_Indices[i] = indices[i];
	}

	// set global variables!!
	NumIndices[ObjectId] = idxCount;
	VertexBufferSize[ObjectId] = sizeof(out_Vertices[0]) * vertCount;
	IndexBufferSize[ObjectId] = sizeof(GLushort) * idxCount;
}


void createObjects(void)
{
	//-- COORDINATE AXES --//
	Vertex CoordVerts[] =
	{
		{ { 0.0, 0.0, 0.0, 1.0 },{ 1.0, 0.0, 0.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { 5.0, 0.0, 0.0, 1.0 },{ 1.0, 0.0, 0.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { 0.0, 0.0, 0.0, 1.0 },{ 0.0, 1.0, 0.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { 0.0, 5.0, 0.0, 1.0 },{ 0.0, 1.0, 0.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { 0.0, 0.0, 0.0, 1.0 },{ 0.0, 0.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { 0.0, 0.0, 5.0, 1.0 },{ 0.0, 0.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
	};

	VertexBufferSize[0] = sizeof(CoordVerts);	// ATTN: this needs to be done for each hand-made object with the ObjectID (subscript)
	createVAOs(CoordVerts, NULL, 0);

	//-- GRID --//

	// ATTN: create your grid vertices here!
	Vertex GridVerts[] =
	{
		//x grids
		{ { -5.0, 0.0, -5.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { -5.0, 0.0,  5.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { -4.0, 0.0, -5.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { -4.0, 0.0,  5.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { -3.0, 0.0, -5.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { -3.0, 0.0,  5.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { -2.0, 0.0, -5.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { -2.0, 0.0,  5.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { -1.0, 0.0, -5.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { -1.0, 0.0,  5.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { 0.0, 0.0, -5.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { 0.0, 0.0,  5.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { 1.0, 0.0, -5.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { 1.0, 0.0,  5.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { 2.0, 0.0, -5.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { 2.0, 0.0,  5.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { 3.0, 0.0, -5.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { 3.0, 0.0,  5.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { 4.0, 0.0, -5.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { 4.0, 0.0,  5.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { 5.0, 0.0, -5.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { 5.0, 0.0,  5.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },

		//z grids
		{ { -5.0, 0.0, -5.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { 5.0, 0.0, -5.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { -5.0, 0.0, -4.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { 5.0, 0.0, -4.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { -5.0, 0.0, -3.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { 5.0, 0.0, -3.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { -5.0, 0.0, -2.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { 5.0, 0.0, -2.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { -5.0, 0.0, -1.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { 5.0, 0.0, -1.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { -5.0, 0.0,  0.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { 5.0, 0.0,  0.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { -5.0, 0.0,  1.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { 5.0, 0.0,  1.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { -5.0, 0.0,  2.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { 5.0, 0.0,  2.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { -5.0, 0.0,  3.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { 5.0, 0.0,  3.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { -5.0, 0.0,  4.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { 5.0, 0.0,  4.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { -5.0, 0.0,  5.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { 5.0, 0.0,  5.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
	};

	VertexBufferSize[1] = sizeof(GridVerts);	// ATTN: this needs to be done for each hand-made object with the ObjectID (subscript)
	createVAOs(GridVerts, NULL, 1);

	//-- .OBJs --//

	// ATTN: load your models here

	//load base
	loadObject("models/base.obj", glm::vec4(1.0, 1.0, 0.0, 1.0), BVerts, BIdcs, 2);
	createVAOs(BVerts, BIdcs, 2);

	loadObject("models/base.obj", glm::vec4(1.0, 1.0, 1.0, 1.0), Verts, Idcs, 9);
	createVAOs(Verts, Idcs, 9);

	//load top
	loadObject("models/top.obj", glm::vec4(0.5, 0.0, 0.5, 1.0), TVerts, TIdcs, 3);
	createVAOs(TVerts, TIdcs, 3);

	loadObject("models/top.obj", glm::vec4(1.0, 1.0, 1.0, 1.0), Verts, Idcs, 10);
	createVAOs(Verts, Idcs, 10);

	//load arm1
	loadObject("models/arm1.obj", glm::vec4(1.0, 0.5, 0.0, 1.0), A1Verts, A1Idcs, 4);
	createVAOs(A1Verts, A1Idcs, 4);

	loadObject("models/arm1.obj", glm::vec4(1.0, 1.0, 1.0, 1.0), Verts, Idcs, 11);
	createVAOs(Verts, Idcs, 11);

	//load joint
	loadObject("models/joint.obj", glm::vec4(0.0, 1.0, 0.0, 1.0), JVerts, JIdcs, 5);
	createVAOs(JVerts, JIdcs, 5);

	//load arm2
	loadObject("models/arm2.obj", glm::vec4(0.0, 0.5, 1.0, 1.0), A2Verts, A2Idcs, 6);
	createVAOs(A2Verts, A2Idcs, 6);

	loadObject("models/arm2.obj", glm::vec4(1.0, 1.0, 1.0, 1.0), Verts, Idcs, 12);
	createVAOs(Verts, Idcs, 12);

	//load pen
	loadObject("models/pen.obj", glm::vec4(0.0, 1.0, 0.5, 1.0), PVerts, PIdcs, 7);
	createVAOs(PVerts, PIdcs, 7);

	loadObject("models/pen.obj", glm::vec4(1.0, 1.0, 1.0, 1.0), Verts, Idcs, 13);
	createVAOs(Verts, Idcs, 13);

	//load button
	loadObject("models/button.obj", glm::vec4(1.0, 0.0, 0.0, 1.0), BUVerts, BUIdcs, 8);
	createVAOs(BUVerts, BUIdcs, 8);

	//load ball
	loadObject("models/ball.obj", glm::vec4(0.3, 0.5, 0.7, 1.0), Verts, Idcs, 14);
	createVAOs(Verts, Idcs, 14);
}

void renderScene(void)
{
	//ATTN: DRAW YOUR SCENE HERE. MODIFY/ADAPT WHERE NECESSARY!

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.2f, 0.0f);
	// Re-clear the screen for real rendering
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(programID);
	{
		//Draw Coordinates and Grids
		glm::mat4x4 ModelMatrix = glm::mat4(1.0);     // TranslationMatrix * RotationMatrix;
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(LightID2, lightPos2.x, lightPos2.y, lightPos2.z);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &gViewMatrix[0][0]);
		glUniformMatrix4fv(ProjMatrixID, 1, GL_FALSE, &gProjectionMatrix[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		glBindVertexArray(VertexArrayId[0]);	// draw CoordAxes
		glDrawArrays(GL_LINES, 0, 6);

		glBindVertexArray(0);

		glBindVertexArray(VertexArrayId[1]);	// draw Grids
		glDrawArrays(GL_LINES, 0, 44);

		glBindVertexArray(0);

		//Draw Models
		ModelMatrix = glm::mat4(1.0);
		TranslationMatrix = glm::mat4
		(
			1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			shift[0], shift[1], shift[2], 1.0
		);
		ModelMatrix = TranslationMatrix * ModelMatrix;
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &gViewMatrix[0][0]);
		glUniformMatrix4fv(ProjMatrixID, 1, GL_FALSE, &gProjectionMatrix[0][0]);
		glUniformMatrix4fv(TranslationMatrixID, 1, GL_FALSE, &TranslationMatrix[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		{
			glBindVertexArray(VertexArrayId[9]);	// draw Base
			glDrawElements(
				GL_TRIANGLES,      // mode
				NumIndices[9],    // count
				GL_UNSIGNED_SHORT,   // type
				(void*)0           // element array buffer offset
			);
		}
		else
		{
			glBindVertexArray(VertexArrayId[2]);	// draw Base
			glDrawElements(
				GL_TRIANGLES,      // mode
				NumIndices[2],    // count
				GL_UNSIGNED_SHORT,   // type
				(void*)0           // element array buffer offset
			);

		}

		glBindVertexArray(0);

		ModelMatrix = glm::mat4(1.0);
		TranslationMatrix = glm::mat4
		(
			1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			shift[0], shift[1], shift[2], 1.0
		);
		TopRotationMatrix = glm::mat4
		(
			cos(TopRotate), 0.0, -1 * sin(TopRotate), 0.0,
			0.0, 1.0, 0.0, 0.0,
			sin(TopRotate), 0.0, cos(TopRotate), 0.0,
			0.0, 0.0, 0.0, 1.0
		);
		ModelMatrix = TranslationMatrix * TopRotationMatrix * ModelMatrix;
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &gViewMatrix[0][0]);
		glUniformMatrix4fv(ProjMatrixID, 1, GL_FALSE, &gProjectionMatrix[0][0]);
		glUniformMatrix4fv(TranslationMatrixID, 1, GL_FALSE, &TranslationMatrix[0][0]);
		glUniformMatrix4fv(TopRotationMatrixID, 1, GL_FALSE, &TopRotationMatrix[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		{
			glBindVertexArray(VertexArrayId[10]);	// draw Top
			glDrawElements(
				GL_TRIANGLES,      // mode
				NumIndices[10],    // count
				GL_UNSIGNED_SHORT,   // type
				(void*)0           // element array buffer offset
			);
		}
		else
		{
			glBindVertexArray(VertexArrayId[3]);	// draw Top
			glDrawElements(
				GL_TRIANGLES,      // mode
				NumIndices[3],    // count
				GL_UNSIGNED_SHORT,   // type
				(void*)0           // element array buffer offset
			);

		}

		glBindVertexArray(0);

		ModelMatrix = glm::mat4(1.0);
		TranslationMatrix = glm::mat4
		(
			1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			shift[0], shift[1] + 2.1, shift[2], 1.0
		);
		A1RotationMatrix = glm::mat4
		(
			cos(A1Rotate), sin(A1Rotate), 0.0, 0.0,
			-1 * sin(A1Rotate), cos(A1Rotate), 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			0.0, 0.0, 0.0, 1.0
		);
		glm::mat4x4 A1OffsetMatrix = glm::mat4
		(
			1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			1.0, 0.0, 0.0, 1.0
		);
		ModelMatrix = TranslationMatrix * TopRotationMatrix * A1RotationMatrix * A1OffsetMatrix * ModelMatrix;
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &gViewMatrix[0][0]);
		glUniformMatrix4fv(ProjMatrixID, 1, GL_FALSE, &gProjectionMatrix[0][0]);
		glUniformMatrix4fv(TranslationMatrixID, 1, GL_FALSE, &TranslationMatrix[0][0]);
		glUniformMatrix4fv(TopRotationMatrixID, 1, GL_FALSE, &TopRotationMatrix[0][0]);
		glUniformMatrix4fv(A1RotationMatrixID, 1, GL_FALSE, &A1RotationMatrix[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		{
			glBindVertexArray(VertexArrayId[11]);	// draw Arm1
			glDrawElements(
				GL_TRIANGLES,      // mode
				NumIndices[11],    // count
				GL_UNSIGNED_SHORT,   // type
				(void*)0           // element array buffer offset
			);
		}
		else
		{
			glBindVertexArray(VertexArrayId[4]);	// draw Arm1
			glDrawElements(
				GL_TRIANGLES,      // mode
				NumIndices[4],    // count
				GL_UNSIGNED_SHORT,   // type
				(void*)0           // element array buffer offset
			);

		}

		glBindVertexArray(0);

		ModelMatrix = glm::mat4(1.0);
		TranslationMatrix = glm::mat4
		(
			1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			shift[0], shift[1] + 2.1, shift[2], 1.0
		);

		glm::mat4x4 JOffsetMatrix = glm::mat4
		(
			1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			1.0, 0.0, 0.0, 1.0

		);
		ModelMatrix = TranslationMatrix * TopRotationMatrix * A1RotationMatrix * A1OffsetMatrix * JOffsetMatrix * ModelMatrix;
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &gViewMatrix[0][0]);
		glUniformMatrix4fv(ProjMatrixID, 1, GL_FALSE, &gProjectionMatrix[0][0]);
		glUniformMatrix4fv(TranslationMatrixID, 1, GL_FALSE, &TranslationMatrix[0][0]);
		glUniformMatrix4fv(TopRotationMatrixID, 1, GL_FALSE, &TopRotationMatrix[0][0]);
		glUniformMatrix4fv(A1RotationMatrixID, 1, GL_FALSE, &A1RotationMatrix[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		glBindVertexArray(VertexArrayId[5]);	// draw Joint
		glDrawElements(
			GL_TRIANGLES,      // mode
			NumIndices[5],    // count
			GL_UNSIGNED_SHORT,   // type
			(void*)0           // element array buffer offset
		);

		glBindVertexArray(0);

		ModelMatrix = glm::mat4(1.0);
		TranslationMatrix = glm::mat4
		(
			1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			shift[0], shift[1], shift[2], 1.0
		);
		A2RotationMatrix = glm::mat4
		(
			double(cos(A2Rotate)), double(sin(A2Rotate)), 0.0, 0.0,
			double(-1 * sin(A2Rotate)), double(cos(A2Rotate)), 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			0.0, 0.0, 0.0, 1.0
		);
		glm::mat4x4 A2OffsetMatrix = glm::mat4
		(
			1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			0.0, -0.9, 0.0, 1.0

		);
		glm::mat4x4 A2Offset2Matrix = glm::mat4
		(
			1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			double(2.0 * cos(A1Rotate)), double(2.1 + 2.0 * sin(A1Rotate)), 0.0, 1.0

		);

		ModelMatrix = TranslationMatrix * TopRotationMatrix * A2Offset2Matrix * A1RotationMatrix * A2RotationMatrix * A2OffsetMatrix * ModelMatrix;
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &gViewMatrix[0][0]);
		glUniformMatrix4fv(ProjMatrixID, 1, GL_FALSE, &gProjectionMatrix[0][0]);
		glUniformMatrix4fv(TranslationMatrixID, 1, GL_FALSE, &TranslationMatrix[0][0]);
		glUniformMatrix4fv(TopRotationMatrixID, 1, GL_FALSE, &TopRotationMatrix[0][0]);
		glUniformMatrix4fv(A1RotationMatrixID, 1, GL_FALSE, &A1RotationMatrix[0][0]);
		glUniformMatrix4fv(A2RotationMatrixID, 1, GL_FALSE, &A2RotationMatrix[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		{
			glBindVertexArray(VertexArrayId[12]);	// draw Arm2
			glDrawElements(
				GL_TRIANGLES,      // mode
				NumIndices[12],    // count
				GL_UNSIGNED_SHORT,   // type
				(void*)0           // element array buffer offset
			);
		}
		else
		{
			glBindVertexArray(VertexArrayId[6]);	// draw Arm2
			glDrawElements(
				GL_TRIANGLES,      // mode
				NumIndices[6],    // count
				GL_UNSIGNED_SHORT,   // type
				(void*)0           // element array buffer offset
			);
		}

		glBindVertexArray(0);

		ModelMatrix = glm::mat4(1.0);
		TranslationMatrix = glm::mat4
		(
			1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			shift[0], shift[1], shift[2], 1.0
		);
		glm::mat4x4 POffsetMatrix = glm::mat4
		(
			1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			0.0, 0.0, -1.0, 1.0

		);
		glm::mat4x4 POffset2Matrix = glm::mat4
		(
			1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			double(2.0 * cos(A1Rotate) + 1.8 * sin(A1Rotate + A2Rotate)), double(2.1 + 2.0 * sin(A1Rotate) - 1.8 * cos(A1Rotate + A2Rotate)), 0.0, 1.0

		);
		glm::mat4x4 PVRotationMatrix = glm::mat4
		(
			1.0, 0.0, 0.0, 0.0,
			0.0, double(cos(PVRotate)), double(sin(PVRotate)), 0.0,
			0.0, double(-1 * sin(PVRotate)), double(cos(PVRotate)), 0.0,
			0.0, 0.0, 0.0, 1.0
		);
		glm::mat4x4 PTwistMatrix = glm::mat4
		(
			double(cos(PTWIST)), double(sin(PTWIST)), 0.0, 0.0,
			double(sin(PTWIST)), double(cos(PTWIST)), 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			0.0, 0.0, 0.0, 1.0
		);
		glm::mat4x4 PHRotationMatrix = glm::mat4
		(
			double(cos(PHRotate)), 0.0, double(-1 * sin(PHRotate)), 0.0,
			0.0, 1.0, 0.0, 0.0,
			double(sin(PHRotate)), 0.0, double(cos(PHRotate)), 0.0,
			0.0, 0.0, 0.0, 1.0
		);

		ModelMatrix = TranslationMatrix * TopRotationMatrix * POffset2Matrix * A1RotationMatrix * A2RotationMatrix * PHRotationMatrix * PVRotationMatrix * PTwistMatrix * POffsetMatrix * ModelMatrix;
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &gViewMatrix[0][0]);
		glUniformMatrix4fv(ProjMatrixID, 1, GL_FALSE, &gProjectionMatrix[0][0]);
		glUniformMatrix4fv(TranslationMatrixID, 1, GL_FALSE, &TranslationMatrix[0][0]);
		glUniformMatrix4fv(TopRotationMatrixID, 1, GL_FALSE, &TopRotationMatrix[0][0]);
		glUniformMatrix4fv(A1RotationMatrixID, 1, GL_FALSE, &A1RotationMatrix[0][0]);
		glUniformMatrix4fv(A2RotationMatrixID, 1, GL_FALSE, &A2RotationMatrix[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		{
			glBindVertexArray(VertexArrayId[13]);	// draw Pen
			glDrawElements(
				GL_TRIANGLES,      // mode
				NumIndices[13],    // count
				GL_UNSIGNED_SHORT,   // type
				(void*)0           // element array buffer offset
			);
		}
		else
		{
			glBindVertexArray(VertexArrayId[7]);	// draw Pen
			glDrawElements(
				GL_TRIANGLES,      // mode
				NumIndices[7],    // count
				GL_UNSIGNED_SHORT,   // type
				(void*)0           // element array buffer offset
			);
		}

		glBindVertexArray(0);

		ModelMatrix = glm::mat4(1.0);
		TranslationMatrix = glm::mat4
		(
			1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			shift[0], shift[1], shift[2], 1.0
		);
		glm::mat4x4 ButtonOffsetMatrix = glm::mat4
		(
			1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			0.0, 0.1, -0.5, 1.0

		);

		ModelMatrix = TranslationMatrix * TopRotationMatrix * POffset2Matrix * A1RotationMatrix * A2RotationMatrix * PHRotationMatrix * PVRotationMatrix * PTwistMatrix * ButtonOffsetMatrix * ModelMatrix;
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &gViewMatrix[0][0]);
		glUniformMatrix4fv(ProjMatrixID, 1, GL_FALSE, &gProjectionMatrix[0][0]);
		glUniformMatrix4fv(TranslationMatrixID, 1, GL_FALSE, &TranslationMatrix[0][0]);
		glUniformMatrix4fv(TopRotationMatrixID, 1, GL_FALSE, &TopRotationMatrix[0][0]);
		glUniformMatrix4fv(A1RotationMatrixID, 1, GL_FALSE, &A1RotationMatrix[0][0]);
		glUniformMatrix4fv(A2RotationMatrixID, 1, GL_FALSE, &A2RotationMatrix[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		glBindVertexArray(VertexArrayId[8]);	// draw Button
		glDrawElements(
			GL_TRIANGLES,      // mode
			NumIndices[8],    // count
			GL_UNSIGNED_SHORT,   // type
			(void*)0           // element array buffer offset
		);

		glBindVertexArray(0);

		ModelMatrix = glm::mat4(1.0);
		TranslationMatrix = glm::mat4
		(
			1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			double(shift[0]), double(shift[1]), double(shift[2]), 1.0
		);
		glm::mat4x4 BallOffsetMatrix = glm::mat4
		(
			1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			0.0, 0.0, double(-2.0 - BallIniSpeed * time), 1.0

		);
		glm::mat4x4 BallInitOffsetMatrix = glm::mat4
		(
			1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			0.0, 0.0, -2.0, 1.0

		);
		glm::mat4x4 BallOffset2Matrix = glm::mat4
		(
			1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			double(2.0 * cos(A1Rotate) + 1.8 * sin(A1Rotate + A2Rotate)), double(2.1 + 2.0 * sin(A1Rotate) - 1.8 * cos(A1Rotate + A2Rotate) - 1 * 0.5 * gravity * time * time), 0.0, 1.0

		);
		glm::mat4x4 BallInitOffset2Matrix = glm::mat4
		(
			1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			double(2.0 * cos(A1Rotate) + 1.8 * sin(A1Rotate + A2Rotate)), double(2.1 + 2.0 * sin(A1Rotate) - 1.8 * cos(A1Rotate + A2Rotate)), 0.0, 1.0

		);
		ModelMatrix = TranslationMatrix * TopRotationMatrix * BallOffset2Matrix * A1RotationMatrix * A2RotationMatrix * PHRotationMatrix * PVRotationMatrix * PTwistMatrix * BallOffsetMatrix * ModelMatrix;
		glm::mat4x4 MyModelMatrix = glm::mat4(1.0);
		//MyModelMatrix = TranslationMatrix * TopRotationMatrix * BallInitOffset2Matrix * A1RotationMatrix * A2RotationMatrix * PHRotationMatrix * PVRotationMatrix * PTwistMatrix * MyModelMatrix;
		SpeedVector = TopRotationMatrix * A1RotationMatrix * A2RotationMatrix * PHRotationMatrix * PVRotationMatrix * MyModelMatrix * SpeedVector;
		BallPosi = MyModelMatrix * BallPosi;
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &gViewMatrix[0][0]);
		glUniformMatrix4fv(ProjMatrixID, 1, GL_FALSE, &gProjectionMatrix[0][0]);
		glUniformMatrix4fv(TranslationMatrixID, 1, GL_FALSE, &TranslationMatrix[0][0]);
		glUniformMatrix4fv(TopRotationMatrixID, 1, GL_FALSE, &TopRotationMatrix[0][0]);
		glUniformMatrix4fv(A1RotationMatrixID, 1, GL_FALSE, &A1RotationMatrix[0][0]);
		glUniformMatrix4fv(A2RotationMatrixID, 1, GL_FALSE, &A2RotationMatrix[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		glBindVertexArray(VertexArrayId[14]);	// draw Ball
		glDrawElements(
			GL_TRIANGLES,      // mode
			NumIndices[14],    // count
			GL_UNSIGNED_SHORT,   // type
			(void*)0           // element array buffer offset
		);

		glBindVertexArray(0);
	}

	glUseProgram(0);
	{
		// Draw GUI
		TwDraw();
	}

	// Swap buffers
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void pickObject(void)
{
	// Clear the screen in white
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(pickingProgramID);
	{
		glm::mat4 ModelMatrix = glm::mat4(1.0); // TranslationMatrix * RotationMatrix;
		glm::mat4 MVP = gProjectionMatrix * gViewMatrix * ModelMatrix;

		// Send our transformation to the currently bound shader, in the "MVP" uniform
		glUniformMatrix4fv(PickingMatrixID, 1, GL_FALSE, &MVP[0][0]);

		// ATTN: DRAW YOUR PICKING SCENE HERE. REMEMBER TO SEND IN A DIFFERENT PICKING COLOR FOR EACH OBJECT BEFOREHAND
		glBindVertexArray(0);

	}
	glUseProgram(0);
	// Wait until all the pending drawing commands are really done.
	// Ultra-mega-over slow ! 
	// There are usually a long time between glDrawElements() and
	// all the fragments completely rasterized.
	glFlush();
	glFinish();

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Read the pixel at the center of the screen.
	// You can also use glfwGetMousePos().
	// Ultra-mega-over slow too, even for 1 pixel, 
	// because the framebuffer is on the GPU.
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	unsigned char data[4];
	glReadPixels(xpos, window_height - ypos, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data); // OpenGL renders with (0,0) on bottom, mouse reports with (0,0) on top

																					 // Convert the color back to an integer ID
	gPickedIndex = int(data[0]);

	if (gPickedIndex == 255) { // Full white, must be the background !
		gMessage = "background";
	}
	else {
		std::ostringstream oss;
		oss << "point " << gPickedIndex;
		gMessage = oss.str();
	}

	// Uncomment these lines to see the picking shader in effect
	//glfwSwapBuffers(window);
	//continue; // skips the normal rendering
}

int initWindow(void)
{
	// Initialise GLFW
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(window_width, window_height, "Zhitao,Liu(90838017)", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Initialize the GUI
	TwInit(TW_OPENGL_CORE, NULL);
	TwWindowSize(window_width, window_height);
	TwBar * GUI = TwNewBar("Picking");
	TwSetParam(GUI, NULL, "refresh", TW_PARAM_CSTRING, 1, "0.1");
	TwAddVarRW(GUI, "Last picked object", TW_TYPE_STDSTRING, &gMessage, NULL);

	// Set up inputs
	glfwSetCursorPos(window, window_width / 2, window_height / 2);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetMouseButtonCallback(window, mouseCallback);

	return 0;
}


void initOpenGL(void)
{

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	gProjectionMatrix = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	// Or, for an ortho camera :
	//gProjectionMatrix = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, 0.0f, 100.0f); // In world coordinates

	// Camera matrix
	gViewMatrix = glm::lookAt(
		glm::vec3(6.0, 6.0, 6.0),	// eye
		glm::vec3(0.0, 0.0, 0.0),	// center
		glm::vec3(0.0, 1.0, 0.0));	// up

									// Create and compile our GLSL program from the shaders

	lightPos.x = 5.0;
	lightPos.y = 6.0;
	lightPos.z = 6.0;

	lightPos2.x = 7.0;
	lightPos2.y = 6.0;
	lightPos2.z = 6.0;

	programID = LoadShaders("StandardShading.vertexshader", "StandardShading.fragmentshader");
	pickingProgramID = LoadShaders("Picking.vertexshader", "Picking.fragmentshader");

	// Get a handle for our "MVP" uniform
	MatrixID = glGetUniformLocation(programID, "MVP");
	ModelMatrixID = glGetUniformLocation(programID, "M");
	ViewMatrixID = glGetUniformLocation(programID, "V");
	ProjMatrixID = glGetUniformLocation(programID, "P");
	TranslationMatrixID = glGetUniformLocation(programID, "TRA");
	TopRotationMatrixID = glGetUniformLocation(programID, "TRO");
	A1RotationMatrixID = glGetUniformLocation(programID, "A1RO");
	A2RotationMatrixID = glGetUniformLocation(programID, "A2RO");

	PickingMatrixID = glGetUniformLocation(pickingProgramID, "MVP");
	// Get a handle for our "pickingColorID" uniform
	pickingColorID = glGetUniformLocation(pickingProgramID, "PickingColor");
	// Get a handle for our "LightPosition" uniform
	LightID = glGetUniformLocation(programID, "LightPosition_worldspace");
	// Get a handle for our "LightPosition2" uniform
	LightID2 = glGetUniformLocation(programID, "LightPosition2_worldspace");

	createObjects();
}

void createVAOs(Vertex Vertices[], unsigned short Indices[], int ObjectId) {

	GLenum ErrorCheckValue = glGetError();
	const size_t VertexSize = sizeof(Vertices[0]);
	const size_t RgbOffset = sizeof(Vertices[0].Position);
	const size_t Normaloffset = sizeof(Vertices[0].Color) + RgbOffset;

	// Create Vertex Array Object
	glGenVertexArrays(1, &VertexArrayId[ObjectId]);	//
	glBindVertexArray(VertexArrayId[ObjectId]);		//

													// Create Buffer for vertex data
	glGenBuffers(1, &VertexBufferId[ObjectId]);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferId[ObjectId]);
	glBufferData(GL_ARRAY_BUFFER, VertexBufferSize[ObjectId], Vertices, GL_STATIC_DRAW);

	// Create Buffer for indices
	if (Indices != NULL) {
		glGenBuffers(1, &IndexBufferId[ObjectId]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferId[ObjectId]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexBufferSize[ObjectId], Indices, GL_STATIC_DRAW);
	}

	// Assign vertex attributes
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, VertexSize, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, VertexSize, (GLvoid*)RgbOffset);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, VertexSize, (GLvoid*)Normaloffset);

	glEnableVertexAttribArray(0);	// position
	glEnableVertexAttribArray(1);	// color
	glEnableVertexAttribArray(2);	// normal

									// Disable our Vertex Buffer Object 
	glBindVertexArray(0);

	ErrorCheckValue = glGetError();
	if (ErrorCheckValue != GL_NO_ERROR)
	{
		fprintf(
			stderr,
			"ERROR: Could not create a VBO: %s \n",
			gluErrorString(ErrorCheckValue)
		);
	}
}

void cleanup(void)
{
	// Cleanup VBO and shader
	for (int i = 0; i < NumObjects; i++) {
		glDeleteBuffers(1, &VertexBufferId[i]);
		glDeleteBuffers(1, &IndexBufferId[i]);
		glDeleteVertexArrays(1, &VertexArrayId[i]);
	}
	glDeleteProgram(programID);
	glDeleteProgram(pickingProgramID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// ATTN: MODIFY AS APPROPRIATE
	if (action == GLFW_PRESS) {
		switch (key)
		{
		case GLFW_KEY_A:
			break;
		case GLFW_KEY_D:
			break;
		case GLFW_KEY_W:
			break;
		case GLFW_KEY_S:
			break;
		case GLFW_KEY_SPACE:
			break;
		case GLFW_KEY_C:
			printf("Key C pressed!\n");
			break;
		default:
			break;
		}
	}
}

static void mouseCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		pickObject();
	}
}

int main(void)
{
	// initialize window
	int errorCode = initWindow();
	if (errorCode != 0)
		return errorCode;

	// initialize OpenGL pipeline
	initOpenGL();

	// For speed computation
	double lastTime = glfwGetTime();
	int nbFrames = 0;
	do {
		//// Measure speed
		double currentTime = glfwGetTime();
		nbFrames++;
		if (currentTime - lastTime >= 1.0) { // If last prinf() was more than 1sec ago
											 // printf and reset
			printf("%f ms/frame\n", 1000.0 / double(nbFrames));
			nbFrames = 0;
			lastTime += 1.0;
		}

		// rotate camera
		if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		{
			double camx = 6.0;
			double camy = 6.0;
			double camz = 6.0;

			if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			{
				phi2 += 0.2;
				if (phi2 > 360)
				{
					phi2 -= 360;
					printf("up is %f\n", up);
				}
				printf("phi2 is %f!\n", phi2);
			}

			if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			{
				phi2 -= 0.2;
				if (phi2 < -360)
				{
					phi2 += 360;
					printf("up is %f\n", up);
				}
				printf("phi2 is %f!\n", phi2);
			}

			if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			{
				phi1 -= 0.2;
				if (phi1 < -360)
				{
					phi1 += 360;
				}
				printf("phi1 is %f!\n", phi1);
			}
			if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			{
				phi1 += 0.2;
				if (phi1 > 360)
				{
					phi1 -= 360;
				}
				printf("phi1 is %f!\n", phi1);
			}

			double radian1 = phi1 / 360 * (2 * 3.14159265358979323846264338327950288);
			double radian2 = phi2 / 360 * (2 * 3.14159265358979323846264338327950288);
			camx = double(6 * sqrt(3) * cos(radian2) * sin(radian1));
			camy = double(6 * sqrt(3) * sin(radian2));
			camz = double(6 * sqrt(3) * cos(radian2) * cos(radian1));

			lightPos.x = camx - 1.0;
			lightPos2.x = camx + 1.0;
			lightPos.y = camy;
			lightPos2.y = camy;
			lightPos.z = camz;
			lightPos2.z = camz;

			if (abs(abs(phi2) - 90) <= 0.100 || (abs(abs(phi2) - 270) <= 0.100))
			{
				up = up * (-1);
				printf("change up!\n");
			}

			gViewMatrix = glm::lookAt(
				glm::vec3(camx, camy, camz),	// eye
				glm::vec3(0.0, 0.0, 0.0),	// center
				glm::vec3(0.0, up, 0.0));	// up



		}

		// Base actions
		if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		{
			if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			{
				shift[2] += -0.01f;
			}

			else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			{
				shift[2] += 0.01f;
			}

			else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			{
				shift[0] += -0.01f;
			}

			else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			{
				shift[0] += 0.01f;
			}
		}

		// Top actions
		if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		{
			if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			{
				TopRotate += 0.01f;
			}

			else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			{
				TopRotate += -0.01f;
			}
		}

		// Arm1 actions
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		{
			if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			{
				A1Rotate += 0.01f;
				if (A1Rotate >= 1.2f)
				{
					A1Rotate = 1.2f;
				}
				printf("Arm1 Rotation Angle: %f\n", A1Rotate);
			}

			else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			{
				A1Rotate += -0.01f;
				if (A1Rotate <= -0.0f)
				{
					A1Rotate = -0.0f;
				}
				printf("Arm1 Rotation Angle: %f\n", A1Rotate);
			}
		}

		// Arm2 actions
		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		{
			if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			{
				A2Rotate += 0.01f;
				printf("Arm2 Rotation Angle: %f\n", A2Rotate);
			}

			else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			{
				A2Rotate += -0.01f;
				printf("Arm2 Rotation Angle: %f\n", A2Rotate);
			}
		}

		// Pen actions
		if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		{
			if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
			{
				if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
				{
					PTWIST += 0.01f;
					printf("PEN Twist Angle: %f\n", PTWIST);
				}

				else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
				{
					PTWIST += -0.01f;
					printf("PEN Twist Rotation Angle: %f\n", PTWIST);
				}
			}

			else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			{
				PVRotate += 0.01f;
				printf("PEN Vertical Rotation Angle: %f\n", PVRotate);
			}

			else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			{
				PVRotate += -0.01f;
				printf("PEN Vertical Rotation Angle: %f\n", PVRotate);
			}

			else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			{
				PHRotate += 0.01f;
				printf("PEN Horizontal Rotation Angle: %f\n", PHRotate);
			}

			else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			{
				PHRotate += -0.01f;
				printf("PEN Horizontal Rotation Angle: %f\n", PHRotate);
			}
		}

		// Teleporting
		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		{
			time += 0.005f;
		}

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			//printf("BallPosition: X:%f Y:%f Z:%f\n", BVerts[0].Position[0], BVerts[0].Position[1], BVerts[0].Position[2]);
			printf("BallPosition: X:%f Y:%f Z:%f\n", BallPosi[0], BallPosi[1], BallPosi[2]);
			printf("Initial Speed: X:%f Y:%f Z:%f\n", SpeedVector[0], SpeedVector[1], SpeedVector[2]);
		}


		/*if (animation) {
		phi += 0.01;
		if (phi > 360)
		phi -= 360;
		}*/

		// DRAWING POINTS
		renderScene();


	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	cleanup();

	return 0;
}