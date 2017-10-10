#pragma once

struct Vector
{
public:
	double x;
	double y;

public:
	Vector();
	Vector(double x, double y);
	
	inline static double dotProduct(const Vector& a, const Vector& b) { return a.x * b.x + a.y * b.y; }

	Vector& operator+=(const Vector& rhs);
	Vector& operator-=(const Vector& rhs);
	Vector& operator*=(double rhs);


	Vector& Vector::operator=(Vector rhs)
	{
		//TODO: write the correct swap idiom please :P
		//std::swap(rhs);
		this->x = rhs.x;
		this->y = rhs.y;
		return *this;
	}

};

// addition and substraction

inline Vector operator+(Vector lhs, const Vector& rhs)
{
	lhs += rhs;
	return lhs;
}

inline Vector operator-(Vector lhs, const Vector& rhs)
{
	lhs -= rhs;
	return lhs;
}

inline Vector operator*(Vector lhs, double rhs)
{
	lhs *= rhs;
	return lhs;
}

