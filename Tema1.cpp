#include "Tema1.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include "Transform2D.h"
#include "Object2D.h"

using namespace std;

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

float RandomFloat(float min, float max)
{
	assert(max > min);
	float random = ((float)rand()) / (float)RAND_MAX;
	float range = max - min;

	return min + (random * range);
}

float clamp(float v, float lo, float hi)
{
	assert(!(hi < lo));
	return (v < lo) ? lo : (hi < v) ? hi : v;
}

int detectCollision(float rectangleX, float rectangleY, float width, float height, float circleX, float circleY, float radius) {

	float closestX = clamp(circleX, rectangleX, rectangleX + width);
	float closestY = clamp(circleY, rectangleY, rectangleY + height);

	// Calculate the distance between the circle's center and this closest point
	float distanceX = circleX - closestX;
	float distanceY = circleY - closestY;

	// If the distance is less than the circle's radius, an intersection occurs
	float distance = (distanceX * distanceX) + (distanceY * distanceY);
	
	if (distance < (radius * radius))
		return 1;
	
	return 0;
}


void Tema1::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	top = resolution.y - topLine;

	//init left bottom corner at origin
	glm::vec3 corner = glm::vec3(0, 0, 0);

	// initialize tx and ty (the translation steps)
	translateX = 0;
	translateY = 0;
	rotation = 0;

	colorPipe = glm::vec3(0.02, 0.75, 0.2);
	colorBord = glm::vec3(0.13, 0.63, 0.13);
	colorTop = glm::vec3(0.3, 0.5, 0.5);

	srand(time(NULL));

	//init bird
	{
		
		position = 0;
		bodyPositionX = 435;
		bodyPositionY = 385;
		wingPosition = bodyPositionY - 5;
		beakPosition = bodyPositionY - 13;
		eyePosition = bodyPositionY + 10;
		radius = 30;


		Mesh* circle1 = Object2D::CreateCircle("circle1", corner, radius, glm::vec3(0.3, 0.5, 1)); //body
		AddMeshToList(circle1);

		Mesh* circle2 = Object2D::CreateCircle("circle2", corner, 3, glm::vec3(0.1, 0.1, 0.1)); //eye
		AddMeshToList(circle2);

		Mesh* triangle1 = Object2D::CreateTriangle("triangle1", corner, 25, glm::vec3(0.95, 1, 0.35)); //beak
		AddMeshToList(triangle1);

		Mesh* triangle2 = Object2D::CreateTriangle("triangle2", corner, 20, glm::vec3(0.15, 0.2, 0.50)); //wing 
		AddMeshToList(triangle2);
	}


	//init pipes
	{
		offset = 1300;
		distanceX = 350;
		distanceY = 155;

		for (int i = 0; i < 4 * 2; i++)
		{
			Mesh* pipe = Object2D::CreateRectangle("pipe", width, height, colorPipe);
			pipes.push_back(pipe);
		}

		positionPipe.push_back(std::make_pair(offset, RandomFloat(380, 620)));
		positionPipe.push_back(std::make_pair(offset + distanceX, RandomFloat(300, 600)));
		positionPipe.push_back(std::make_pair(offset + 2 * distanceX, RandomFloat(310, 630)));
		positionPipe.push_back(std::make_pair(offset + 3 * distanceX, RandomFloat(305, 650)));

		translateX = positionPipe[0].first;

		Mesh* border = Object2D::CreateRectangle("border", width, borderH, colorBord);
		AddMeshToList(border);

	}

	Mesh* topBorder = Object2D::CreateRectangle("topBorder", resolution.x, topLine, colorTop);
	AddMeshToList(topBorder);
}

void Tema1::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	//glClearColor(0.2, 0.65, 0.7, 1);
	glClearColor(0.55, 0.7, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(0, resolution.y - topLine);
	RenderMesh2D(meshes["topBorder"], shaders["VertexColor"], modelMatrix);
}

void Tema1::Update(float deltaTimeSeconds)
{

	if (!endGame) {
		
		//waiting for game to start
		if (spaceKey == 0) 
		{
			if (translateY <= 0)
				fly = true;

			if (translateY > 30) //bird start to fall
				fly = false;

			if (fly) //rise
				translateY += 30 * deltaTimeSeconds;
			else // fall
				translateY -= 22 * deltaTimeSeconds;
		
			modelMatrix = glm::mat3(1);
			modelMatrix *= Transform2D::Translate(418, translateY + wingPosition);
			if (fly)
			{
				modelMatrix *= Transform2D::Rotate(600);
				modelMatrix *= Transform2D::Translate(-12, -10);
			}
			RenderMesh2D(meshes["triangle2"], shaders["VertexColor"], modelMatrix); //wing

			modelMatrix = glm::mat3(1);
			modelMatrix *= Transform2D::Translate(450, translateY + eyePosition);
			RenderMesh2D(meshes["circle2"], shaders["VertexColor"], modelMatrix); //ochi

			modelMatrix = glm::mat3(1);
			modelMatrix *= Transform2D::Translate(bodyPositionX, translateY + bodyPositionY);
			RenderMesh2D(meshes["circle1"], shaders["VertexColor"], modelMatrix); //corp

			modelMatrix = glm::mat3(1);
			modelMatrix *= Transform2D::Translate(458, translateY + beakPosition);
			RenderMesh2D(meshes["triangle1"], shaders["VertexColor"], modelMatrix); //cioc
		}

		//game started
		if (spaceKey == 1) {

			//BIRD
			{
				if (jump) {
					if (position + bodyPositionY + radius < top) {
						upVelocity += acceleration * deltaTimeSeconds * 70;
						position += upVelocity * deltaTimeSeconds;
					}
					else jump = false;

					if (upVelocity <= 0)
						jump = false;
				}
				else {
					if (position + bodyPositionY + radius >= 0) {
						downVelocity += acceleration * deltaTimeSeconds * 70;
						position += downVelocity * deltaTimeSeconds;
					}
					else {
						endGame = true;
					}
				}

				modelMatrix = glm::mat3(1);
				modelMatrix *= Transform2D::Translate(418, position + wingPosition);
				if (jump)
				{
					modelMatrix *= Transform2D::Rotate(600);
					modelMatrix *= Transform2D::Translate(-12, -10);

				}
				RenderMesh2D(meshes["triangle2"], shaders["VertexColor"], modelMatrix); //wing

				modelMatrix = glm::mat3(1);
				modelMatrix *= Transform2D::Translate(450, position + eyePosition);
				RenderMesh2D(meshes["circle2"], shaders["VertexColor"], modelMatrix); //ochi

				modelMatrix = glm::mat3(1);
				modelMatrix *= Transform2D::Translate(bodyPositionX, position + bodyPositionY);
				RenderMesh2D(meshes["circle1"], shaders["VertexColor"], modelMatrix); //corp

				modelMatrix = glm::mat3(1);
				modelMatrix *= Transform2D::Translate(458, position + beakPosition);
				if (jump) {
					modelMatrix *= Transform2D::Rotate(0.7);
					modelMatrix *= Transform2D::Translate(7, 1);
				}
				RenderMesh2D(meshes["triangle1"], shaders["VertexColor"], modelMatrix); //cioc
			}
			
			//PIPE
			{
				if (translateX > -width) {
					move = true;
				}
				else
				{
					int runway = positionPipe[0].first + width;

					positionPipe[0].first = positionPipe[1].first - runway;
					positionPipe[0].second = positionPipe[1].second;

					positionPipe[1].first = positionPipe[2].first - runway;
					positionPipe[1].second = positionPipe[2].second;

					positionPipe[2].first = positionPipe[3].first - runway;
					positionPipe[2].second = positionPipe[3].second;

					positionPipe[3].first = positionPipe[2].first + distanceX;
					positionPipe[3].second = RandomFloat(330, 615);

					translateX = positionPipe[0].first;
				}

				if (move) {
					translateX -= (205 * deltaTimeSeconds);

					for (int i = 0; i < 4; i++) {
						//up pipes
						modelMatrix = glm::mat3(1);
						modelMatrix *= Transform2D::Translate(translateX + i * distanceX, positionPipe[i].second);
						RenderMesh2D(meshes["border"], shaders["VertexColor"], modelMatrix);

						modelMatrix = glm::mat3(1);
						modelMatrix *= Transform2D::Translate(translateX + i * distanceX, positionPipe[i].second);
						RenderMesh2D(pipes[i], shaders["VertexColor"], modelMatrix);
						

						collision = detectCollision(translateX + i * distanceX, positionPipe[i].second, width, height,
							bodyPositionX, position + bodyPositionY, radius - 2);

						if (collision == 1) {
							endGame = true;
							break;
						}

						//down pipes
						modelMatrix = glm::mat3(1);
						modelMatrix *= Transform2D::Translate(translateX + i * distanceX, positionPipe[i].second - distanceY - borderH);
						RenderMesh2D(meshes["border"], shaders["VertexColor"], modelMatrix);

						modelMatrix = glm::mat3(1);
						modelMatrix *= Transform2D::Translate(translateX + i * distanceX, positionPipe[i].second - distanceY - height);
						RenderMesh2D(pipes[i], shaders["VertexColor"], modelMatrix);


						collision = detectCollision(translateX + i * distanceX, positionPipe[i].second - distanceY - height, width, height,
							bodyPositionX, position + bodyPositionY, radius - 2);

						if (collision == 1) {
							endGame = true;
							break;
						}

						if (((float)translateX + i * distanceX + width < bodyPositionX) &&
							((float)translateX + i * distanceX + width > bodyPositionX - 180 * deltaTimeSeconds)) {
							score++;
							printf("score: %d\n", score);
						}
					}
				}
			}

		}

	}
	else { //GAME OVER
		if (onlyOnce) {
			printf("final score: %d", score);
			onlyOnce = false;
		}

		if (position + bodyPositionY > -30) {
			position -= 700 * deltaTimeSeconds;
		}

		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(418, position + wingPosition);
		if (jump)
		{
			modelMatrix *= Transform2D::Rotate(600);
			modelMatrix *= Transform2D::Translate(-12, -10);
		}
		RenderMesh2D(meshes["triangle2"], shaders["VertexColor"], modelMatrix); //wing

		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(450, position + eyePosition);
		RenderMesh2D(meshes["circle2"], shaders["VertexColor"], modelMatrix); //ochi

		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(bodyPositionX, position + bodyPositionY);
		RenderMesh2D(meshes["circle1"], shaders["VertexColor"], modelMatrix); //corp

		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(460, position + beakPosition);
		RenderMesh2D(meshes["triangle1"], shaders["VertexColor"], modelMatrix); //cioc

		for (int i = 0; i < 4; i++) {
			modelMatrix = glm::mat3(1);
			modelMatrix *= Transform2D::Translate(translateX + i * distanceX, positionPipe[i].second);
			RenderMesh2D(meshes["border"], shaders["VertexColor"], modelMatrix);

			modelMatrix = glm::mat3(1);
			modelMatrix *= Transform2D::Translate(translateX + i * distanceX, positionPipe[i].second);
			RenderMesh2D(pipes[i], shaders["VertexColor"], modelMatrix);

			modelMatrix = glm::mat3(1);
			modelMatrix *= Transform2D::Translate(translateX + i * distanceX, positionPipe[i].second - distanceY - borderH);
			RenderMesh2D(meshes["border"], shaders["VertexColor"], modelMatrix);

			modelMatrix = glm::mat3(1);
			modelMatrix *= Transform2D::Translate(translateX + i * distanceX, positionPipe[i].second - distanceY - height);
			RenderMesh2D(pipes[i], shaders["VertexColor"], modelMatrix);

		}
	}

}

void Tema1::FrameEnd()
{

}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{

}

void Tema1::OnKeyPress(int key, int mods)
{
	// add key press event
	if (window->KeyHold(GLFW_KEY_SPACE)) {
		spaceKey = 1;
		jump = true;

		downVelocity = 0;
		upVelocity = 350;
		acceleration = gravity;
	}
}

void Tema1::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{
}
