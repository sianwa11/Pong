#include "Game.h"

#include <fstream>
#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>


Game::Game(const std::string& config)
{
	init(config);

}

Game::~Game()
{
}


void Game::init(const std::string& config)
{
	// Get configs from a file
	std::ifstream fin;
	std::string key;

	fin.open(config);
	if (!fin.is_open())
	{
		std::cerr << "Error opening file" << std::endl;
	}

	while (fin >> key)
	{
		if (key == "Window")
		{
			fin >> m_windowConfig.w >> m_windowConfig.h >> m_windowConfig.name;
		}
	}
	m_window.create(sf::VideoMode(m_windowConfig.w, m_windowConfig.h), m_windowConfig.name);
	spawnPlayer();
	spawnPoles();
}

void Game::spawnPlayer()
{
	Entity* ball =  m_entities.addEntity("ball");

	srand((unsigned)std::time(nullptr));

	float mx = m_window.getSize().x / 2.0f;
	float my = m_window.getSize().y / 2.0f;

	float vx = -2.0f + static_cast<float>(rand() % 10);
	float vy = -2.0f + static_cast<float>(rand() % 10);
	float speedFactor = 0.2f;
	vx *= speedFactor;
	vy *= speedFactor;


	ball->cTransform = new cTransform(Vec2(mx, my), Vec2(vx, vy), 0.0f);
	ball->cShape = new cShape(10.0f, 8, sf::Color::White, sf::Color::White, 1.0f);

	m_ball = ball;
}

void Game::spawnPoles()
{
	Entity* leftPole = m_entities.addEntity("leftPole");
	Entity* rightPole = m_entities.addEntity("rightPole");

	//cRect(const sf::Vector2f & size, const sf::Color & fill, const sf::Color & outline, float thickness) :rect(size)
	// 	cTransform(const Vec2& p, const Vec2& v, float a) :pos(p), velocity(v), angle(a) {};

	leftPole->cRect = new cRect(sf::Vector2f(25.0f, 200.0f), sf::Color::White, sf::Color::White, 1.0f);
	rightPole->cRect = new cRect(sf::Vector2f(25.0f, 200.0f), sf::Color::White, sf::Color::White, 1.0f);

	float my = m_window.getSize().y / 2.0f;
	float rightX = m_window.getSize().x - 25.0f;

	leftPole->cTransform = new cTransform(Vec2(0, my), Vec2(1.0f, 1.0f), 0.0f);
	rightPole->cTransform = new cTransform(Vec2(rightX, my), Vec2(1.0f, 1.0f), 0.0f);

	rightPole->cInput = new cInput();
	leftPole->cInput = new cInput();

	m_leftPole = leftPole;
	m_rightPole = rightPole;
}


void Game::sUserInput()
{
	sf::Event event;

	while (m_window.pollEvent(event))
	{
		if(event.type == sf::Event::Closed)
		{ 
			m_running = false;
		}

		if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
				case sf::Keyboard::Up:
					m_rightPole->cInput->up = true;
				break;	
				
				case sf::Keyboard::W:
					m_leftPole->cInput->up = true;
				break;	
				
				case sf::Keyboard::Down:
					m_rightPole->cInput->down = true;
				break;
				
				case sf::Keyboard::S:
					m_leftPole->cInput->down = true;
				break;

				case sf::Keyboard::Escape:
					m_window.close();
					m_running = false;
				break;

				default:
					break;
			}
		}

		if (event.type == sf::Event::KeyReleased)
		{
			switch (event.key.code)
			{
				case sf::Keyboard::Up:
					m_rightPole->cInput->up = false;
				break;

				case sf::Keyboard::W:
					m_leftPole->cInput->up = false;
				break;

				case sf::Keyboard::Down:
					m_rightPole->cInput->down = false;
				break;

				case sf::Keyboard::S:
					m_leftPole->cInput->down = false;
				break;

				default:
					break;
			}
		}

	}
}

void Game::sCollision()
{
	ballOutOfBounds(m_ball);
	poleOutOfBounds(m_rightPole);
	poleOutOfBounds(m_leftPole);

}

void Game::poleOutOfBounds(Entity* pole)
{
	float poleHeight = pole->cRect->rect.getGlobalBounds().height;
	float windowHeight = m_window.getSize().y;

	if (pole->cTransform->pos.y <= 0)
	{
		pole->cTransform->pos.y = 0;
	}
	else if (pole->cTransform->pos.y >= windowHeight - poleHeight)
	{
		pole->cTransform->pos.y = windowHeight - poleHeight; 
	}
}

void Game::ballOutOfBounds(Entity* ball)
{
	float ballRadius = ball->cShape->circle.getRadius();
	float windowHeight = m_window.getSize().y;
	float windowWidth = m_window.getSize().x;

	if (ball->cTransform->pos.x >= windowWidth - ballRadius)
	{
		ball->cTransform->pos.x = windowWidth - ballRadius;
		ball->cTransform->velocity.x *= -1.0f;
	}
	else if (ball->cTransform->pos.x <= ballRadius)
	{
		ball->cTransform->pos.x = ballRadius;
		ball->cTransform->velocity.x *= -1.0f;
	}

	if (ball->cTransform->pos.y >= windowHeight - ballRadius)
	{
		ball->cTransform->pos.y = windowHeight - ballRadius;
		ball->cTransform->velocity.y *= -1.0f;
	}
	else if(ball->cTransform->pos.y <= ballRadius)
	{
		ball->cTransform->pos.y = ballRadius;
		ball->cTransform->velocity.y *= -1.0f;
	}
}
	
void Game::sMovement()
{
	m_rightPole->cTransform->velocity.y = 0;
	m_leftPole->cTransform->velocity.y = 0;

	if (m_rightPole->cInput->up)
	{
		m_rightPole->cTransform->velocity.y = -1.5f;
	}

	if (m_rightPole->cInput->down)
	{
		m_rightPole->cTransform->velocity.y = 1.5f;
	}	
	
	if (m_leftPole->cInput->up)
	{
		m_leftPole->cTransform->velocity.y = -1.5f;
	}

	if (m_leftPole->cInput->down)
	{
		m_leftPole->cTransform->velocity.y = 1.5f;
	}

	m_rightPole->cTransform->pos.y += m_rightPole->cTransform->velocity.y;
	m_leftPole->cTransform->pos.y += m_leftPole->cTransform->velocity.y;

	m_ball->cTransform->pos.y += m_ball->cTransform->velocity.y;
	m_ball->cTransform->pos.x += m_ball->cTransform->velocity.x;

}

void Game::sRender()
{
	m_window.clear();

	for (auto& e : m_entities.getEntities())
	{
		if (e->getTag() == "ball")
		{
			e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);
			e->cTransform->angle += 1.0f;
			e->cShape->circle.setRotation(e->cTransform->angle);
			m_window.draw(e->cShape->circle);
		}

		if (e->getTag() == "leftPole" || e->getTag() == "rightPole")
		{
			e->cRect->rect.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);
			m_window.draw(e->cRect->rect);
		}

	}

	m_window.display();
}


void Game::run()
{
	while (m_running)
	{
		m_entities.update();
		sMovement();
		sCollision();
		sUserInput();

		sRender();
	}
}