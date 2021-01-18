#pragma once
#include <SFML/Graphics.hpp>
#include "Calc.h"

class Projectile
{
private:
	double _x_pos;
	double _y_pos;
	double _vx;
	double _vy;
	double _force;
	double _rotation;

	sf::ConvexShape _sprite;

public:
	Projectile(double x, double y, double vx, double vy, double r, sf::ConvexShape s);
	~Projectile();

	double x_pos() { return _x_pos; }
	double y_pos() { return _y_pos; }
	double vx() { return _vx; }
	double vy() { return _vy; }
	double force() { return _force; }
	double rotation() { return _rotation; }

	void set_x_pos(double x) { _x_pos = x; }
	void set_y_pos(double y) { _y_pos = y; }
	void set_vx(double vx) { _vx = vx; }
	void set_vy(double vy) { _vy = vy; }
	void set_force(double f) { _force = f; }
	void set_rotation(double r) { _rotation = r; }

	sf::ConvexShape& sprite() { return _sprite; }

	void move(double dt);
	void accelerate(double dt);
	void turn(double dr, double dt);
	void teleport(sf::RenderWindow& window);
	void draw(sf::RenderWindow& window);

};

