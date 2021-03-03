#pragma once

#include <SFML/System.hpp>
#include <cmath>
#include <iostream>
#include <vector>

namespace sf {
	//Create a definition for a sf::vector using size_t types
	//Vector 2 stores x and y, where x is the width (in number of tiles)
	//and y is the height (in number of tiles). For example, 8x5. 8 tiles by 5 tiles.
	typedef Vector2<size_t> Vector2ul;
	//Returns the length of a sf::vector
	template <typename T> double length(const Vector2<T>& v) {
		return sqrt(v.x * v.x + v.y * v.y);
	}
	//return normalized sf::vector
	template <typename T> Vector2<T> normalize(const Vector2<T>& v) {
		Vector2<T> vector;
		double l = length(v);
		if (l != 0) {
			vector.x = v.x / l;
			vector.y = v.y / l;
		}
		return vector;
	}
	//Allow casting from one sf::vector internal type to another
	template <typename T, typename U> Vector2<T> Vcast(const Vector2<U>& v) {
		return Vector2<T>(static_cast<T>(v.x), static_cast<T>(v.y));
	}
	//Degrees to radians conversion. atan(1)*4 is the PI number
	static double deg2rad(double degrees) {
		return ((atan(1) * 4) / 180) * degrees;
	}
	//Rotate a sf::vector by an angle(degrees)
	template <typename T>
	Vector2<T> rotate(const Vector2<T>& v, const double degrees) {
		const double theta = deg2rad(degrees);
		const double cs = cos(theta);
		const double sn = sin(theta);
		return { (T)(v.x * cs - v.y * sn), (T)(v.x * sn + v.y * cs) };
	}
	//Allow sf::vectors to be cout'ed
	template <typename T>
	std::ostream& operator<<(std::ostream& os, const Vector2<T>& v) {
		os << '(' << v.x << ',' << v.y << ')';
		return os;
	}
}