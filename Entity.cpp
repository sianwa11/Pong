#include "Entity.h"

Entity::Entity(int id, const std::string& tag):m_id(id), m_tag(tag) {}

Entity::~Entity()
{
	delete cTransform;
	delete cShape;
	delete cRect;
	delete cCollision;
	delete cScore;
	delete cLifespan;
	delete cInput;
}

bool Entity::isActive() const
{
	return m_active;
}

const std::string& Entity::getTag() const
{
	return m_tag;
}

const size_t Entity::id() const
{
	return size_t();
}

void Entity::destroy()
{
	m_active = false;
}
