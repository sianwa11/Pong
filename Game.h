#ifndef GAME_H
#define GAME_h

#include "Entity.h"
#include "EntityManager.h"

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>


struct WindowConfig { int w, h;  std::string name; };
class Game 
{
sf::RenderWindow m_window;
WindowConfig m_windowConfig;
EntityManager m_entities;

bool m_running = true;
Entity* m_ball;
Entity* m_leftPole;
Entity* m_rightPole;

void sMovement();
void sUserInput();
void sCollision();
void sRender();

void spawnPlayer();
void spawnPoles();

void poleOutOfBounds(Entity* pole);
void ballOutOfBounds(Entity* ball);

void init(const std::string& config);

public:

	Game(const std::string &config);
	~Game();


	void run();

};

#endif

