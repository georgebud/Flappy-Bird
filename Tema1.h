#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>
#include <utility>
#include <cstdlib>
#include <ctime>

class Tema1 : public SimpleScene
{
public:
	Tema1();
	~Tema1();

	void Init() override;
	glm::mat3 modelMatrix;

private:
	
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;

protected:
	
	float top;
	float translateX, translateY;
	float scaleX, scaleY;
	float rotation;
	float radius;
	float distanceX, distanceY;
	float width = 120;
	float height = 600;
	float borderH = 12;
	float topLine = 18;
	
	int index, offset;
	int spaceKey = 0;
	int collision = 0;
	int score = 0;

	bool jump = false;
	bool move = false;
	bool fly = false;
	bool endGame = false;
	bool onlyOnce = true;

	float bodyPositionY, wingPosition, beakPosition, eyePosition, bodyPositionX; //bird

	float gravity = -13.8f;
	float position;
	float downVelocity = 0;
	float upVelocity;
	float acceleration = gravity;

	glm::vec3 colorPipe;
	glm::vec3 colorBord;
	glm::vec3 colorTop;

	std::vector<Mesh*> pipes;
	std::vector<std::pair<float, float>> positionPipe;
};
