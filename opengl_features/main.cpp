
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include <vector>
#include <iostream>

#include <shaders.h>
#include <meshes.h>

typedef glm::mat3 mat3;
typedef glm::mat4 mat4;
typedef glm::vec3 vec3;
typedef glm::vec4 vec4;

vec3 eye = vec3(0.0f, 2.f, 8.f);
vec3 up = vec3(0.f, 0.5f, -1.f);
mat4 projection, view;
GLuint projection_loc, view_loc;
GLuint s_projection_loc, s_view_loc;

GLuint shader_program, stencil_shader_program;

std::vector<GLuint> cubeVAOs;
std::vector<mat4> cube_models;
GLuint floorVAO;
mat4 floor_model;

GLuint depth_mode_loc;
bool depth_mode = false;

void init();
void changeViewport(int w, int h);
void keyboard(unsigned char key, int x, int y);
void init_shaders();
void init_stencil_shaders();
void init_stencil_shaders();
void init_camera();
void init_cubes();
void init_floor();
void draw_camera();
void draw_cubes();
void draw_scaled_cubes(float scale);
void draw_floor();
void render();

int main(int argc, char* argv[])
{
	// Initialize GLUT
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	// Create window
	glutCreateWindow("OPENGL APP");

	glEnable(GL_DEPTH_TEST); // Enable depth test
	glDepthFunc(GL_LESS);

	glEnable(GL_STENCIL_TEST); // Enable stencil test
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glutInitWindowSize(1200, 800);
	glutReshapeWindow(1200, 800);

	// Bind functions
	glutReshapeFunc(changeViewport);
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(render);

	// Initialize Glew
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << "Unable to initalize Glew ! " << std::endl;
		return 1;
	}

	// Start program
	init();

	glutMainLoop();

	return 0;
}

void changeViewport(int w, int h)
{
	glViewport(0, 0, w, h);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'k':
			depth_mode = !depth_mode;
			break;
		default:
			break;
	}
	glutPostRedisplay();
}

void init()
{
	std::cout << "Press 'k' to see depth values with color of the fragments.";
	std::cout << std::endl;
	init_shaders();
	init_stencil_shaders();
	init_camera();
	init_floor();
	init_cubes();
}

void init_shaders()
{
	// Initialize shaders
	GLuint vertex_shader = initshaders(GL_VERTEX_SHADER, "shaders/vertex_shader.glsl");
	GLuint fragment_shader = initshaders(GL_FRAGMENT_SHADER, "shaders/fragment_shader.glsl");
	shader_program = initprogram(vertex_shader, fragment_shader);

	depth_mode_loc = glGetUniformLocation(shader_program, "depth_mode");
}

void init_stencil_shaders()
{
	// Initialize shaders
	GLuint vertex_shader = initshaders(GL_VERTEX_SHADER, "shaders/stencil_vs.glsl");
	GLuint fragment_shader = initshaders(GL_FRAGMENT_SHADER, "shaders/stencil_fs.glsl");
	stencil_shader_program = initprogram(vertex_shader, fragment_shader);
}

void init_camera()
{
	// Initialize viewing values
	projection = glm::perspective(glm::radians(60.0f), (float)1200 / 800, 0.1f, 100.0f);
	view = glm::lookAt(eye, vec3(0.0f, 0.0f, 0.0f), up);

	projection_loc = glGetUniformLocation(shader_program, "projection");
	view_loc = glGetUniformLocation(shader_program, "view");

	s_projection_loc = glGetUniformLocation(stencil_shader_program, "projection");
	s_view_loc = glGetUniformLocation(stencil_shader_program, "view");
}

void init_floor()
{
	glGenVertexArrays(1, &floorVAO);

	glBindVertexArray(floorVAO);

	// Vertices buffer
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*4*3, floor_vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Colors buffer
	GLuint CBO;
	glGenBuffers(1, &CBO);
	glBindBuffer(GL_ARRAY_BUFFER, CBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*4*4, floor_colors, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

	// Element buffer
	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLshort)*6, floor_elements, GL_STATIC_DRAW);

	// Model matrix of the floor
	floor_model = mat4(1.0);

	// Unbind for prevent further modification
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void init_cubes()
{
	float posx = -1.5f; float posz = -1.f;

	for (int i = 0; i < 2; i++)
	{
		GLuint cubeVAO;
		glGenVertexArrays(1, &cubeVAO);
		glBindVertexArray(cubeVAO);

		// Vertices buffer
		GLuint VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8 * 3, cube_vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// Colors buffer
		GLuint CBO;
		glGenBuffers(1, &CBO);
		glBindBuffer(GL_ARRAY_BUFFER, CBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8 * 4, cube_colors, GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

		// Element buffer
		GLuint EBO;
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLshort) * 36, cube_elements, GL_STATIC_DRAW);

		// Model matrix of the floor
		mat4 cube_model = glm::translate(mat4(1.0f), vec3(posx + i * 3, 0.0f, posz + i * 3));

		// Store model matrices and VAO of cube
		cube_models.push_back(cube_model);
		cubeVAOs.push_back(cubeVAO);

		// Unbind for prevent further modification
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}

void draw_camera()
{
	// Send camera uniforms
	glUniformMatrix4fv(view_loc, 1, GL_FALSE, &(view)[0][0]);
	glUniformMatrix4fv(projection_loc, 1, GL_FALSE, &(projection)[0][0]);
}

void draw_stencil_camera()
{
	// Send camera uniforms
	glUniformMatrix4fv(s_view_loc, 1, GL_FALSE, &(view)[0][0]);
	glUniformMatrix4fv(s_projection_loc, 1, GL_FALSE, &(projection)[0][0]);
}

void draw_floor()
{
	GLuint model_loc = glGetUniformLocation(shader_program, "model");
	glUniformMatrix4fv(model_loc, 1, GL_FALSE, &floor_model[0][0]);

	glBindVertexArray(floorVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
	glBindVertexArray(0);
}

void draw_cubes()
{
	for (int i = 0; i < 2; i++)
	{
		GLuint model_loc = glGetUniformLocation(shader_program, "model");
		glUniformMatrix4fv(model_loc, 1, GL_FALSE, &(cube_models[i])[0][0]);

		glBindVertexArray(cubeVAOs[i]);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, nullptr);
		glBindVertexArray(0);
	}

}

void draw_scaled_cubes(float scale)
{
	for (int i = 0; i < 2; i++)
	{
		GLuint model_loc = glGetUniformLocation(stencil_shader_program, "model");
		mat4 temp_model = glm::scale(cube_models[i], vec3(scale, scale, scale));
		glUniformMatrix4fv(model_loc, 1, GL_FALSE, &temp_model[0][0]);

		glBindVertexArray(cubeVAOs[i]);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, nullptr);
		glBindVertexArray(0);
	}
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClearColor(0.2f, 0.2f, 0.2f, 1.f);

	glStencilMask(0x00); // make sure we don't update the stencil buffer while drawing the floor
	float scale = 1.05f;

	glUseProgram(shader_program);
	glUniform1i(depth_mode_loc, (int)depth_mode);
	draw_floor();
	
	// 1st. render pass, draw objects as normal, writing to the stencil buffer
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF); // Enable writing to the stencil buffer

	draw_camera();
	draw_cubes();

	// 2nd. render pass: now draw slightly scaled versions of the objects, this time disabling stencil writing.
	// Because the stencil buffer is now filled with several 1s. The parts of the buffer that are 1 are not drawn, thus only drawing 
	// the objects' size differences, making it look like borders.
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00); // Disable writing to the stencil buffer
	glDisable(GL_DEPTH_TEST);
	
	glUseProgram(stencil_shader_program);

	draw_stencil_camera();
	draw_scaled_cubes(scale); // Scale the objects for highlight

	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glEnable(GL_DEPTH_TEST); // Enable depth test again for the next frame

	glutSwapBuffers();
}