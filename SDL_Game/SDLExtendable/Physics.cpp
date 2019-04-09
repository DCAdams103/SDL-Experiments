#include <stdio.h>
#include <Box2D/Box2D.h>
#include <string>
#include "Physics.h"


b2Vec2 gravity(0.0f, 0.0f);
b2World world(gravity);

// When key pressed, change to one of these
enum _moveState
{
	MS_STOP,
	MS_LEFT,
	MS_RIGHT,
	MS_UP,
	MS_DOWN,
	MS_UPRIGHT,
	MS_DOWNRIGHT,
	MS_UPLEFT,
	MS_DOWNLEFT,
};

_moveState moveState;

// Constructor
Physics::Physics()
{
	world.SetContinuousPhysics(false);
	posX = 0;
	posY = 0;
	bodyType = "";

	timeStep = 1.0f / 60.0f;
	velocityIterations = 8;
	positionIterations = 3;
}

struct bodyUserData
{
	int entityType;
	float health;
	std::string name;
};

void Physics::makeDynamic()
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0, 10);
	body = world.CreateBody(&bodyDef);

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(100.0f, 100.0f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	fixtureDef.filter.categoryBits = CATEGORY_MONSTER;
	fixtureDef.filter.maskBits = CATEGORY_MONSTER;
	fixtureDef.filter.groupIndex = -2;

	body->CreateFixture(&fixtureDef);
	body->SetUserData(this);

	world.Step(timeStep, velocityIterations, positionIterations);
	pos = body->GetPosition();
	posX = pos.x;
	posY = pos.y;

	bodyType = "dynamic";

}

void Physics::playerDynamic(float x, float y, float width, float height)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(x, y);
	
	body = world.CreateBody(&bodyDef);

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox((200.0f) / 2, (200.0f) / 2);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.filter.categoryBits = CATEGORY_PLAYER;
	fixtureDef.filter.maskBits = CATEGORY_PLAYER;
	fixtureDef.filter.groupIndex = -1;

	bodyUserData* myStruct = new bodyUserData;
	myStruct->health = 4;
	myStruct->name = "player";
	body->SetUserData(this);

	body->CreateFixture(&fixtureDef);

	world.Step(timeStep, velocityIterations, positionIterations);
	
	posX = x ;
	posY = y;

	bodyType = "player";
}

void Physics::makeStatic(float x, float y, float width, float height)
{
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(x, y);

	groundBody = world.CreateBody(&groundBodyDef);

	b2PolygonShape groundBox;
	groundBox.SetAsBox(width, height);
	groundBody->SetUserData(this);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &groundBox;
	fixtureDef.filter.categoryBits = CATEGORY_SCENERY;
	fixtureDef.filter.maskBits = CATEGORY_SCENERY;
	fixtureDef.filter.groupIndex = -3;

	groundBody->CreateFixture(&fixtureDef);
	pos = groundBody->GetPosition();
	posX = pos.x;
	posY = pos.y;

	bodyType = "static";

}

b2Body* Physics::makeStaticReturn(int x, int y, int width, int height)
{
	b2Body* tempBody;
	b2BodyDef tempBodyDef;
	tempBodyDef.position.Set(x, y);

	tempBody = world.CreateBody(&tempBodyDef);

	b2PolygonShape tempBox;
	tempBox.SetAsBox(width, height);
	tempBody->SetUserData(this);

	b2FixtureDef tempDef;
	tempDef.shape = &tempBox;
	tempDef.filter.categoryBits = CATEGORY_SCENERY;
	tempDef.filter.maskBits = CATEGORY_SCENERY;
	tempDef.filter.groupIndex = -3;

	tempBody->CreateFixture(&tempDef);

	return tempBody;
}

void Physics::move(const Uint8* keystates)
{

	player.setTexture(keystates);

	if (keystates[SDL_SCANCODE_D] && (keystates[SDL_SCANCODE_W]))
		moveState = MS_UPRIGHT;
	else if (keystates[SDL_SCANCODE_A] && (keystates[SDL_SCANCODE_W]))
		moveState = MS_UPLEFT;
	else if (keystates[SDL_SCANCODE_D] && keystates[SDL_SCANCODE_S])
		moveState = MS_DOWNRIGHT;
	else if (keystates[SDL_SCANCODE_A] && keystates[SDL_SCANCODE_S])
		moveState = MS_DOWNLEFT;
	else if (keystates[SDL_SCANCODE_D])
		moveState = MS_RIGHT;
	else if (keystates[SDL_SCANCODE_A])
		moveState = MS_LEFT;
	else if (keystates[SDL_SCANCODE_W])
		moveState = MS_UP;
	else if (keystates[SDL_SCANCODE_S])
		moveState = MS_DOWN;
	else 
		moveState = MS_STOP;
}

void Physics::moveBodies()
{
	
	myCollisionDetection myListener;
	world.SetContactListener(&myListener);

	if (bodyType != "player")
	{
		
	}
	else if (bodyType == "player")
	{

		body->SetAwake(true);

		float forceX = 0;
		float forceY = 0;
		
		switch (moveState)
		{
		case MS_UP:
			forceY = -50;
			break;
		case MS_DOWN:
			forceY = 50;
			break;
		case MS_LEFT:
			forceX = -50;
			break;
		case MS_RIGHT:
			forceX = 300;
			break;
		case MS_UPRIGHT:
			forceY = -50;
			forceX = 50;
			break;
		case MS_UPLEFT:
			forceY = -50;
			forceX = -50;
			break;
		case MS_DOWNRIGHT:
			forceY = 50;
			forceX = 50;
			break;
		case MS_DOWNLEFT:
			forceY = 50;
			forceX = -50;
			break;
		case MS_STOP:
			body->SetLinearVelocity(b2Vec2(0, 0));
		}

		body->ApplyLinearImpulse(b2Vec2(forceX * 20, forceY * 20), body->GetWorldCenter(), true);
		//body->SetTransform(b2Vec2(body->GetPosition().x * PPM, body->GetPosition().y * PPM), body->GetAngle());
		pos = body->GetPosition();
		posX = body->GetPosition().x;
		posY = body->GetPosition().y;
	}

	world.Step(timeStep, velocityIterations, positionIterations);
}

/* ----------------------------- Collision --------------------------------------*/

void myCollisionDetection::BeginContact(b2Contact* contact)
{

	void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
	void* bodyUserData1 = contact->GetFixtureB()->GetBody()->GetUserData();
	Physics* physics1 = static_cast<Physics*>(bodyUserData)->returnCurrent();
	Physics* physics2 = static_cast<Physics*>(bodyUserData1)->returnCurrent();

	Physics test;

	if (bodyUserData && bodyUserData1)
	{

		b2Filter filterA = physics1->body->GetFixtureList()->GetFilterData();
		b2Filter filterB = contact->GetFixtureB()->GetBody()->GetFixtureList()->GetFilterData();

		if (physics1->body != NULL)
		{
			// Calls when player hits wall (0x0001 == player)
			if (filterA.categoryBits == 0x0001)
			{
				printf("test");
			}

			// Calls when enemy/monster hits the ground (0x0002 == enemy/monster)
			if (filterA.categoryBits == 0x0002)
			{
				printf("test1");
			}

			// Calls when something touches the ground (0x0003 == ground/platform)
			if (filterB.categoryBits == 0x0003)
			{
				printf("test2");
			}

		}

	}

}

void myCollisionDetection::EndContact(b2Contact* contact)
{

	void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
	void* bodyUserData1 = contact->GetFixtureB()->GetBody()->GetUserData();

	b2Body* groundBody = contact->GetFixtureB()->GetBody();

	Physics* physics1 = static_cast<Physics*>(bodyUserData)->returnCurrent();
	Physics* physics2 = static_cast<Physics*>(bodyUserData1)->returnCurrent();

	if (bodyUserData && bodyUserData1)
	{
		b2Filter filterA = physics1->body->GetFixtureList()->GetFilterData();
		b2Filter filterB = contact->GetFixtureB()->GetBody()->GetFixtureList()->GetFilterData();

		if (physics1->body != NULL)
		{
			// Calls when player hits the ground (0x0001 == player)
			if (filterA.categoryBits == 0x0001)
			{
				
			}

			// Calls when enemy/monster hits the ground (0x0002 == enemy/monster)
			if (filterA.categoryBits == 0x0002)
			{

			}

			// Calls when something touches the ground (0x0003 == ground/platform)
			if (filterB.categoryBits == 0x0003)
			{

			}

		}
	}

}

