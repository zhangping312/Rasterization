#include <Math/Vectors.h>
#include <Logger/Logger.h>

#include <math.h>
#include <cfloat>

namespace FW::Math
{
	#pragma region Vector2
	Vector2& Vector2::operator=(const Vector2& rhs)
	{
		x = rhs.x; y = rhs.y;
		return *this;
	}

	float Vector2::LengthSquare() const
	{
		return Dot(*this, *this);
	}

	float Vector2::Length() const
	{
		float ls = LengthSquare();
		if (ls <= FLT_EPSILON) return 0.0f;
		else return sqrtf(ls);
	}

	void Vector2::Normalize()
	{
		float ls = LengthSquare();
		if (ls <= FLT_EPSILON) return;

		float rl = 1.0f / sqrt(ls);
		x *= rl; y *= rl;
	}

	Vector2& Vector2::operator+=(const Vector2& rhs)
	{
		x += rhs.x; y += rhs.y;
		return *this;
	}

	Vector2& Vector2::operator-=(const Vector2& rhs)
	{
		x -= rhs.x; y -= rhs.y;
		return *this;
	}

	Vector2& Vector2::operator*=(float s)
	{
		x *= s; y *= s;
		return *this;
	}

	Vector2& Vector2::operator/=(float s)
	{
		float rs = 1.0f / s;
		x *= rs; y *= rs;
		return *this;
	}

	bool operator==(const Vector2& lhs, const Vector2& rhs)
	{
		return (lhs - rhs).LengthSquare() <= FLT_EPSILON;
	}

	bool operator!=(const Vector2& lhs, const Vector2& rhs)
	{
		return !(lhs == rhs);
	}

	Vector2 operator+(const Vector2& lhs, const Vector2& rhs)
	{
		return Vector2(lhs.x + rhs.x, lhs.y + rhs.y);
	}

	Vector2 operator-(const Vector2& lhs, const Vector2& rhs)
	{
		return Vector2(lhs.x - rhs.x, lhs.y - rhs.y);
	}

	Vector2 operator*(float s, const Vector2& v)
	{
		return Vector2(s * v.x, s * v.y);
	}

	Vector2 operator*(const Vector2& v, float s)
	{
		return Vector2(s * v.x, s * v.y);
	}

	Vector2 operator/(const Vector2& v, float s)
	{
		float rs = 1.0f / s;
		return Vector2(rs * v.x, rs * v.y);
	}

	Vector2 Vector2::Normalize(const Vector2& v)
	{
		auto result = v;
		result.Normalize();
		return result;
	}

	float Vector2::Dot(const Vector2& lhs, const Vector2& rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y;
	}

	float Vector2::Angle(const Vector2& lhs, const Vector2& rhs)
	{
		auto ll = lhs.LengthSquare();
		auto rl = rhs.LengthSquare();
		LOG_ASSERT(ll > 0 && rl > 0, "Have 0 length vector!")

		float c = Dot(lhs, rhs) / sqrtf(ll * rl);
		return acosf(c);
	}

	Vector2 Vector2::Project(const Vector2& v, const Vector2& dir)
	{
		return Dot(v, dir) * dir;
	}

	Vector2 Vector2::Perpendicular(const Vector2& v, const Vector2& dir)
	{
		return v - Project(v, dir);
	}

	Vector2 Vector2::Reflection(const Vector2& v, const Vector2& normal)
	{
		return v + 2.0f * Project(-v, normal);
	}

	float Vector2::Cross(const Vector2& lhs, const Vector2& rhs)
	{
		return lhs.x * rhs.y - lhs.y * rhs.x;
	}

	float Vector2::Distance(const Vector2& lhs, const Vector2& rhs)
	{
		return (lhs - rhs).Length();
	}
	#pragma endregion

	#pragma region Vector3
	Vector3& Vector3::operator=(const Vector3& rhs)
	{
		x = rhs.x; y = rhs.y; z = rhs.z;
		return *this;
	}

	float Vector3::LengthSquare() const
	{
		return Dot(*this, *this);
	}

	float Vector3::Length() const
	{
		float ls = LengthSquare();
		if (ls <= FLT_EPSILON) return 0.0f;
		else return sqrtf(ls);
	}

	void Vector3::Normalize()
	{
		float ls = LengthSquare();
		if (ls <= FLT_EPSILON) return;

		float rl = 1.0f / sqrt(ls);
		x *= rl; y *= rl; z *= rl;
	}

	Vector3& Vector3::operator+=(const Vector3& rhs)
	{
		x += rhs.x; y += rhs.y; z += rhs.z;
		return *this;
	}

	Vector3& Vector3::operator-=(const Vector3& rhs)
	{
		x -= rhs.x; y -= rhs.y; z -= rhs.z;
		return *this;
	}

	Vector3& Vector3::operator*=(float s)
	{
		x *= s; y *= s; z *= s;
		return *this;
	}

	Vector3& Vector3::operator/=(float s)
	{
		float rs = 1.0f / s;
		x *= rs; y *= rs; z *= rs;
		return *this;
	}

	bool operator==(const Vector3& lhs, const Vector3& rhs)
	{
		return (lhs - rhs).LengthSquare() <= FLT_EPSILON;
	}

	bool operator!=(const Vector3& lhs, const Vector3& rhs)
	{
		return !(lhs == rhs);
	}

	Vector3 operator+(const Vector3& lhs, const Vector3& rhs)
	{
		return Vector3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
	}

	Vector3 operator-(const Vector3& lhs, const Vector3& rhs)
	{
		return Vector3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
	}

	Vector3 operator*(float s, const Vector3& v)
	{
		return Vector3(s * v.x, s * v.y, s * v.z);
	}

	Vector3 operator*(const Vector3& v, float s)
	{
		return Vector3(s * v.x, s * v.y, s * v.z);
	}

	Vector3 operator/(const Vector3& v, float s)
	{
		float rs = 1.0f / s;
		return Vector3(rs * v.x, rs * v.y, rs * v.z);
	}

	Vector3 Vector3::Normalize(const Vector3& v)
	{
		auto result = v;
		result.Normalize();
		return result;
	}

	float Vector3::Dot(const Vector3& lhs, const Vector3& rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
	}

	float Vector3::Angle(const Vector3& lhs, const Vector3& rhs)
	{
		auto ll = lhs.LengthSquare();
		auto rl = rhs.LengthSquare();
		LOG_ASSERT(ll > 0 && rl > 0, "Have 0 length vector!")

		float c = Dot(lhs, rhs) / sqrtf(ll * rl);
		return acosf(c);
	}

	Vector3 Vector3::Project(const Vector3& v, const Vector3& dir)
	{
		return Dot(v, dir) * dir;
	}

	Vector3 Vector3::Perpendicular(const Vector3& v, const Vector3& dir)
	{
		return v - Project(v, dir);
	}

	Vector3 Vector3::Reflection(const Vector3& v, const Vector3& normal)
	{
		return v + 2.0f * Project(-v, normal);
	}

	Vector3 Vector3::Cross(const Vector3& lhs, const Vector3& rhs)
	{
		return Vector3(lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x);
	}

	float Vector3::Distance(const Vector3& lhs, const Vector3& rhs)
	{
		return (lhs - rhs).Length();
	}
	#pragma endregion

	#pragma region Vector4
	Vector4& Vector4::operator=(const Vector4& rhs)
	{
		x = rhs.x; y = rhs.y; z = rhs.z; w = rhs.w;
		return *this;
	}

	Vector4 operator*(float s, const Vector4& v)
	{
		return Vector4(s * v.x, s * v.y, s * v.z, s * v.w);
	}

	Vector4 operator*(const Vector4& v, float s)
	{
		return Vector4(s * v.x, s * v.y, s * v.z, s * v.w);
	}

	Vector4 operator/(const Vector4& v, float s)
	{
		float rs = 1.0f / s;
		return Vector4(rs * v.x, rs * v.y, rs * v.z, rs * v.w);
	}

	#pragma endregion
}