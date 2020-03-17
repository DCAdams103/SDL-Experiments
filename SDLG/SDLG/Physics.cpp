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

// Creates a body that can be affected by forces
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

	// Updates the world
	world.Step(timeStep, velocityIterations, positionIterations);
	posX = body->GetPosition().x;
	posY = body->GetPosition().y;

	bodyType = "dynamic";

}

// Creates the player physcis body. One that can be affected by forces
void Physics::playerDynamic(float x, float y, float width, float height)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(x * PTM, y * PTM);
	
	body = world.CreateBody(&bodyDef);

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(PTM * ((200.0f) / 7.0f), PTM * ((200.0f) / 7.0f));

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
	
	posX = x *MTP;
	posY = y *MTP;

	bodyType = "player";
}

// Creates physics body that cannot be affected by force
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
	posX = groundBody->GetPosition().x;
	posY = groundBody->GetPosition().y;

	bodyType = "static";

}

// Returns a static physics body
b2Body* Physics::makeStaticReturn(float x, float y, float width, float height)
{
	b2Body* tempBody;
	b2BodyDef tempBodyDef;
	tempBodyDef.position.Set(x * PTM, y *PTM);

	tempBody = world.CreateBody(&tempBodyDef);

	b2PolygonShape tempBox;
	tempBox.SetAsBox(PTM * ((200.0f) / 7.0f), PTM * ((200.0f) /7.0f));
	tempBody->SetUserData(this);

	b2FixtureDef tempDef;
	tempDef.shape = &tempBox;
	tempDef.filter.categoryBits = CATEGORY_SCENERY;
	tempDef.filter.maskBits = CATEGORY_SCENERY;
	tempDef.filter.groupIndex = -3;

	tempBody->CreateFixture(&tempDef);

	bodyType = "static";

	return tempBody;
}

// Determines the direction to move the character based on pressed keys
void Physics::move(const Uint8* keystates)
{
	// Sets the correct facing texture based on direction
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

// Sets the force needed to be applied based on which way the character is moving
void Physics::moveBodies()
{
	
	// Sets collision detection to detect collisions
	myCollisionDetection myListener;
	world.SetContactListener(&myListener);

	if (bodyType != "player")
	{
		
	}
	else if (bodyType == "player")
	{
		// To make sure the physics body is always awake to receive forces
		body->SetAwake(true);

		float forceX = 0;
		float forceY = 0;
		
		// Sets force based on each direction
		switch (moveState)
		{
		case MS_UP:
			forceY = -5;
			break;
		case MS_DOWN:
			forceY = 5;
			break;
		case MS_LEFT:
			forceX = -5;
			break;
		case MS_RIGHT:
			forceX = 5;
			break;
		case MS_UPRIGHT:
			forceY = -5;
			forceX = 5;
			break;
		case MS_UPLEFT:
			forceY = -5;
			forceX = -5;
			break;
		case MS_DOWNRIGHT:
			forceY = 5;
			forceX = 5;
			break;
		case MS_DOWNLEFT:
			forceY = 5;
			forceX = -5;
			break;
		case MS_STOP:
			body->SetLinearVelocity(b2Vec2(0, 0));
		}

		// Applies force to body and grabs new position
		float velChange = forceX - body->GetLinearVelocity().x;
		float velChangeY = forceY - body->GetLinearVelocity().y;
		float forceX1 = body->GetMass() * velChange / (1 / 60.0f); // f = mv/t
		float forceY1 = body->GetMass() * velChangeY / (1 / 60.0f);
		body->ApplyForce(b2Vec2(forceX1, forceY1), body->GetWorldCenter(), true);

		posX = body->GetPosition().x*MTP;
		posY = body->GetPosition().y*MTP;
	}

	// Updates physics world
	world.Step(timeStep, velocityIterations, positionIterations);
}

/* ----------------------------- Collision --------------------------------------*/

// All used for collision.
// Calls when a collision has occured.
void myCollisionDetection::BeginContact(b2Contact* contact)
{
	// Userdata is how you tell the difference in bodies
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

// Calls when collision has stopped.
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


