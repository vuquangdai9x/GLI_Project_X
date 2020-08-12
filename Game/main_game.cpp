//TrainingFramework.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG
// ---
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
	glClearColor(1.0f, 0.8f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return 0;
}

void Draw(ESContext* esContext)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	Singleton<SceneManager2D>::GetInstance()->Render();
	//SceneManager::GetInstance()->Render();
	//Singleton<GameStateManager>::GetInstance()->render();
	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
}

void Update(ESContext * esContext, float deltaTime)
{
	Singleton<InputManager>::GetInstance()->Update(deltaTime);
	//Singleton<SceneManager2D>::GetInstance()->Update(deltaTime);
	Singleton<GameStateManager>::GetInstance()->update(deltaTime);
	//SceneManager::GetInstance()->Update(deltaTime);
}

void Key(ESContext * esContext, unsigned char key, bool bIsPressed)
{
	//InputManager::GetInstance()->KeyPressed(key, bIsPressed);
	Singleton<InputManager>::GetInstance()->KeyPressed(key, bIsPressed);
	Singleton<GameStateManager>::GetInstance()->KeyPress();
}
void Mouse(ESContext* esContext, int typeOfService, int button, int x, int y) {
	/*float xx = x, yy = y;
	printf("2D pos: %f %f \n", xx, yy);
	Vector3 pos3D;
	pos3D = Singleton<SceneManager2D>::GetInstance()->get3Dpos(xx, yy);
	printf("3D pos: %f %f \n", pos3D.x, pos3D.y);*/
}
void CleanUp()
{

}

int _tmain(int argc, _TCHAR * argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

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

	_CrtDumpMemoryLeaks();

	B2_NOT_USED(argc);
	B2_NOT_USED(argv);

	// Define the gravity vector.
	b2Vec2 gravity(0.0f, -10.0f);

	// Construct a world object, which will hold and simulate the rigid bodies.
	b2World world(gravity);

	// Define the ground body.
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -10.0f);

	// Call the body factory which allocates memory for the ground body
	// from a pool and creates the ground box shape (also from a pool).
	// The body is also added to the world.
	b2Body * groundBody = world.CreateBody(&groundBodyDef);

	// Define the ground box shape.
	b2PolygonShape groundBox;

	// The extents are the half-widths of the box.
	groundBox.SetAsBox(50.0f, 10.0f);

	// Add the ground fixture to the ground body.
	groundBody->CreateFixture(&groundBox, 0.0f);

	// Define the dynamic body. We set its position and call the body factory.
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0.0f, 4.0f);
	b2Body * body = world.CreateBody(&bodyDef);

	// Define another box shape for our dynamic body.
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(1.0f, 1.0f);

	// Define the dynamic body fixture.
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;

	// Set the box density to be non-zero, so it will be dynamic.
	fixtureDef.density = 1.0f;

	// Override the default friction.
	fixtureDef.friction = 0.3f;

	// Add the shape to the body.
	body->CreateFixture(&fixtureDef);

	// Prepare for simulation. Typically we use a time step of 1/60 of a
	// second (60Hz) and 10 iterations. This provides a high quality simulation
	// in most game scenarios.
	float32 timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	// This is our little game loop.
	for (int32 i = 0; i < 60; ++i)
	{
		// Instruct the world to perform a single step of simulation.
		// It is generally best to keep the time step and iterations fixed.
		world.Step(timeStep, velocityIterations, positionIterations);

		// Now print the position and angle of the body.
		b2Vec2 position = body->GetPosition();
		float32 angle = body->GetAngle();

		printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
	}

	// When the world destructor is called, all bodies and joints are freed. This can
	// create orphaned pointers, so be careful about your world management.

	return 0;
}

