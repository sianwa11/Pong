#ifndef COMPONENT_H
#define COMPONENT_H

#include "Vec2.h"
#include <SFML/Graphics.hpp>


class cTransform
{
public:
	Vec2 pos = { 0.0, 0.0 };
	Vec2 velocity = { 0.0, 0.0 };
	float angle = 0;

	cTransform(const Vec2& p, const Vec2& v, float a) :pos(p), velocity(v), angle(a) {};
};


class cShape
{
public:
	sf::CircleShape circle;

	cShape(float radius, int points, const sf::Color& fill, const sf::Color& outline, float thickness) :circle(radius, points)
	{
		circle.setFillColor(fill);
		circle.setOutlineColor(outline);
		circle.setOutlineThickness(thickness);
		circle.setOrigin(radius, radius);
	};
};

class cRect
{
public:
	sf::RectangleShape rect;

	cRect(const sf::Vector2f& size, const sf::Color& fill, const sf::Color& outline, float thickness) :rect(size)
	{
		rect.setFillColor(fill);
		rect.setOutlineColor(outline);
		rect.setOutlineThickness(thickness);
	};
};

class cCollision
{
public:
	float radius = 0;
	cCollision(float r) :radius(r) {};
};

class cScore
{
public:
	int score = 0;
	cScore(int s) :score(s) {};
};

class cLifespan
{
public:
	int remaining = 0;
	int total = 0;

	cLifespan(int total) : remaining(total), total(total) {};
};

class cInput
{
public:
	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;

	cInput() {};
};


#endif
