#pragma once
struct Segment {
	double _x1;
	double _y1;
	double _x2;
	double _y2;
	Segment(sf::Vector2f p1, sf::Vector2f p2) {
		_x1 = p1.x;
		_y1 = p1.y;
		_x2 = p2.x;
		_y2 = p2.y;
	}
};
bool intersect(Segment &s1, Segment &s2);
int shoot_ray(sf::Vector2f& p, sf::ConvexShape &object);
bool compare_dr(sf::Vector2f &p, Segment &s1, Segment &s2);