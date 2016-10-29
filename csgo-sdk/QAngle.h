#pragma once

class QAngle {
public:
	float x, y, z;

	QAngle() {
		x = y = z = 0.f;
	}

	QAngle(float ix, float iy, float iz) {
		x = ix;
		y = iy;
		z = iz;
	}

	auto Init(float ix = 0.f, float iy = 0.f, float iz = 0.f) {
		x = ix;
		y = iy;
		z = iz;
	}

	auto Empty(float tol = 0.01f) const {
		return (x > -tol && x < tol && y > -tol && y < tol && z > -tol && z < tol);
	}

	auto& operator=(const QAngle& other) {
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}

	auto operator==(const QAngle& other) const {
		return (other.x == x) && (other.y == y) && (other.z == z);
	}

	auto operator!=(const QAngle& other) const {
		return (other.x != x) || (other.y != y) || (other.z != z);
	}

	auto& operator+=(const QAngle& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	auto& operator+=(float other) {
		x += other;
		y += other;
		z += other;
		return *this;
	}

	auto& operator-=(const QAngle& other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	auto& operator-=(float other) {
		x -= other;
		y -= other;
		z -= other;
		return *this;
	}

	auto& operator*=(float other) {
		x *= other;
		y *= other;
		z *= other;
		return *this;
	}

	auto& operator/=(float other) {
		x /= other;
		y /= other;
		z /= other;
		return *this;
	}

	auto& operator[](int i) {
		return ((float*)this)[i];
	}

	auto operator[](int i) const {
		return ((float*)this)[i];
	}

	inline float Dot(const QAngle& other) const {
		auto ang = *this;
		return (ang.x * other.x + ang.y * other.y + ang.z * other.z);
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

	auto operator-() const {
		return QAngle(-x, -y, -z);
	}

	auto operator+(const QAngle& other) const {
		QAngle ang;
		ang.x = x + other.x;
		ang.y = y + other.y;
		ang.z = z + other.z;
		return ang;
	}

	auto operator-(const QAngle& other) const {
		QAngle ang;
		ang.x = x - other.x;
		ang.y = y - other.y;
		ang.z = z - other.z;
		return ang;
	}

	auto operator*(float other) const {
		QAngle ang;
		ang.x = x * other;
		ang.y = y * other;
		ang.z = z * other;
		return ang;
	}

	auto operator/(float other) const {
		QAngle ang;
		ang.x = x / other;
		ang.y = y / other;
		ang.z = z / other;
		return ang;
	}

	auto Normalize() {
		auto x_rev = this->x / 360.f;
		if (this->x > 180.f || this->x < -180.f) {
			x_rev = abs(x_rev);
			x_rev = round(x_rev);

			if (this->x < 0.f)
				this->x = (this->x + 360.f * x_rev);

			else
				this->x = (this->x - 360.f * x_rev);
		}

		auto y_rev = this->y / 360.f;
		if (this->y > 180.f || this->y < -180.f) {
			y_rev = abs(y_rev);
			y_rev = round(y_rev);

			if (this->y < 0.f)
				this->y = (this->y + 360.f * y_rev);

			else
				this->y = (this->y - 360.f * y_rev);
		}

		auto z_rev = this->z / 360.f;
		if (this->z > 180.f || this->z < -180.f) {
			z_rev = abs(z_rev);
			z_rev = round(z_rev);

			if (this->z < 0.f)
				this->z = (this->z + 360.f * z_rev);

			else
				this->z = (this->z - 360.f * z_rev);
		}
	}

	auto Normalized() const {
		auto ang = *this;
		ang.Normalize();
		return ang;
	}

	auto Abs() const {
		return QAngle(abs(x), abs(y), abs(z));
	}

	auto Clamp() {
		while (this->x < -89.0f)
			this->x += 89.0f;

		if (this->x > 89.0f)
			this->x = 89.0f;

		while (this->y < -180.0f)
			this->y += 360.0f;

		while (this->y > 180.0f)
			this->y -= 360.0f;

		this->z = 0.0f;
	}
	
	auto Clamped() {
		QAngle tmp = *this;
		tmp.Clamp();
		return tmp;
	}

	float RealYawDifference(QAngle dst, QAngle add = QAngle());
	float Difference(QAngle dst, QAngle add);

	QAngle NormalizeYaw() {
		while (this->y > 180)
			this->y -= 360;

		while (this->y < -180)
			this->y += 360;

		return *this;
	}


};
