#include "Vector3.h"

void Vector3::Normalize()
{
	FLOAT length = Magnitude();
	if (length == 0.0f) return;
	
	(*this) *=  1/ length;
}

FLOAT Vector3::Magnitude()
{
	return sqrt(x * x + y * y + z * z);
}

Vector3 Vector3::operator*(FLOAT s)
{
	Vector3 vector;
	vector.x = x * s;
	vector.y = y * s;
	vector.z = z * s;
	return vector;
}

void Vector3::operator*=(FLOAT s)
{
	x *= s;
	y *= s;
	z *= s;
}

Vector3 Vector3::operator+(Vector3 vector)
{
	Vector3 vector2;
	vector2.x = x * vector.x;
	vector2.y = y * vector.y;
	vector2.z = z * vector.z;
	return vector;
}

void Vector3::operator+=(Vector3 vector)
{
	x += vector.x;
	y += vector.y;
	z += vector.z;
}


