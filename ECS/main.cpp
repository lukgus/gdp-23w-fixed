#include <engine/engine.h>
#include <systems/motionsystem.h>
#include <physics/PhysicsSystem.h>
#include <physics/components/rigidbodycomponent.h>
#include <components/positioncomponent.h>
#include <components/velocitycomponent.h>

#include "SQLiteDBHelper.h"
#include <lua/lua.hpp>

Engine g_Engine;

unsigned int CreateAnEntity()
{
	unsigned int entity = g_Engine.CreateEntity();
	RigidBodyComponent* rbc = g_Engine.AddComponent<RigidBodyComponent>(entity);
	//rbc->SetRigidBody();
	//rbc->SetPhysicsMaterial();
	// Some way to customize the rbc
	// You want to assign the shape yourself, and collider info


	return entity;
}

struct PositionVelocityEntityType
{
	PositionComponent position;
	VelocityComponent velocity;
};

std::vector<PositionVelocityEntityType> position_velocity_types;



void RunECSEngine()
{
	MotionSystem motion;
	PhysicsSystem physics;

	g_Engine.AddSystem(&motion);
	g_Engine.AddSystem(&physics);

	g_Engine.Update(0.1f);
	g_Engine.Update(0.1f);
	g_Engine.Update(0.1f);

	unsigned int entity = CreateAnEntity();

	g_Engine.Update(0.1f);
	g_Engine.Update(0.1f);
	g_Engine.Update(0.1f);


	//VelocityComponent* velCmp = g_Engine.AddComponent<VelocityComponent>(entity);
	//velCmp->vx = 10;
	//velCmp->vy = 0;
	//velCmp->vz = 0;

	g_Engine.Update(0.1f);
	g_Engine.Update(0.1f);
	g_Engine.Update(0.1f);

	g_Engine.RemoveSystem(&motion);

}

static int playSound(lua_State* L) {
	const char* soundName = lua_tostring(L, 1);
	printf("Playing sound: %s\n", soundName);
	return 0;
}

void RunLuaScript(lua_State* L)
{
	luaL_loadfile(L, "hello_world.lua");

	lua_pushstring(L, "skylands");
	lua_setglobal(L, "area");

	lua_pcall(L, 0, 1, 0);
	lua_close(L);
}

void LuaTest()
{
	lua_State* lua_state;
	lua_state = luaL_newstate();

	luaL_openlibs(lua_state);

	lua_pushcfunction(lua_state, playSound);
	lua_setglobal(lua_state, "playSound");
	RunLuaScript(lua_state);

	system("pause");
}

void DoLuaExample()
{
	for (int i = 0; i < 5; i++)
	{
		LuaTest();
	}
}

void DoSQLiteExample()
{
	SQLiteDBHelper sql;

	sql.ConnectToDB("entities.db");

	sql.ExecuteQuery("SELECT * FROM Entities;");
}

int main(int argc, char** argv)
{
	DoSQLiteExample();
	return 0;
}
