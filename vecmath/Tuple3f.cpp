#include "Tuple3f.h"
#include <cmath>

Tuple3f::Tuple3f() :x(0), y(0), z(0)
{
}

Tuple3f::Tuple3f(const float x, const float y, const float z) : x(x), y(y), z(z)
{
}

Tuple3f::Tuple3f(const Tuple3f& other) : x(other.x), y(other.y), z(other.z)
{
}

Tuple3f::~Tuple3f()
{
}

float Tuple3f::distance(const Tuple3f t1, const Tuple3f t2) {
	return sqrt(Tuple3f::distanceSquared(t1, t2));
}

float Tuple3f::distanceSquared(const Tuple3f t1, const Tuple3f t2) {
	Tuple3f vector = t1 - t2;
	return vector.x * vector.x + vector.y * vector.y + vector.z * vector.z;
}

Tuple3f Tuple3f::operator+ (const Tuple3f& rhs) const
{
	return Tuple3f(this->x + rhs.x, this->y + rhs.y, this->z + rhs.z);
}

Tuple3f Tuple3f::operator- (const Tuple3f& rhs) const {
	return Tuple3f(this->x - rhs.x, this->y - rhs.y, this->z - rhs.z);
}

Tuple3f Tuple3f::operator* (const float& rhs) const {
	return Tuple3f(this->x * rhs, this->y * rhs, this->z * rhs);
}

Tuple3f Tuple3f::operator/ (const float& rhs) const {
	return Tuple3f(this->x / rhs, this->y / rhs, this->z / rhs);
}

bool Tuple3f::operator== (Tuple3f& rhs) const {
	return this->x == rhs.x && this->y == rhs.y && this->z == rhs.z;
}

bool Tuple3f::operator!= (Tuple3f& rhs) const {
	return !((*this) == rhs);
}

bool Tuple3f::operator< (const Tuple3f& rhs) const {
	return this->x < rhs.x || (this->x == rhs.x && this->y < rhs.y) || (this->x == rhs.x && this->y == rhs.y && this->z < rhs.z);
}