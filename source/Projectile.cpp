#include "Projectile.h"
#include "Calc.h"
#include <SFML/Graphics.hpp>
#include <iostream>

Projectile::Projectile(double x, double y, double vx, double vy, double r, sf::ConvexShape s)
{
	_x_pos = x;
	_y_pos = y;
	_vx = vx;
	_vy = vy;
	_force = 0;
	_rotation = r;
	_sprite = s;
}

Projectile::~Projectile()
{
}

void Projectile::accelerate(double dt) {
	_vx += sin(d2r(_rotation)) * _force * dt;
	_vy += 0 - cos(d2r(_rotation)) * _force * dt;
	_vx *= pow(0.995, dt * 60 / 50); // TIME ELAPSED INCORPORATION
	_vy *= pow(0.995, dt * 60 / 50);
}

void Projectile::move(double dt) {
	_x_pos += _vx * dt;
	_y_pos += _vy * dt;
}

void Projectile::turn(double dr, double dt) {
	_rotation += dr * dt;
	if (_rotation > 360) _rotation -= 360;
	if (_rotation < 0) _rotation += 360;
}

void Projectile::teleport(sf::RenderWindow &window) {
	if (!(_sprite.getGlobalBounds().intersects(sf::FloatRect(0, 0, (float)window.getSize().x, (float)window.getSize().y)))) {
		if (_x_pos > (double)window.getSize().x) _x_pos = 0;
		else if (_x_pos < 0) _x_pos = (double)window.getSize().x;
		if (_y_pos > (double)window.getSize().y) _y_pos = 0;
		else if (_y_pos < 0) _y_pos = (double)window.getSize().y;
	}
}

void Projectile::draw(sf::RenderWindow& window) {
	_sprite.setPosition((float)_x_pos, (float)_y_pos);
	_sprite.setRotation((float)_rotation);
	window.draw(_sprite);
}