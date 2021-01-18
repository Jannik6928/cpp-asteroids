#include <iostream>
#include <SFML/Graphics.hpp>
#include "RayShooting.h"

int shoot_ray(sf::Vector2f &p, sf::ConvexShape &object) {
	int intersections = 0;
	sf::Transform t;
	t = object.getTransform();
	Segment ray(p, sf::Vector2f(p.x + 10000, p.y));
	for (unsigned i = 0; i < object.getPointCount() - 1; i++) {
		Segment side(t.transformPoint(object.getPoint(i)),
					 t.transformPoint(object.getPoint(i + 1)));
		if (intersect(ray, side) == 1) intersections++;
	}
	Segment side(t.transformPoint(object.getPoint(0)),
				 t.transformPoint(object.getPoint(object.getPointCount() - 1)));
	if (intersect(ray, side) == 1) { intersections++; }
	return intersections;
}
bool intersect(Segment &s1, Segment &s2) {
	double m1;
	double m2;

	double dx1 = s1._x2 - s1._x1;
	double dx2 = s2._x2 - s2._x1;

	double dy1 = s1._y2 - s1._y1;
	double dy2 = s2._y2 - s2._y1;

	if (abs(dx1) < 1.0e-12 && abs(dx2) < 1.0e-12) { return 0; }
	else {
		if (abs(dx1) < 1.0e-12) {
			m2 = dy2 / dx2;
			double b2 = s2._y1 - m2 * s2._x1;
			double x_int = s1._x1;
			double y_int = b2 + m2 * x_int;
			sf::Vector2f p((float)x_int, (float)y_int);
			return compare_dr(p, s1, s2);
		}
		else if (abs(dx2) < 1.0e-12) {
			m1 = dy1 / dx1;
			double b1 = s1._y1 - m1 * s1._x1;
			double x_int = s2._x1;
			double y_int = b1 + m1 * x_int;
			sf::Vector2f p((float)x_int, (float)y_int);
			return compare_dr(p, s1, s2);
		}
		else {
			m1 = dy1 / dx1;
			m2 = dy2 / dx2;
			if (m1 - m2 != 0.0) {
				double b1 = s1._y1 - m1 * s1._x1;
				double b2 = s2._y1 - m2 * s2._x1;
				double x_int = (b2 - b1) / (m1 - m2);
				double y_int = m1 * x_int + b1;
				sf::Vector2f p((float)x_int, (float)y_int);
				return compare_dr(p, s1, s2);
			}
		}
	}
	return 0;
}

bool compare_dr(sf::Vector2f& p, Segment& s1, Segment& s2) {
	if ((p.x >= s1._x1 && p.x <= s1._x2) || (p.x <= s1._x1 && p.x >= s1._x2)) {
		if ((p.x >= s2._x1 && p.x <= s2._x2) || (p.x <= s2._x1 && p.x >= s2._x2)) {
			if ((p.y >= s1._y1 && p.y <= s1._y2) || (p.y <= s1._y1 && p.y >= s1._y2)) {
				if ((p.y >= s2._y1 && p.y <= s2._y2) || (p.y <= s2._y1 && p.y >= s2._y2)) {
					return 1;
				}
			}
		}
	}
	return 0;
}