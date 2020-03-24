#include "Object2D.h"

#include <Core/Engine.h>
#include <math.h>

Mesh* Object2D::CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length, 0, 0), color),
		VertexFormat(corner + glm::vec3(length, length, 0), color),
		VertexFormat(corner + glm::vec3(0, length, 0), color)
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };

	if (!fill) {
		square->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	square->InitFromData(vertices, indices);
	return square;
}

Mesh* Object2D::CreateCircle(std::string name, glm::vec3 leftBottomCorner, float radius, glm::vec3 color)
{
	float PI = (float)M_PI;
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
	};

	std::vector<unsigned short> indices =
	{

	};

	int triangleAmount = 100;
	float t, cost, sint;
	for (int i = 0; i < triangleAmount; i++) {
		t = i * 2 * PI / triangleAmount;
		cost = radius * cos(t); 
		sint = radius * sin(t);
		vertices.push_back(VertexFormat(glm::vec3(cost, sint, 0), color));
	}

	for (int i = 0; i < triangleAmount; i++) {
		indices.push_back(i);
	}
	indices.push_back(1);

	Mesh* circle = new Mesh(name);
	circle->InitFromData(vertices, indices);
	circle->SetDrawMode(GL_TRIANGLE_FAN);

	return circle;
}

Mesh* Object2D::CreateTriangle(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length - length / 4, length / 6, 0), color),
		VertexFormat(corner + glm::vec3(0, length, 0), color)
	};

	Mesh* triangle = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2 };
	triangle->InitFromData(vertices, indices);

	return triangle;
}

Mesh* Object2D::CreateRectangle(std::string name, float width, float height, glm::vec3 color)
{
	glm::vec3 corner = glm::vec3(0, 0, 0);

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(width, 0, 0), color),
		VertexFormat(corner + glm::vec3(width, height, 0), color),
		VertexFormat(corner + glm::vec3(0, height, 0), color)
	};

	Mesh* rectangle = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3, 0, 2 };
	rectangle->InitFromData(vertices, indices);
	

	return rectangle;
}