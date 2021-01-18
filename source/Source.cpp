#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>
#include "Calc.h"
#include "RayShooting.h"
#include "Projectile.h"
#include <sstream>

void define_sprites();
sf::ConvexShape craft_sprite;
sf::ConvexShape craft_flame_sprite;
sf::ConvexShape shrapnel_sprite;
sf::ConvexShape bullet_sprite;
sf::ConvexShape asteroid_sprite1;
sf::ConvexShape asteroid_sprite2;
sf::ConvexShape asteroid_sprite3;

int main()
{	
	sf::Font sansation;
	sansation.loadFromFile("sansation.ttf");
	sf::Text lives_label;
	lives_label.setFont(sansation);
	lives_label.setCharacterSize(24);
	sf::Text dead_label;
	dead_label.setFont(sansation);
	dead_label.setCharacterSize(120);

	sf::VideoMode desktop;
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(desktop.getDesktopMode().width * 3 / 4, desktop.getDesktopMode().height * 3 / 4), "Asteroids!", sf::Style::Titlebar | sf::Style::Close, settings);

	window.setFramerateLimit(144);
	window.setKeyRepeatEnabled(false);
	std::srand((int)std::time(0));

	define_sprites();

	Projectile craft((double)window.getSize().x / 2, (double)window.getSize().y / 2, 0, 0, 0, craft_sprite);
	Projectile craft_flame(0, 0, 0, 0, 0, craft_flame_sprite);
	std::vector <Projectile> craft_shrapnelv;
	std::vector <double> craft_shrapneld;

	std::vector <Projectile> bulletv;
	std::vector <double> bulletd;

	std::vector <Projectile> asteroidv;
	std::vector <int> asteroidt;

	sf::Clock clock;

	int lives = 4;
	double time_alive = 0;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window.close();
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Space && lives > 0) {
					Projectile bullet(craft.x_pos() + sin(d2r(craft.rotation())) * 8, craft.y_pos() - cos(d2r(craft.rotation())) * 8, 
									  sin(d2r(craft.rotation())) * 8 + craft.vx(), 0 - cos(d2r(craft.rotation())) * 8 + craft.vy(), 
									  craft.rotation(), bullet_sprite);
					bulletv.push_back(bullet);
					bulletd.push_back(0);
				}
				if (event.key.code == sf::Keyboard::W) craft.set_force(0.2);
			}
			if (event.type == sf::Event::KeyReleased) {
				if (event.key.code == sf::Keyboard::W) craft.set_force(0);
			}
		}

		double elapsed = clock.getElapsedTime().asMilliseconds();
		clock.restart();

		window.clear();

		for (unsigned i = 0; i < bulletv.size(); i++) {
			bulletv[i].move(elapsed / 20);
			bulletv[i].teleport(window);
			bulletv[i].draw(window);
			bulletd[i] += elapsed / 20;
		}
		for (unsigned i = 0; i < bulletv.size(); i++) {
			if (bulletd[i] > 64) {
				bulletv.erase(bulletv.begin() + i);
				bulletd.erase(bulletd.begin() + i);
			}
		}

		if (std::rand() / double(RAND_MAX) < elapsed / 8000 && lives > 0) {
			double a = std::rand() % 360;
			double b = std::rand() % 2;
			sf::ConvexShape temp = asteroid_sprite1;
			Projectile asteroid(0, 0, (b + 1) * sin(d2r(a)), 0 - (b + 1) * cos(d2r(a)), a, temp);
			asteroidv.push_back(asteroid);
			asteroidt.push_back(1);
		}
		for (unsigned i = 0; i < asteroidv.size(); i++) {
			asteroidv[i].move(elapsed / 20);
			asteroidv[i].teleport(window);
			asteroidv[i].draw(window);
		}

		for (unsigned i = 0; i < bulletv.size(); i++) {
			for (unsigned j = 0; j < asteroidv.size(); j++) {
				if (bulletv[i].sprite().getGlobalBounds().intersects(asteroidv[j].sprite().getGlobalBounds())) {
					sf::Vector2f p((float)bulletv[i].x_pos(), (float)bulletv[i].y_pos());
					if (shoot_ray(p, asteroidv[j].sprite()) % 2 == 1) {
						for (int k = 0; k < int(std::rand() % 3 * (4 - asteroidt[j])) + 6; k++) {
							double r = std::rand() % 360;
							double rv = std::rand() % 1000;
							double v = sqrt(pow(asteroidv[j].vx(), 2) + pow(asteroidv[j].vy(), 2));
							Projectile asteroid_shrapnel(asteroidv[j].x_pos(), asteroidv[j].y_pos(), rv * v * sin(d2r(r)) / 400, rv * v * (0 - cos(d2r(r))) / 400,
														 r, shrapnel_sprite);
							craft_shrapnelv.push_back(asteroid_shrapnel);
							craft_shrapneld.push_back(0);
						}
						if (asteroidt[j] != 3) {
							for (int k = 0; k < std::rand() % 2 + 2; k++) {
								double a = std::rand() % 360;
								double b = std::rand() % 2;
								sf::ConvexShape temp;
								if (asteroidt[j] == 1) { temp = asteroid_sprite2; }
								else if (asteroidt[j] == 2) { temp = asteroid_sprite3; }
								Projectile asteroid(asteroidv[j].x_pos(),
									asteroidv[j].y_pos(),
									(b + 1) * sin(d2r(a)), 0 - (b + 1) * cos(d2r(a)), a, temp);
								asteroidt.push_back(asteroidt[j] + 1);
								asteroidv.push_back(asteroid);
							}
						}
						asteroidv.erase(asteroidv.begin() + j);
						asteroidt.erase(asteroidt.begin() + j);
  						bulletv.erase(bulletv.begin() + i);
						bulletd.erase(bulletd.begin() + i);
						break; 
					}
				}
			}
		}
		for (unsigned i = 0; i < asteroidv.size(); i++) {
			if (asteroidv[i].sprite().getGlobalBounds().intersects(craft.sprite().getGlobalBounds()) && time_alive > 5000 && lives > 0) {
				for (unsigned j = 0; j < craft.sprite().getPointCount(); j++) {
					sf::Transform t;
					t = craft.sprite().getTransform();
					sf::Vector2f p(craft.sprite().getPoint(j).x, craft.sprite().getPoint(j).y);
					sf::Vector2f pf = t.transformPoint(p);
					if (shoot_ray(pf, asteroidv[i].sprite()) % 2 == 1) {
						time_alive = 0;
						int r = int(floor(craft.rotation())) + 90;
						for (int j = 0; j < int(std::rand() % 9) + 12; j++) {
							double r1 = std::rand() % 180;
							double rv = std::rand() % 1000;
							double v = sqrt(pow(craft.vx(), 2) + pow(craft.vy(), 2));
							Projectile craft_shrapnel(craft.x_pos(), craft.y_pos(), rv * v * sin(d2r(r1 + r)) / 1000, rv * v * (0 - cos(d2r(r1 + r))) / 1000,
								r1 + r, shrapnel_sprite);
							craft_shrapnelv.push_back(craft_shrapnel);
							craft_shrapneld.push_back(0);
						}
						craft.set_x_pos((double)window.getSize().x / 2);
						craft.set_y_pos((double)window.getSize().y / 2);
						craft.set_vx(0);
						craft.set_vy(0);
						craft.set_rotation(0);
						lives--;
						break;
					}
				}
			}
		}

		for (unsigned i = 0; i < craft_shrapnelv.size(); i++) {
			craft_shrapnelv[i].move(elapsed / 20);
			craft_shrapnelv[i].draw(window);
			craft_shrapneld[i] += elapsed / 20;
		}
		for (unsigned i = 0; i < craft_shrapnelv.size(); i++) {
			if (craft_shrapneld[i] > 64) {
				craft_shrapnelv.erase(craft_shrapnelv.begin() + i);
				craft_shrapneld.erase(craft_shrapneld.begin() + i);
			}
		}

		std::ostringstream ss;
		ss << lives;
		lives_label.setString("lives: " + ss.str());
		lives_label.setPosition(25, (float)window.getSize().y - 50);
		window.draw(lives_label);

		if (!(lives > 0)) {
			dead_label.setString("Game Over!");
			dead_label.setPosition(window.getSize().x / 2 - dead_label.getGlobalBounds().width / 2,
				window.getSize().y / 2 - dead_label.getGlobalBounds().height / 2);
			window.draw(dead_label);
		} else {
			craft.accelerate(elapsed / 20);
			craft.move(elapsed / 20);
			craft.teleport(window);
			if (time_alive > 5000 || std::rand() % 3 != 2) { craft.draw(window); }
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) craft.turn(-3, elapsed / 20);
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) craft.turn(3, elapsed / 20);
			if (craft.force() > 0) {
				if (std::rand() % 5 != 4) {
					craft_flame.set_x_pos(craft.x_pos());
					craft_flame.set_y_pos(craft.y_pos());
					craft_flame.set_rotation(craft.rotation());
					craft_flame.draw(window);
				}
			}
		}

		time_alive += elapsed;

		window.display();
	}

	return 0;
}

void define_sprites() {
	craft_sprite.setPointCount(4);
	craft_sprite.setPoint(0, sf::Vector2f(0, 0));
	craft_sprite.setPoint(1, sf::Vector2f(-12, 32));
	craft_sprite.setPoint(2, sf::Vector2f(0, 24));
	craft_sprite.setPoint(3, sf::Vector2f(12, 32));
	craft_sprite.setOrigin(sf::Vector2f(0, 16));

	bullet_sprite.setPointCount(3);
	bullet_sprite.setPoint(0, sf::Vector2f(0, 5));
	bullet_sprite.setPoint(1, sf::Vector2f(-2, 0));
	bullet_sprite.setPoint(2, sf::Vector2f(2, 0));
	bullet_sprite.setOrigin(sf::Vector2f(0, 2));

	asteroid_sprite1.setPointCount(11);
	asteroid_sprite1.setPoint(0, sf::Vector2f(24, 0));
	asteroid_sprite1.setPoint(1, sf::Vector2f(60, 0));
	asteroid_sprite1.setPoint(2, sf::Vector2f(72, 24));
	asteroid_sprite1.setPoint(3, sf::Vector2f(96, 36));
	asteroid_sprite1.setPoint(4, sf::Vector2f(96, 72));
	asteroid_sprite1.setPoint(5, sf::Vector2f(60, 84));
	asteroid_sprite1.setPoint(6, sf::Vector2f(48, 108));
	asteroid_sprite1.setPoint(7, sf::Vector2f(12, 108));
	asteroid_sprite1.setPoint(8, sf::Vector2f(0, 72));
	asteroid_sprite1.setPoint(9, sf::Vector2f(24, 60));
	asteroid_sprite1.setPoint(10, sf::Vector2f(0, 36));
	asteroid_sprite1.setOrigin(asteroid_sprite1.getGlobalBounds().width / 2, asteroid_sprite1.getGlobalBounds().height / 2);

	asteroid_sprite2.setPointCount(9);
	asteroid_sprite2.setPoint(0, sf::Vector2f(12, 12));
	asteroid_sprite2.setPoint(1, sf::Vector2f(48, 0));
	asteroid_sprite2.setPoint(2, sf::Vector2f(66, 18));
	asteroid_sprite2.setPoint(3, sf::Vector2f(72, 48));
	asteroid_sprite2.setPoint(4, sf::Vector2f(60, 72));
	asteroid_sprite2.setPoint(5, sf::Vector2f(36, 84));
	asteroid_sprite2.setPoint(6, sf::Vector2f(12, 72));
	asteroid_sprite2.setPoint(7, sf::Vector2f(24, 60));
	asteroid_sprite2.setPoint(8, sf::Vector2f(0, 48));
	asteroid_sprite2.setOrigin(asteroid_sprite2.getGlobalBounds().width / 2, asteroid_sprite2.getGlobalBounds().height / 2);

	asteroid_sprite3.setPointCount(9);
	asteroid_sprite3.setPoint(0, sf::Vector2f(8, 0));
	asteroid_sprite3.setPoint(1, sf::Vector2f(20, 0));
	asteroid_sprite3.setPoint(2, sf::Vector2f(28, 8));
	asteroid_sprite3.setPoint(3, sf::Vector2f(36, 8));
	asteroid_sprite3.setPoint(4, sf::Vector2f(40, 24));
	asteroid_sprite3.setPoint(5, sf::Vector2f(36, 32));
	asteroid_sprite3.setPoint(6, sf::Vector2f(16, 40));
	asteroid_sprite3.setPoint(7, sf::Vector2f(4, 32));
	asteroid_sprite3.setPoint(8, sf::Vector2f(0, 16));
	asteroid_sprite3.setOrigin(asteroid_sprite3.getGlobalBounds().width / 2, asteroid_sprite3.getGlobalBounds().height / 2);

	craft_flame_sprite.setPointCount(3);
	craft_flame_sprite.setPoint(0, sf::Vector2f(0, 24));
	craft_flame_sprite.setPoint(1, sf::Vector2f(-6, 6));
	craft_flame_sprite.setPoint(2, sf::Vector2f(6, 6));

	shrapnel_sprite.setPointCount(3);
	shrapnel_sprite.setPoint(0, sf::Vector2f(0, 4));
	shrapnel_sprite.setPoint(1, sf::Vector2f(float(0 - sqrt(3)) * 2, -2));
	shrapnel_sprite.setPoint(2, sf::Vector2f(float(sqrt(3)) * 2, -2));
}