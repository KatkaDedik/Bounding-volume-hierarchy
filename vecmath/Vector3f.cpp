#include "Vector3f.h"
#include <cmath>

Vector3f::Vector3f() : Tuple3f(0,0,0)
{
}
Vector3f::Vector3f(const float x, const float y, const float z) : Tuple3f(x, y, z)
{
}
Vector3f::Vector3f(const Tuple3f& other) : Tuple3f(other)
{
}
Vector3f::~Vector3f()
{
}

void Vector3f::Normalize()
{
	const float magnitude = this->Magnitude();
	this->x = this->x / magnitude;
	this->y = this->y / magnitude;
	this->z = this->z / magnitude;
}

float Vector3f::Magnitude() const
{
	return std::sqrt(this->x*this->x + this->y*this->y + this->z * this->z);
}

float Vector3f::Dot(const Vector3f& other) const
{
	return this->x*other.x + this->y*other.y + this->z*other.z;
}

Vector3f Vector3f::Cross(const Vector3f& other) const
{
	float a = this->y * other.z - this->z * other.y;
	float b = this->z * other.x - this->x * other.z;
	float c = this->x * other.y - this->y * other.x;
	return Vector3f(a, b, c);
}

Vector3f Vector3f::Rotate(const float theta, Vector3f axis) const
{
	axis.Normalize();

	const float xPrime = axis.x * (axis.x * x + axis.y * y + axis.z * z) * (1 - cos(theta))
		+ x * cos(theta)
		+ (-axis.z * y + axis.y * z) * sin(theta);
	const float yPrime = axis.y * (axis.x * x + axis.y * y + axis.z * z) * (1 - cos(theta))
		+ y * cos(theta)
		+ (axis.z * x - axis.x * z) * sin(theta);
	const float zPrime = axis.z * (axis.x * x + axis.y * y + axis.z * z) * (1 - cos(theta))
		+ z * cos(theta)
		+ (-axis.y * x + axis.x * y) * sin(theta);

	return Vector3f(xPrime, yPrime, zPrime);
}

