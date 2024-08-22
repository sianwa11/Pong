#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include "Component.h";

class Entity
{
private:
	friend class EntityManager;
	bool m_active = true;
	size_t m_id = 0;
	std::string m_tag = "";


public:
	Entity(int id, const std::string& tag);
	~Entity();

	cTransform* cTransform;
	cShape* cShape;
	cRect* cRect;
	cCollision* cCollision;
	cScore* cScore;
	cLifespan* cLifespan;
	cInput* cInput;

	bool isActive() const;
	const std::string& getTag() const;
	const size_t id() const;
	void destroy();

};

#endif // !ENTITY_H
