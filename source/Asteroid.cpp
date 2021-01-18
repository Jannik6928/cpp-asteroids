#include "Asteroid.h"
#include "Ship.h"
#include <SFML/Graphics.hpp>
#include "Calc.h"
#include "iostream"

void Asteroid::draw(sf::RenderWindow &window)
{
	for (unsigned i = 0; i < _spritev.size(); i++) {
		_spritev[i].setOrigin(_spritev[i].getGlobalBounds().height / 2, _spritev[i].getGlobalBounds().height / 2);
		_spritev[i].setPosition((float)_x_pos[i], (float)_y_pos[i]);
		_spritev[i].setRotation((float)_direction[i]);
		window.draw(_spritev[i]);
	}
}

void Asteroid::spawn()
{
	double r = (double)rand() / RAND_MAX;
	if (r > 0.999) {
		for (int a = 0; a < 3; a++) {
			_x_pos.push_back(0);
			_y_pos.push_back(0);
			_direction.push_back((double)rand() / RAND_MAX * 360);
			_velocity.push_back((double)rand() / RAND_MAX * 6 + 0.1f);
			_type.push_back(1);

			_sprite.setPointCount(11);
			_sprite.setPoint(0, sf::Vector2f(24, 0));
			_sprite.setPoint(1, sf::Vector2f(60, 0));
			_sprite.setPoint(2, sf::Vector2f(72, 24));
			_sprite.setPoint(3, sf::Vector2f(96, 36));
			_sprite.setPoint(4, sf::Vector2f(96, 72));
			_sprite.setPoint(5, sf::Vector2f(60, 84));
			_sprite.setPoint(6, sf::Vector2f(48, 108));
			_sprite.setPoint(7, sf::Vector2f(12, 108));
			_sprite.setPoint(8, sf::Vector2f(0, 72));
			_sprite.setPoint(9, sf::Vector2f(24, 60));
			_sprite.setPoint(10, sf::Vector2f(0, 36));

			_spritev.push_back(_sprite);

		}
	}
}

void Asteroid::fly()
{
	for (unsigned i = 0; i < _x_pos.size(); i++) {
		_x_pos[i] += sin(d2r(_direction[i]) * _velocity[i]);
		_y_pos[i] -= cos(d2r(_direction[i]) * _velocity[i]);
	}
}

void Asteroid::teleport(sf::RenderWindow &window)
{
	for (unsigned a = 0; a < _x_pos.size(); a++) {
		if (_x_pos[a] > (double)window.getSize().x + 150) _x_pos[a] = -150;
		if (_y_pos[a] > (double)window.getSize().y + 150) _y_pos[a] = -150;
		if (_x_pos[a] < -150) _x_pos[a] = (double)window.getSize().x + 150;
		if (_y_pos[a] < -150) _y_pos[a] = (double)window.getSize().y + 150;
	}
}

void Asteroid::collide(Bullet& bullet, Ship& ship, bool& lose) {
	for (unsigned b = 0; b < _spritev.size(); b++) {
		for (unsigned a = 0; a < bullet.spritev().size(); a++) {
			if (bullet.spritev()[a].getGlobalBounds().intersects(_spritev[b].getGlobalBounds())) {
				bullet.remove(a);
				split(b);
				erase(b);
			}
		}
		if (_spritev[b].getGlobalBounds().intersects(ship.sprite().getGlobalBounds())) lose = true;
	}
	
}

void Asteroid::update(sf::RenderWindow &window, Bullet &bullet, Ship &ship, bool& lose) {
	spawn();
	collide(bullet, ship, lose);
	teleport(window);
	fly();
	draw(window);
}

void Asteroid::split(unsigned i) {
	double r = (double)rand() / RAND_MAX + 1.5;
	r = ceil(r);
	for (int a = 0; a != r; a++) {
		if (_type[i] != 3) {
			_direction.push_back(360.f * rand() / RAND_MAX);
			_x_pos.push_back(_x_pos[i]);
			_y_pos.push_back(_y_pos[i]);
			_velocity.push_back(6.f * rand() / RAND_MAX + 0.1f);
			if (_type[i] == 1) {
				_type.push_back(2);

				_sprite.setPointCount(9);
				_sprite.setPoint(0, sf::Vector2f(12, 12));
				_sprite.setPoint(1, sf::Vector2f(48, 0));
				_sprite.setPoint(2, sf::Vector2f(66, 18));
				_sprite.setPoint(3, sf::Vector2f(72, 48));
				_sprite.setPoint(4, sf::Vector2f(60, 72));
				_sprite.setPoint(5, sf::Vector2f(36, 84));
				_sprite.setPoint(6, sf::Vector2f(12, 72));
				_sprite.setPoint(7, sf::Vector2f(24, 60));
				_sprite.setPoint(8, sf::Vector2f(0, 48));

				_spritev.push_back(_sprite);
			}
			else if (_type[i] == 2) {
				_type.push_back(3);

				_sprite.setPointCount(9);
				_sprite.setPoint(0, sf::Vector2f(8, 0));
				_sprite.setPoint(1, sf::Vector2f(20, 0));
				_sprite.setPoint(2, sf::Vector2f(28, 8));
				_sprite.setPoint(3, sf::Vector2f(36, 8));
				_sprite.setPoint(4, sf::Vector2f(40, 24));
				_sprite.setPoint(5, sf::Vector2f(36, 32));
				_sprite.setPoint(6, sf::Vector2f(16, 40));
				_sprite.setPoint(7, sf::Vector2f(4, 32));
				_sprite.setPoint(8, sf::Vector2f(0, 16));

				_spritev.push_back(_sprite);
			}
		}
	}
}

void Asteroid::erase(unsigned i) {
	_spritev.erase(_spritev.begin() + i);
	_direction.erase(_direction.begin() + i);
	_x_pos.erase(_x_pos.begin() + i);
	_y_pos.erase(_y_pos.begin() + i);
	_type.erase(_type.begin() + i);
	_velocity.erase(_velocity.begin() + i);
}