#ifndef GAME_H
#define GAME_h

#include "Entity.h"
#include "EntityManager.h"

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>


struct WindowConfig { int W, H;  std::string NAME; };
struct BallConfig { int SR, CR, OT, V; float S; };

class Game 
{
sf::RenderWindow m_window;
sf::Font m_font;
sf::Text m_player1ScoreText;
sf::Text m_player2ScoreText;
sf::Text m_winningText;
sf::Text m_pauseText;

int m_winningScore = 10;

WindowConfig m_windowConfig;
BallConfig m_ballConfig;
EntityManager m_entities;

bool m_running = true;
bool m_isPlayer1Serving = true;
bool m_paused = false;

std::string m_gameState = "play";

Entity* m_ball;
Entity* m_leftPole;
Entity* m_rightPole;

void sMovement();
void sUserInput();
void sCollision();
void sRender();

void spawnPlayer();
void spawnPoles();
void resetGame();

void winningState(Entity* ball, Entity* pole);
void poleOutOfBounds(Entity* pole);
void ballOutOfBounds(Entity* ball);
void checkAndAdjustVelocity(Entity* ball) const;
void reset(Entity* ball, bool isPlayer1Serving) const;

bool checkCollision(Entity* ball, Entity* pole);

void init(const std::string& config);

public:

	Game(const std::string &config);
	~Game();


	void run();

};

#endif

