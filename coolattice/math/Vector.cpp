#include "Vector.h"


Vector::Vector()
{
}

Vector::Vector(double xIn, double yIn) : x(xIn), y(yIn)
{
}

Vector& Vector::operator+=(const Vector& rhs)
{
	this->x += rhs.x;
	this->y += rhs.y;
	return *this;
}

Vector& Vector::operator-=(const Vector& rhs)
{
	this->x -= rhs.x;
	this->y -= rhs.y;
	return *this;
}

Vector& Vector::operator*=(double rhs)
{
	this->x *= rhs;
	this->y *= rhs;
	return *this;
}


