#pragma once

namespace FW::Math
{
	struct Vector2
	{
		union
		{
			struct { float x, y; };
			struct { float u, v; };
			float values[2];
		};

		Vector2() : x(0.0f), y(0.0f) {}
		Vector2(float x, float y) : x(x), y(y) {}
		Vector2(const Vector2& rhs) : x(rhs.x), y(rhs.y) {}
		Vector2& operator=(const Vector2& rhs);

		float LengthSquare() const;
		float Length() const;
		void Normalize();
		void Zero() { x = y = 0.0f; }

		Vector2 operator-() const { return Vector2(-x, -y); }

		Vector2& operator+=(const Vector2& rhs);
		Vector2& operator-=(const Vector2& rhs);
		Vector2& operator*=(float s);
		Vector2& operator/=(float s);

		friend bool operator==(const Vector2& lhs, const Vector2& rhs);
		friend bool operator!=(const Vector2& lhs, const Vector2& rhs);

		friend Vector2 operator+(const Vector2& lhs, const Vector2& rhs);
		friend Vector2 operator-(const Vector2& lhs, const Vector2& rhs);

		friend Vector2 operator*(float s, const Vector2& v);
		friend Vector2 operator*(const Vector2& v, float s);
		friend Vector2 operator/(const Vector2& v, float s);

		static Vector2 Normalize(const Vector2& v);
		static float Dot(const Vector2& lhs, const Vector2& rhs);
		static float Angle(const Vector2& lhs, const Vector2& rhs);
		/**
		 * v: The vector being projected
		 * dir: The vector it is being projected onto(normalized)
		 */
		static Vector2 Project(const Vector2& v, const Vector2& dir);
		static Vector2 Perpendicular(const Vector2& v, const Vector2& dir);
		static Vector2 Reflection(const Vector2& v, const Vector2& normal);
		static float Cross(const Vector2& lhs, const Vector2& rhs);

		static float Distance(const Vector2& lhs, const Vector2& rhs);
	};

	struct Vector3
	{
		union
		{
			struct { float x, y, z; };
			struct { float r, g, b; };
			float values[3];
		};

		Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
		Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
		Vector3(const Vector3& rhs) : x(rhs.x), y(rhs.y), z(rhs.z) {}
		Vector3& operator=(const Vector3& rhs);

		float LengthSquare() const;
		float Length() const;
		void Normalize();
		void Zero() { x = y = z = 0.0f; }

		Vector3 operator-() const { return Vector3(-x, -y, -z); }

		Vector3& operator+=(const Vector3& rhs);
		Vector3& operator-=(const Vector3& rhs);
		Vector3& operator*=(float s);
		Vector3& operator/=(float s);

		friend bool operator==(const Vector3& lhs, const Vector3& rhs);
		friend bool operator!=(const Vector3& lhs, const Vector3& rhs);

		friend Vector3 operator+(const Vector3& lhs, const Vector3& rhs);
		friend Vector3 operator-(const Vector3& lhs, const Vector3& rhs);

		friend Vector3 operator*(float s, const Vector3& v);
		friend Vector3 operator*(const Vector3& v, float s);
		friend Vector3 operator/(const Vector3& v, float s);

		static Vector3 Normalize(const Vector3& v);
		static float Dot(const Vector3& lhs, const Vector3& rhs);
		static float Angle(const Vector3& lhs, const Vector3& rhs);
		/**
		 * v: The vector being projected
		 * dir: The vector it is being projected onto(normalized)
		 */
		static Vector3 Project(const Vector3& v, const Vector3& dir);
		static Vector3 Perpendicular(const Vector3& v, const Vector3& dir);
		static Vector3 Reflection(const Vector3& v, const Vector3& normal);
		static Vector3 Cross(const Vector3& lhs, const Vector3& rhs);

		static float Distance(const Vector3& lhs, const Vector3& rhs);
	};

	struct Vector4
	{
		union
		{
			struct { float x, y, z, w; };
			struct { float r, g, b, a; };
			float values[4];
		};

		Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
		Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
		Vector4(const Vector4& rhs) : x(rhs.x), y(rhs.y), z(rhs.z), w(rhs.w) {}
		Vector4& operator=(const Vector4& rhs);

		friend Vector4 operator*(float s, const Vector4& v);
		friend Vector4 operator*(const Vector4& v, float s);
		friend Vector4 operator/(const Vector4& v, float s);
	};
}