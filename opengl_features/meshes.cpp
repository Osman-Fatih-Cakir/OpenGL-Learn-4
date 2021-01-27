
#include <GL/freeglut.h>
#include <glm.hpp>

typedef glm::mat3 mat3;
typedef glm::mat4 mat4;
typedef glm::vec3 vec3;
typedef glm::vec4 vec4;

// Cube attributes
GLfloat cube_vertices[] = {
	// front
	-1.0f, -1.2f,  1.0f,
	 1.0f, -1.2f,  1.0f,
	 1.0f,  1.2f,  1.0f,
	-1.0f,  1.2f,  1.0f,
	// back	  2
	-1.0f, -1.2f, -1.0f,
	 1.0f, -1.2f, -1.0f,
	 1.0f,  1.2f, -1.0f,
	-1.0f,  1.2f, -1.0f
};
GLfloat cube_colors[] = {
	// front colors
	1.f, 0.f, 0.f, 1.f,
	0.f, 1.f, 0.f, 1.f,
	0.f, 0.f, 1.f, 1.f,
	1.f, 1.f, 0.f, 1.f,
	// back colors
	1.f, 1.f, 1.f, 1.f,
	1.f, 0.f, 1.f, 1.f,
	0.f, 1.f, 1.f, 1.f,
	0.f, 0.f, 1.f, 1.f
};
GLushort cube_elements[] = {
	// front
	0, 1, 2,
	2, 3, 0,
	// right
	1, 5, 6,
	6, 2, 1,
	// back
	7, 6, 5,
	5, 4, 7,
	// left
	4, 0, 3,
	3, 7, 4,
	// bottom
	4, 5, 1,
	1, 0, 4,
	// top
	3, 2, 6,
	6, 7, 3
};

// Floor attributes
GLfloat floor_vertices[] = {
	-5.0f, -1.0f, 5.0f,
	5.0f, -1.0f, 5.0f,
	5.0f, -1.0f, -5.0f,
	-5.0f, -1.0f, -5.0f
};
GLfloat floor_colors[] = {
	0.5f, 0.5f, 0.5f, 1.0f,
	0.5f, 0.5f, 0.5f, 1.0f,
	0.5f, 0.5f, 0.5f, 1.0f,
	0.5f, 0.5f, 0.5f, 1.0f
};
GLushort floor_elements[] = {
	0, 1, 2,
	0, 2, 3
};

