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

	if (!m_font.loadFromFile("fonts/font.ttf"))
	{
		std::cerr << "Error loading font" << std::endl;
	}

	m_player1ScoreText.setFont(m_font);
	m_player2ScoreText.setFont(m_font);
	m_pauseText.setFont(m_font);


	while (fin >> key)
	{
		if (key == "Window")
		{
			fin >> m_windowConfig.W >> m_windowConfig.H >> m_windowConfig.NAME;
		}

		if (key == "Ball")
		{
			fin >> m_ballConfig.SR >> m_ballConfig.CR >> m_ballConfig.OT >> m_ballConfig.V >> m_ballConfig.S;
		}
	}
	m_window.create(sf::VideoMode(m_windowConfig.W, m_windowConfig.H), m_windowConfig.NAME);

	spawnPlayer();
	spawnPoles();
}

void Game::spawnPlayer()
{
	Entity* ball =  m_entities.addEntity("ball");

	float W = static_cast<float>(m_windowConfig.W / 2);
	float H = static_cast<float>(m_windowConfig.H / 2);

	// Randomly choose a positive or negative direction for X and Y
	float DX = (std::rand() % 2 == 0 ? 0.1f : -0.1f) ;
	float DY = (std::rand() % 11  - 5.0f) / 20.0f ;
	std::cout << "DX: " << DX << ", DY: " << DY << "\n";

	ball->cRect = new cRect(sf::Vector2f(10.0f, 10.0f), sf::Color::White, sf::Color::White, 1.0f);
	ball->cTransform = new cTransform(Vec2(W, H), Vec2(DX, DY), 1.0f);

	m_ball = ball;
}

void Game::spawnPoles()
{
	Entity* leftPole = m_entities.addEntity("player1");
	Entity* rightPole = m_entities.addEntity("player2");

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

	rightPole->cScore = new cScore(0);
	leftPole->cScore = new cScore(0);

	m_leftPole = leftPole;
	m_rightPole = rightPole;
}

void Game::resetGame()
{
	m_leftPole->cScore->score = 0;
	m_rightPole->cScore->score = 0;
	m_winningText.setString("");
	m_paused = false;
}

void Game::winningState(Entity* ball, Entity* pole)
{
	float centreX = static_cast<float>(m_windowConfig.W / 2);
	float centreY = static_cast<float>(m_windowConfig.H / 2);

	ball->cTransform->pos.x = centreX;
	ball->cTransform->pos.y = centreY;
	m_paused = true;

	m_winningText.setFont(m_font);
	auto txt = pole->getTag() + " has won!" + "\n" + "Press Enter to start again";
	m_winningText.setString(txt);
	m_winningText.setPosition((m_window.getSize().x - m_winningText.getGlobalBounds().width) / 2, 40.0f);
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

				case sf::Keyboard::Space:
					m_paused = !m_paused;
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

		if (m_paused && event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::Enter:
				resetGame();
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

	if (checkCollision(m_ball, m_leftPole))
	{
		// Reposition the ball just outside the left pole
		float poleRightEdge = m_leftPole->cTransform->pos.x + m_leftPole->cRect->rect.getSize().x;
		m_ball->cTransform->pos.x = poleRightEdge;

		m_ball->cTransform->velocity.x = -(m_ball->cTransform->velocity.x * 1.03f);


		// Check the current velocity direction and adjust it
		checkAndAdjustVelocity(m_ball);
	}

	if (checkCollision(m_ball, m_rightPole))
	{
		m_ball->cTransform->pos.x = m_rightPole->cTransform->pos.x - m_ball->cRect->rect.getSize().x;
		m_ball->cTransform->velocity.x = -(m_ball->cTransform->velocity.x * 1.03f);


		// Check the current velocity direction and adjust it
		checkAndAdjustVelocity(m_ball);
	}

	

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
	if (ball->cTransform->pos.y < 0)
	{
		ball->cTransform->pos.y = 0;
		ball->cTransform->velocity.y = -ball->cTransform->velocity.y;
	}

	if (ball->cTransform->pos.y > m_windowConfig.H)
	{
		ball->cTransform->pos.y = m_windowConfig.H;
		ball->cTransform->velocity.y = -ball->cTransform->velocity.y;
	}

	if (ball->cTransform->pos.x > m_windowConfig.W)
	{
		m_leftPole->cScore->score += 1;
		m_isPlayer1Serving = true;

		if (m_leftPole->cScore->score >= m_winningScore)
		{
			winningState(ball, m_leftPole);
		}
		else 
		{
			reset(ball, m_isPlayer1Serving);
		}
	}

	if (ball->cTransform->pos.x < 0)
	{
		m_rightPole->cScore->score += 1;
		m_isPlayer1Serving = false;

		if (m_rightPole->cScore->score >= m_winningScore)
		{
			winningState(ball, m_rightPole);
		}
		else
		{
			reset(ball, m_isPlayer1Serving);
		}
	}
}

void Game::checkAndAdjustVelocity(Entity* ball) const
{
	if (ball->cTransform->velocity.y < 0)
	{
		float DY = (std::rand() % 11 - 5.0f) / 20.0f;

		ball->cTransform->velocity.y = -DY;
	}
	else
	{
		float DY = (std::rand() % 11 - 5.0f) / 20.0f;
		ball->cTransform->velocity.y = DY;
	}
}

bool Game::checkCollision(Entity* ball, Entity* pole)
{
	float ballX = ball->cTransform->pos.x;
	float ballRightEdge = ball->cTransform->pos.x + ball->cRect->rect.getSize().x;
	float ballY = ball->cTransform->pos.y;
	float ballBottomEdge = ball->cTransform->pos.y + ball->cRect->rect.getSize().y;

	float poleX = pole->cTransform->pos.x;
	float poleRightEdge = pole->cTransform->pos.x + pole->cRect->rect.getSize().x;
	float poleY = pole->cTransform->pos.y;
	float poleBottomEdge = pole->cTransform->pos.y + pole->cRect->rect.getSize().y;

	if (ballRightEdge < poleX || ballX > poleRightEdge || ballBottomEdge < poleY || ballY > poleBottomEdge)
	{
		return false;  // No collision
	}

	return true;  // Collision detected
}

void Game::reset(Entity* ball, bool isPlayer1Serving) const
{
	ball->cTransform->pos.x = static_cast<float>(m_windowConfig.W / 2);
	ball->cTransform->pos.y = static_cast<float>(m_windowConfig.H / 2);


	// Randomly choose a positive or negative direction for X and Y
	//float DX = (std::rand() % 2 == 0 ? 0.1f : -0.1f);
	float DX = 0.1f * 1.03f;
	float DY = ((std::rand() % 11 - 5.0f) / 20.0f) * 1.03f;


	ball->cTransform->velocity.x = isPlayer1Serving ? DX : -DX;
	ball->cTransform->velocity.y = DY;
}
	
void Game::sMovement()
{
	m_rightPole->cTransform->velocity.y = 0;
	m_leftPole->cTransform->velocity.y = 0;

	if (m_rightPole->cInput->up)
	{
		m_rightPole->cTransform->velocity.y = -1.03f;
	}

	if (m_rightPole->cInput->down)
	{
		m_rightPole->cTransform->velocity.y = 1.03f;
	}	
	
	if (m_leftPole->cInput->up)
	{
		m_leftPole->cTransform->velocity.y = -1.03f;
	}

	if (m_leftPole->cInput->down)
	{
		m_leftPole->cTransform->velocity.y = 1.03f;
	}

	m_rightPole->cTransform->pos.y += m_rightPole->cTransform->velocity.y;
	m_leftPole->cTransform->pos.y += m_leftPole->cTransform->velocity.y;


	m_ball->cTransform->pos.y += m_ball->cTransform->velocity.y;
	m_ball->cTransform->pos.x +=m_ball->cTransform->velocity.x;

}

void Game::sRender()
{
	m_window.clear();
	auto p1 = std::to_string(m_leftPole->cScore->score);
	m_player1ScoreText.setString(p1);

	auto p2 = std::to_string(m_rightPole->cScore->score);
	m_player2ScoreText.setString(p2);

	m_player1ScoreText.setPosition(sf::Vector2f(m_windowConfig.W/2 - m_player1ScoreText.getGlobalBounds().width - 20, 0));
	m_player2ScoreText.setPosition(sf::Vector2f(m_windowConfig.W/2 + m_player2ScoreText.getGlobalBounds().width + 20, 0));

	std::string paused = m_paused ? "Paused" : "";
	m_pauseText.setString(paused);
	m_pauseText.setPosition(sf::Vector2f(m_windowConfig.W - m_pauseText.getGlobalBounds().width - 10, 0));

	m_window.draw(m_player1ScoreText);
	m_window.draw(m_player2ScoreText);
	m_window.draw(m_winningText);
	m_window.draw(m_pauseText);


	for (auto& e : m_entities.getEntities())
	{
		e->cRect->rect.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);
		m_window.draw(e->cRect->rect);
	}


	m_window.display();
}


void Game::run()
{
	while (m_running)
	{
		if (!m_paused)
		{
			m_entities.update();
			sMovement();
			sCollision();
		}

		sUserInput();
		sRender();
	}
}