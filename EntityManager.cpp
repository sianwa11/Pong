#include "EntityManager.h"
#include <vector>

void EntityManager::removeDeadEntities(EntityVec& vec)
{
	auto newEnd = std::remove_if(vec.begin(), vec.end(), [](Entity* entity) {
		if(!entity->isActive())
		{
			delete entity;
			return true;
		}
		return false;
	});

	vec.erase(newEnd, vec.end());
}

EntityManager::EntityManager()
{
}

EntityManager::~EntityManager()
{
	// Delete hanging m_entities
	for (Entity* e : m_entities)
	{
		delete e;
	}
	m_entities.clear();

	// Deletee hanging m_entitiesToAdd
	for (Entity* e : m_entitiesToAdd)
	{
		delete e;
	}
	m_entitiesToAdd.clear();

	// Delete hanging m_entityMap
	for (auto& pair : m_entityMap)
	{
		pair.second.clear();
	}
	m_entityMap.clear();
}

void EntityManager::update()
{
	// Add All entities from m_entitiesToAdd to their proper locations
	// Add them to the vector of all entities
	// Add them to the vector in the map, with their tag as key
	for (Entity* e : m_entitiesToAdd)
	{
		m_entities.push_back(e);
		m_entityMap[e->getTag()].push_back(e);
	}

	m_entitiesToAdd.clear();

	removeDeadEntities(m_entities);

	// Remove dead entities in entitymap
	for (auto& pair : m_entityMap)
	{
		std::string tag = pair.first;
		EntityVec& entityVec = pair.second;

		removeDeadEntities(entityVec);
	}
}

Entity* EntityManager::addEntity(const std::string& tag)
{
	// Create a new Entity Object
	Entity* entity = new Entity(m_totalEntities++, tag);
	//
	m_entitiesToAdd.push_back(entity);
	return entity;
}

const EntityVec& EntityManager::getEntities()
{
	return m_entities;
}

const EntityVec& EntityManager::getEntities(const std::string& tag)
{
	return m_entityMap[tag];
}
