#include <GdpPhysics/gdp/PhysicsFactory.h>
#include <GdpPhysics/interfaces/SphereShape.h>
#include <GdpPhysics/gdp/RigidBody.h>

class GameObject {
public:
	Vector3 position;
	iRigidBody* rigidBody;
};

void Test()
{
	GameObject redBall;
	GameObject blueBall;

	// Initialize a Physics Factory
	iPhysicsFactory* physicsFactory = new PhysicsFactory();

	// Create our GDP Physics World
	iPhysicsWorld* world = physicsFactory->CreateWorld();

	// Create a ball 
	RigidBodyDesc ballDesc;
	ballDesc.isStatic = false;
	ballDesc.mass = 10.f;
	ballDesc.position = Vector3(0.f, 0.f, 0.f);
	ballDesc.velocity = Vector3(10.f, 0.f, 0.f);

	iShape* ballShape = new SphereShape(1.0f);

	redBall.rigidBody = physicsFactory->CreateRigidBody(ballDesc, ballShape);

	ballDesc.position = Vector3(100.f, .2f, 0.f);
	ballDesc.velocity = Vector3(-10.f, 0.f, 0.f);
	ballDesc.mass = 4.f;
	blueBall.rigidBody = physicsFactory->CreateRigidBody(ballDesc, ballShape);

	// Add our ball to our PhysicsWorld
	world->AddRigidBody(redBall.rigidBody);
	world->AddRigidBody(blueBall.rigidBody);

	// We can use a pointer to update the GameObject's position from the Physics
	// side instead of manually updating them after the Physics update.
	//RigidBody* body = dynamic_cast<RigidBody*>(redBall.rigidBody);
	//body->SetRenderPosition(&redBall.position);

	world->TimeStep(1.0f);
	world->TimeStep(1.0f);
	world->TimeStep(1.0f);
	world->TimeStep(1.0f);
	world->TimeStep(1.0f);

	int breakpoint = 0;
}


int main(int argc, char** argv)
{
	Test();

	return 0;
}