#include "Player.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include "CommonMath.h"

Player::Player()
	: m_FacingDirection(1.f, 0.f)
{
}

Player::~Player()
{
}

void Player::handleEvents(sf::Event& event)
{
	switch (event.type)
	{
	case sf::Event::KeyPressed:
		//switch (event.key.code)
		//{
		//case sf::Keyboard::W: 
		//	m_FacingDirection.y = 1;
		//	m_HasMoved = true;
		//	break;
		//case sf::Keyboard::A: 
		//	m_FacingDirection.x = -1;
		//	m_HasMoved = true;
		//	break;
		//case sf::Keyboard::S: 
		//	m_FacingDirection.y = -1;
		//	m_HasMoved = true;
		//	break;
		//case sf::Keyboard::D: 
		//	m_FacingDirection.x = 1;
		//	m_HasMoved = true;
		//	break;
		//default: break;
		//}

		break;
	}
}

void Player::update()
{
	sf::Vector2f direction;

	typedef sf::Keyboard K;
	if (K::isKeyPressed(K::D))
		direction.x += 1;
	if (K::isKeyPressed(K::A))
		direction.x += -1;
	if (K::isKeyPressed(K::W))
		direction.y += 1;
	if (K::isKeyPressed(K::S))
		direction.y += -1;

	if(!(direction.x == 0.f && direction.y == 0.f))
	{
		m_FacingDirection = direction;
	}

	m_Sprite.setTextureRect(calcSpriteFrame());
	//if(m_HasMoved)
	//{
	//	m_PreviousFacingDirection = m_FacingDirection;
	//	m_FacingDirection = {0, 0};
	//	m_HasMoved = false;
	//}
	
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	SceneObject::draw(target, states);
}

void Player::resetOrigin()
{
	m_Sprite.setOrigin(CHAR_TILE_WIDTH / 2.f, CHAR_TILE_HEIGHT - 88.f);
	//m_Sprite.setScale(0.75f, 0.75f);
}

sf::IntRect Player::calcSpriteFrame()
{
	sf::Vector2f vector = m_FacingDirection;

	// Default direction if all else fails
	//if (m_HasMoved)
	//{
	//	vector = m_FacingDirection;
	//}
	//else
	//{
	//	vector = m_PreviousFacingDirection;
	//}

	int index = 0;

	bool facingUp    = vector.y == 1.f;
	bool facingDown  = vector.y == -1.f;
	bool facingRight = vector.x == 1.f;
	bool facingLeft  = vector.x == -1.f;

	if (facingDown)
	{
		if (facingRight)
		{
			index = 2;
		}
		else if (facingLeft)
		{
			index = 0;
		}
		else
		{
			index = 1;
		}
	}
	else if (facingUp)
	{
		if (facingRight)
		{
			index = 4;
		}
		else if (facingLeft)
		{
			index = 6;
		}
		else
		{
			index = 5;
		}
	}
	else
	{
		if (facingRight)
		{
			index = 3;
		}
		else if (facingLeft)
		{
			index = 7;
		}
	}

	sf::IntRect result(0, 0, CHAR_TILE_WIDTH, CHAR_TILE_HEIGHT);

	result.left = index * CHAR_TILE_WIDTH;

	return result;
}
