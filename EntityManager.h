#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "Entity.h"
#include <iostream>
#include <vector>
#include <map>

typedef std::vector<Entity*> EntityVec;
typedef std::map<std::string, EntityVec> EntityMap;

class EntityManager
{
private:
	EntityVec m_entities;
	EntityVec m_entitiesToAdd;
	EntityMap m_entityMap;
	int m_totalEntities = 0;

	void removeDeadEntities(EntityVec& vec);

public:
	EntityManager();
	~EntityManager();
	void update();
	Entity* addEntity(const std::string& tag);
	const EntityVec& getEntities();
	const EntityVec& getEntities(const std::string& tag);
};

#endif // !ENTITYMANAGER_H

