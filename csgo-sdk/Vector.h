#pragma once

class Vector {
public:
	float x, y, z;

	Vector() {
		x = y = z = 0.f;
	}

	Vector(float ix, float iy, float iz) {
		x = ix;
		y = iy;
		z = iz;
	}

	auto Init(float ix = 0.f, float iy = 0.f, float iz = 0.f) {
		x = ix;
		y = iy;
		z = iz;
	}

	auto LengthSqr() const {
		return (x * x + y * y + z * z);
	}

	auto Length2DSqr() const {
		return (x * x + y * y);
	}

	auto Length() const {
		return sqrt(LengthSqr());
	}

	auto Length2D() const {
		return sqrt(Length2DSqr());
	}

	auto Empty(float tol = 0.01f) const {
		return (x > -tol && x < tol && y > -tol && y < tol && z > -tol && z < tol);
	}

	auto Zero() {
		x = y = z = 0.f;
	}

	auto& operator=(const Vector& other) {
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}

	auto& operator[](int idx) {
		return ((float*)this)[idx];
	}

	auto operator[](int i) const {
		return reinterpret_cast<float*>(const_cast<Vector*>(this))[i];
	}

	auto operator==(const Vector& other) const {
		return (other.x == x) && (other.y == y) && (other.z == z);
	}

	auto operator!=(const Vector& other) const {
		return (other.x != x) || (other.y != y) || (other.z != z);
	}

	auto& operator+=(const Vector& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	auto& operator-=(const Vector& other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	auto& operator*=(float other) {
		x *= other;
		y *= other;
		z *= other;
		return *this;
	}

	auto& operator*=(const Vector& other) {
		x *= other.x;
		y *= other.y;
		z *= other.z;
		return *this;
	}

	auto& operator+=(float other) {
		x += other;
		y += other;
		z += other;
		return *this;
	}

	auto& operator-=(float other) {
		x -= other;
		y -= other;
		z -= other;
		return *this;
	}

	auto& operator/=(float other) {
		const float fucker = 1.f / other;
		x *= fucker;
		y *= fucker;
		z *= fucker;
		return *this;
	}

	auto& operator/=(const Vector& other) {
		x /= other.x;
		y /= other.y;
		z /= other.z;
		return *this;
	}

	auto operator+(const Vector& other) const {
		Vector vec;
		vec.x = x + other.x;
		vec.y = y + other.y;
		vec.z = z + other.z;
		return vec;
	}

	auto operator-(const Vector& other) const {
		Vector vec;
		vec.x = x - other.x;
		vec.y = y - other.y;
		vec.z = z - other.z;
		return vec;
	}

	auto operator*(float other) const {
		Vector vec;
		vec.x = x * other;
		vec.y = y * other;
		vec.z = z * other;
		return vec;
	}

	auto operator*(const Vector& other) const {
		Vector vec;
		vec.x = x * other.x;
		vec.y = y * other.y;
		vec.z = z * other.z;
		return vec;
	}

	auto operator/(float other) const {
		Vector vec;
		vec.x = x / other;
		vec.y = y / other;
		vec.z = z / other;
		return vec;
	}

	auto operator/(const Vector& other) const {
		Vector vec;
		vec.x = x / other.x;
		vec.y = y / other.y;
		vec.z = z / other.z;
		return vec;
	}

	auto Dot(const Vector& other) const {
		const auto vec = *this;
		return (vec.x * other.x + vec.y * other.y + vec.z * other.z);
	}

	auto Dot(const float* other) const {
		const auto vec = *this;
		return (vec.x * other[0] + vec.y * other[1] + vec.z * other[2]);
	}

	auto Normalize() {
		(*this) /= Length();
	}

	auto Normalized() const {
		auto vec = *this;
		vec.Normalize();
		return vec;
	}

	auto NormalizeInPlace() {
		Vector& v = *this;

		const float iradius = 1.f / (this->Length() + FLT_EPSILON);

		v.x *= iradius;
		v.y *= iradius;
		v.z *= iradius;
		return iradius;
	}

	auto MultAdd(const Vector& vOther, float fl) {
		x += fl * vOther.x;
		y += fl * vOther.y;
		z += fl * vOther.z;
	}
};

class __declspec(align(16)) VectorAligned : public Vector {
public:
	VectorAligned() {}

	VectorAligned(const Vector &vec) {
		this->x = vec.x;
		this->y = vec.y;
		this->z = vec.z;
		this->w = 0.f;
	}

	float w = 0;
};
