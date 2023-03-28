#pragma once

class Entity;
class EntityModelDAOInterface
{
public:
	EntityModelDAOInterface() {}
	virtual ~EntityModelDAOInterface() {}

	virtual Entity* Create(const char* name, int type) = 0;
	virtual Entity* Find(int id) = 0;
	virtual Entity* Update(Entity* entity) = 0;
	virtual Entity* Delete(Entity* entity) = 0;
};