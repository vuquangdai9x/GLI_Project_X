//TrainingFramework.cpp : Defines the entry point for the console application.
#include "stdafx.h"

#include "../Utilities/utilities.h"
#include "../Framework3D/TrainingFramework/framework3d.h"
#include "Box2D/Box2D.h"
#include <conio.h>
#include <stddef.h>
#include "Singleton.h"
#include "State/GameStateManager.h"

// detect memory leak
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#define MOUSE_CLICK     1
#define MOUSE_RELEASE   2
#define MOUSE_MOVE    3

int Init(ESContext* esContext)
{
	Singleton<InputManager>::CreateInstance();
	Singleton<ResourceManager2D>::CreateInstance();
	Singleton<SceneManager2D>::CreateInstance();

	Singleton<GameStateManager>::CreateInstance();

	glClearColor(0.95f, 0.95f, 0.95f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return 0;
}

void Draw(ESContext* esContext)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	Singleton<GameStateManager>::GetInstance()->render();

	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
}

void Update(ESContext * esContext, float deltaTime)
{
	Singleton<GameStateManager>::GetInstance()->update(deltaTime);
}

void Key(ESContext * esContext, unsigned char key, bool bIsPressed)
{
	Singleton<InputManager>::GetInstance()->KeyPressed(key, bIsPressed);
	Singleton<GameStateManager>::GetInstance()->KeyPress();
}
void Mouse(ESContext* esContext, int typeOfService, int button, int x, int y) {
	float xx = x, yy = y;
	printf("2D pos: %f %f \n", xx, yy);
	Vector3 pos3D;
	pos3D = Singleton<SceneManager2D>::GetInstance()->get3Dpos(xx, yy);
	printf("3D pos: %f %f \n", pos3D.x, pos3D.y);
}
void CleanUp()
{

}

int _tmain(int argc, _TCHAR * argv[])
{
	ESContext esContext;

	esInitContext(&esContext);

	esCreateWindow(&esContext, "Hello Triangle", Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

	int iInitResult;
	if ((iInitResult = Init(&esContext)) != 0) {
		printf("Oop! Error happen\n");
	}
	else {
		esRegisterDrawFunc(&esContext, Draw);
		esRegisterUpdateFunc(&esContext, Update);

		esRegisterKeyFunc(&esContext, Key);
		esRegisterMouseFunc(&esContext, Mouse);
		esMainLoop(&esContext);
	}

	//releasing OpenGL resources
	CleanUp();

	Singleton<ResourceManager>::DestroyInstance();
	Singleton<SceneManager>::DestroyInstance();
	Singleton<InputManager>::DestroyInstance();

	//identifying memory leaks
	MemoryDump();
	printf("Press any key...\n");
	_getch();

	return 0;
}

