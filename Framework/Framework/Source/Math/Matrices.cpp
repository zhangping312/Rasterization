#include <Math/Matrices.h>
#include <Math/Vectors.h>
#include <Logger/Logger.h>

#include <memory>

namespace FW::Math
{
	#pragma region Matrix2
	constexpr size_t MATRIX2_MEMORY_SIZE = sizeof(float) * 4;

	Matrix2::Matrix2()
		: m11(1.0f), m12(0.0f),
		  m21(0.0f), m22(1.0f)
	{
	}

	Matrix2::Matrix2(const float values[2][2])
	{
		memcpy(this->values, values, MATRIX2_MEMORY_SIZE);
	}

	Matrix2::Matrix2(const float values[4])
	{
		memcpy(this->values, values, MATRIX2_MEMORY_SIZE);
	}

	Matrix2::Matrix2(const Matrix2& rhs)
	{
		memcpy(this->values, rhs.values, MATRIX2_MEMORY_SIZE);
	}

	Matrix2& Matrix2::operator=(const Matrix2& rhs)
	{
		memcpy(this->values, rhs.values, MATRIX2_MEMORY_SIZE);
		return *this;
	}

	Matrix2 operator*(float s, const Matrix2& m)
	{
		Matrix2 result;
		for (int r = 0; r < 2; ++r)
		{
			for (int c = 0; c < 2; ++c)
				result.values[r][c] = s * m.values[r][c];
		}
		return result;
	}

	Matrix2 operator*(const Matrix2& m, float s)
	{
		return s * m;
	}

	Matrix2 operator*(const Matrix2& lhs, const Matrix2& rhs)
	{
		Matrix2 result;
		result.m11 = lhs.m11 * rhs.m11 + lhs.m12 * rhs.m21;
		result.m12 = lhs.m11 * rhs.m12 + lhs.m12 * rhs.m22;

		result.m21 = lhs.m21 * rhs.m11 + lhs.m22 * rhs.m21;
		result.m22 = lhs.m21 * rhs.m12 + lhs.m22 * rhs.m22;
		return result;
	}

	Vector2 operator*(const Vector2& v, const Matrix2& m)
	{
		Vector2 result;
		result.x = v.x * m.m11 + v.y * m.m21;
		result.y = v.x * m.m12 + v.y * m.m22;
		return result;
	}

	Matrix2 Matrix2::Transpose(const Matrix2& m)
	{
		Matrix2 result;
		for (int r = 0; r < 2; ++r)
		{
			for (int c = 0; c < 2; ++c)
				result.values[r][c] = m.values[c][r];
		}
		return result;
	}

	Matrix2 Matrix2::Scale(const Vector2& scale)
	{
		return Scale(scale.x, scale.y);
	}

	Matrix2 Matrix2::Scale(float x, float y)
	{
		float values[2][2] =
		{
			   x, 0.0f,
			0.0f,    y,
		};
		return Matrix2(values);
	}

	Matrix2 Matrix2::Rotate(float radius)
	{
		float s = sinf(radius);
		float c = cosf(radius);
		float value[2][2] =
		{
			 c, s,
			-s, c,
		};
		return Matrix2(value);
	}
	#pragma endregion

	#pragma region Matrix3
	constexpr size_t MATRIX3_MEMORY_SIZE = sizeof(float) * 9;

	Matrix3::Matrix3()
		: m11(1.0f), m12(0.0f), m13(0.0f),
		  m21(0.0f), m22(1.0f), m23(0.0f),
		  m31(0.0f), m32(0.0f), m33(1.0f)
	{
	}

	Matrix3::Matrix3(const float values[3][3])
	{
		memcpy(this->values, values, MATRIX3_MEMORY_SIZE);
	}

	Matrix3::Matrix3(const float values[9])
	{
		memcpy(this->values, values, MATRIX3_MEMORY_SIZE);
	}

	Matrix3::Matrix3(const Matrix3& rhs)
	{
		memcpy(this->values, rhs.values, MATRIX3_MEMORY_SIZE);
	}

	Matrix3& Matrix3::operator=(const Matrix3& rhs)
	{
		memcpy(this->values, rhs.values, MATRIX3_MEMORY_SIZE);
		return *this;
	}

	Matrix3 operator*(float s, const Matrix3& m)
	{
		Matrix3 result;
		for (int r = 0; r < 3; ++r)
		{
			for (int c = 0; c < 3; ++c)
				result.values[r][c] = s * m.values[r][c];
		}

		return result;
	}

	Matrix3 operator*(const Matrix3& m, float s)
	{
		return s * m;
	}

	Matrix3 operator*(const Matrix3& lhs, const Matrix3& rhs)
	{
		Matrix3 result;
		result.m11 = lhs.m11 * rhs.m11 + lhs.m12 * rhs.m21 + lhs.m13 * rhs.m31;
		result.m12 = lhs.m11 * rhs.m12 + lhs.m12 * rhs.m22 + lhs.m13 * rhs.m32;
		result.m13 = lhs.m11 * rhs.m13 + lhs.m12 * rhs.m23 + lhs.m13 * rhs.m33;

		result.m21 = lhs.m21 * rhs.m11 + lhs.m22 * rhs.m21 + lhs.m23 * rhs.m31;
		result.m22 = lhs.m21 * rhs.m12 + lhs.m22 * rhs.m22 + lhs.m23 * rhs.m32;
		result.m23 = lhs.m21 * rhs.m13 + lhs.m22 * rhs.m23 + lhs.m23 * rhs.m33;

		result.m31 = lhs.m31 * rhs.m11 + lhs.m32 * rhs.m21 + lhs.m33 * rhs.m31;
		result.m32 = lhs.m31 * rhs.m12 + lhs.m32 * rhs.m22 + lhs.m33 * rhs.m32;
		result.m33 = lhs.m31 * rhs.m13 + lhs.m32 * rhs.m23 + lhs.m33 * rhs.m33;
		return result;
	}

	Vector3 operator*(const Vector3& v, const Matrix3& m)
	{
		Vector3 result;
		result.x = v.x * m.m11 + v.y * m.m21 + v.z * m.m31;
		result.y = v.x * m.m12 + v.y * m.m22 + v.z * m.m32;
		result.z = v.x * m.m13 + v.y * m.m23 + v.z * m.m33;
		return result;
	}

	Matrix3 Matrix3::Transpose(const Matrix3& m)
	{
		Matrix3 result;
		for (int r = 0; r < 3; ++r)
		{
			for (int c = 0; c < 3; ++c)
				result.values[r][c] = m.values[c][r];
		}
		return result;
	}

	Matrix3 Matrix3::Scale(const Vector3& scale)
	{
		return Scale(scale.x, scale.y, scale.z);
	}

	Matrix3 Matrix3::Scale(float x, float y, float z)
	{
		float values[3][3] =
		{
			   x, 0.0f, 0.0f,
			0.0f,    y, 0.0f,
			0.0f, 0.0f,    z,
		};
		return Matrix3(values);
	}

	Matrix3 Matrix3::RotateX(float radius)
	{
		float s = sinf(radius);
		float c = cosf(radius);
		float value[3][3] =
		{
			1.0f, 0.0f, 0.0f,
			0.0f,	 c,	   s,
			0.0f,   -s,	   c
		};

		return Matrix3(value);
	}

	Matrix3 Matrix3::RotateY(float radius)
	{
		float s = sinf(radius);
		float c = cosf(radius);
		float value[3][3] =
		{
			   c, 0.0f,   -s,
			0.0f, 1.0f, 0.0f,
			   s, 0.0f,	   c
		};

		return Matrix3(value);
	}

	Matrix3 Matrix3::RotateZ(float radius)
	{
		float s = sinf(radius);
		float c = cosf(radius);
		float value[3][3] =
		{
			   c,	 s, 0.0f,
			  -s,	 c, 0.0f,
			0.0f, 0.0f, 1.0f
		};

		return Matrix3(value);
	}

	Matrix3 Matrix3::Rotate(const Vector3& rotation)
	{
		return Rotate(rotation.x, rotation.y, rotation.z);
	}

	Matrix3 Matrix3::Rotate(float x, float y, float z)
	{
		float sx = sinf(x);
		float cx = cosf(x);
		float sy = sinf(y);
		float cy = cosf(y);
		float sz = sinf(z);
		float cz = cosf(z);
		float values[3][3] =
		{
						   cy * cz,				   cy * sz,		-sy,
			sx * sy * cz - cx * sz, sx * sy * sz + cx * cz, sx * cy,
			cx * sy * cz + sx * sz, cx * sy * sz - sx * cz, cx * cy,
		};
		return Matrix3(values);
	}

	Matrix3 Matrix3::RotateArbitraryAxis(const Vector3& axis, float radius)
	{
		float s = sinf(radius);
		float c = cosf(radius);
		float oc = 1.0f - c;
		float xy = axis.x * axis.y;
		float xz = axis.x * axis.z;
		float yz = axis.y * axis.z;
		float xx = axis.x * axis.x;
		float yy = axis.y * axis.y;
		float zz = axis.z * axis.z;

		float sx = s * axis.x;
		float sy = s * axis.y;
		float sz = s * axis.z;
		float values[3][3] =
		{
			 c + oc * xx, oc * xy + sz, oc * xz - sy,
			oc * xy - sz,  c + oc * yy, oc * yz + sx,
			oc * xz + sy, oc * yz - sx,  c + oc * zz,
		};
		return Matrix3(values);
	}
	#pragma endregion

	#pragma region Matrix4
	constexpr size_t MATRIX4_MEMORY_SIZE = sizeof(float) * 16;

	Matrix4::Matrix4()
		: m11(1.0f), m12(0.0f), m13(0.0f), m14(0.0f),
		  m21(0.0f), m22(1.0f), m23(0.0f), m24(0.0f),
		  m31(0.0f), m32(0.0f), m33(1.0f), m34(0.0f),
		  m41(0.0f), m42(0.0f), m43(0.0f), m44(1.0f)
	{
	}

	Matrix4::Matrix4(const float values[4][4])
	{
		memcpy(this->values, values, MATRIX4_MEMORY_SIZE);
	}

	Matrix4::Matrix4(const float values[16])
	{
		memcpy(this->values, values, MATRIX4_MEMORY_SIZE);
	}

	Matrix4::Matrix4(const Matrix4& rhs)
	{
		memcpy(this->values, rhs.values, MATRIX4_MEMORY_SIZE);
	}

	Matrix4& Matrix4::operator=(const Matrix4& rhs)
	{
		memcpy(this->values, rhs.values, MATRIX4_MEMORY_SIZE);
		return *this;
	}

	void Matrix4::Inverse()
	{
		float temp[6];
		temp[0] = m33 * m44 - m34 * m43;
		temp[1] = m32 * m44 - m34 * m42;
		temp[2] = m32 * m43 - m33 * m42;

		temp[3] = m31 * m44 - m34 * m41;
		temp[4] = m31 * m43 - m33 * m41;
		temp[5] = m31 * m42 - m32 * m41;

		float M11 = m22 * temp[0] - m23 * temp[1] + m24 * temp[2];
		float M12 = m21 * temp[0] - m23 * temp[3] + m24 * temp[4];
		float M13 = m21 * temp[1] - m22 * temp[3] + m24 * temp[5];
		float M14 = m21 * temp[2] - m22 * temp[4] + m23 * temp[5];

		float det = m11 * M11 - m12 * M12 + m13 * M13 - m14 * M14;
		LOG_ASSERT(det > 0.0f, "This matrix can't invert!");

		float M21 = m12 * temp[0] - m13 * temp[1] + m14 * temp[2];
		float M22 = m11 * temp[0] - m13 * temp[3] + m14 * temp[4];
		float M23 = m11 * temp[1] - m12 * temp[3] + m14 * temp[5];
		float M24 = m11 * temp[2] - m12 * temp[4] + m13 * temp[5];

		temp[0] = m13 * m24 - m14 * m23;
		temp[1] = m12 * m24 - m14 * m22;
		temp[2] = m12 * m23 - m13 * m22;

		temp[3] = m11 * m24 - m14 * m21;
		temp[4] = m11 * m23 - m13 * m21;
		temp[5] = m11 * m22 - m12 * m21;

		float M31 = m42 * temp[0] - m43 * temp[1] + m44 * temp[2];
		float M32 = m41 * temp[0] - m43 * temp[3] + m44 * temp[4];
		float M33 = m41 * temp[1] - m42 * temp[3] + m44 * temp[5];
		float M34 = m41 * temp[2] - m42 * temp[4] + m43 * temp[5];

		float M41 = m32 * temp[0] - m33 * temp[1] + m34 * temp[2];
		float M42 = m31 * temp[0] - m33 * temp[3] + m34 * temp[4];
		float M43 = m31 * temp[1] - m32 * temp[3] + m34 * temp[5];
		float M44 = m31 * temp[2] - m32 * temp[4] + m33 * temp[5];

		float A11 =  M11; float A12 = -M12; float A13 =  M13; float A14 = -M14;
		float A21 = -M21; float A22 =  M22; float A23 = -M23; float A24 =  M24;
		float A31 =  M31; float A32 = -M32; float A33 =  M33; float A34 = -M34;
		float A41 = -M41; float A42 =  M42; float A43 = -M43; float A44 =  M44;

		m11 = A11; m12 = A21; m13 =  A31; m14 = A41;
		m21 = A12; m22 = A22; m23 = A32; m24 = A42;
		m31 = A13; m32 = A23; m33 = A33; m34 = A43;
		m41 = A14; m42 = A24; m43 = A34; m44 = A44;
		float oneOverDet = 1.0f / det;
		for (int r = 0; r < 4; ++r)
		{
			for (int c = 0; c < 4; ++c)
				values[r][c] *= oneOverDet;
		}
	}

	Matrix4 operator*(float s, const Matrix4& m)
	{
		Matrix4 result;
		for (int r = 0; r < 4; ++r)
		{
			for (int c = 0; c < 4; ++c)
				result.values[r][c] = s * m.values[r][c];
		}
		return result;
	}

	Matrix4 operator*(const Matrix4& m, float s)
	{
		return s * m;
	}

	Matrix4 operator*(const Matrix4& lhs, const Matrix4& rhs)
	{
		Matrix4 result;
		result.m11 = lhs.m11 * rhs.m11 + lhs.m12 * rhs.m21 + lhs.m13 * rhs.m31 + lhs.m14 * rhs.m41;
		result.m12 = lhs.m11 * rhs.m12 + lhs.m12 * rhs.m22 + lhs.m13 * rhs.m32 + lhs.m14 * rhs.m42;
		result.m13 = lhs.m11 * rhs.m13 + lhs.m12 * rhs.m23 + lhs.m13 * rhs.m33 + lhs.m14 * rhs.m43;
		result.m14 = lhs.m11 * rhs.m14 + lhs.m12 * rhs.m24 + lhs.m13 * rhs.m34 + lhs.m14 * rhs.m44;

		result.m21 = lhs.m21 * rhs.m11 + lhs.m22 * rhs.m21 + lhs.m23 * rhs.m31 + lhs.m24 * rhs.m41;
		result.m22 = lhs.m21 * rhs.m12 + lhs.m22 * rhs.m22 + lhs.m23 * rhs.m32 + lhs.m24 * rhs.m42;
		result.m23 = lhs.m21 * rhs.m13 + lhs.m22 * rhs.m23 + lhs.m23 * rhs.m33 + lhs.m24 * rhs.m43;
		result.m24 = lhs.m21 * rhs.m14 + lhs.m22 * rhs.m24 + lhs.m23 * rhs.m34 + lhs.m24 * rhs.m44;

		result.m31 = lhs.m31 * rhs.m11 + lhs.m32 * rhs.m21 + lhs.m33 * rhs.m31 + lhs.m34 * rhs.m41;
		result.m32 = lhs.m31 * rhs.m12 + lhs.m32 * rhs.m22 + lhs.m33 * rhs.m32 + lhs.m34 * rhs.m42;
		result.m33 = lhs.m31 * rhs.m13 + lhs.m32 * rhs.m23 + lhs.m33 * rhs.m33 + lhs.m34 * rhs.m43;
		result.m34 = lhs.m31 * rhs.m14 + lhs.m32 * rhs.m24 + lhs.m33 * rhs.m34 + lhs.m34 * rhs.m44;

		result.m41 = lhs.m41 * rhs.m11 + lhs.m42 * rhs.m21 + lhs.m43 * rhs.m31 + lhs.m44 * rhs.m41;
		result.m42 = lhs.m41 * rhs.m12 + lhs.m42 * rhs.m22 + lhs.m43 * rhs.m32 + lhs.m44 * rhs.m42;
		result.m43 = lhs.m41 * rhs.m13 + lhs.m42 * rhs.m23 + lhs.m43 * rhs.m33 + lhs.m44 * rhs.m43;
		result.m44 = lhs.m41 * rhs.m14 + lhs.m42 * rhs.m24 + lhs.m43 * rhs.m34 + lhs.m44 * rhs.m44;
		return result;
	}

	Matrix4 Matrix4::Transpose(const Matrix4& m)
	{
		Matrix4 result;
		for (int r = 0; r < 4; ++r)
		{
			for (int c = 0; c < 4; ++c)
				result.values[r][c] = m.values[c][r];
		}
		return result;
	}

	Matrix4 Matrix4::Invert(const Matrix4& m)
	{
		Matrix4 result = m;
		result.Inverse();
		return result;
	}

	Matrix4 Matrix4::Scale(const Vector3& scale)
	{
		return Scale(scale.x, scale.y, scale.z);
	}

	Matrix4 Matrix4::Scale(float x, float y, float z)
	{
		float values[4][4] =
		{
			   x, 0.0f, 0.0f, 0.0f,
			0.0f,    y, 0.0f, 0.0f,
			0.0f, 0.0f,    z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f,
		};
		return Matrix4(values);
	}

	Matrix4 Matrix4::RotateX(float radius)
	{
		float s = sinf(radius);
		float c = cosf(radius);
		float values[4][4] =
		{
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f,    c,    s, 0.0f,
			0.0f,   -s,    c, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f,
		};
		return Matrix4(values);
	}

	Matrix4 Matrix4::RotateY(float radius)
	{
		float s = sinf(radius);
		float c = cosf(radius);
		float values[4][4] =
		{
			   c, 0.0f,   -s, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			   s, 0.0f,    c, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f,
		};
		return Matrix4(values);
	}

	Matrix4 Matrix4::RotateZ(float radius)
	{
		float s = sinf(radius);
		float c = cosf(radius);
		float values[4][4] =
		{
			   c,    s, 0.0f, 0.0f,
			  -s,    c, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f,
		};
		return Matrix4(values);
	}

	Matrix4 Matrix4::Rotate(const Vector3& rotation)
	{
		return Rotate(rotation.x, rotation.y, rotation.z);
	}

	Matrix4 Matrix4::Rotate(float x, float y, float z)
	{
		float sx = sinf(x);
		float cx = cosf(x);
		float sy = sinf(y);
		float cy = cosf(y);
		float sz = sinf(z);
		float cz = cosf(z);
		float values[4][4] =
		{
						   cy * cz,				   cy * sz,		-sy, 0.0f,
			sx * sy * cz - cx * sz, sx * sy * sz + cx * cz, sx * cy, 0.0f,
			cx * sy * cz + sx * sz, cx * sy * sz - sx * cz, cx * cy, 0.0f,
							  0.0f,					  0.0f,	   0.0f, 1.0f
		};
		return Matrix4(values);
	}

	Matrix4 Matrix4::RotateArbitraryAxis(const Vector3& axis, float radius)
	{
		float s = sinf(radius);
		float c = cosf(radius);
		float oc = 1.0f - c;
		float xy = axis.x * axis.y;
		float xz = axis.x * axis.z;
		float yz = axis.y * axis.z;
		float xx = axis.x * axis.x;
		float yy = axis.y * axis.y;
		float zz = axis.z * axis.z;

		float sx = s * axis.x;
		float sy = s * axis.y;
		float sz = s * axis.z;
		float values[4][4] =
		{
			 c + oc * xx, oc * xy + sz, oc * xz - sy, 0.0f,
			oc * xy - sz,  c + oc * yy, oc * yz + sx, 0.0f,
			oc * xz + sy, oc * yz - sx,  c + oc * zz, 0.0f,
					0.0f,		  0.0f,			0.0f, 1.0f
		};
		return Matrix4(values);
	}

	Matrix4 Matrix4::Translate(const Vector3& position)
	{
		return Translate(position.x, position.y, position.z);
	}

	Matrix4 Matrix4::Translate(float x, float y, float z)
	{
		float values[4][4] =
		{
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			   x,    y,    z, 1.0f,
		};
		return Matrix4(values);
	}

	Vector3 Matrix4::Transform(const Vector3& v, const Matrix4& m, float w)
	{
		Vector3 result;
		result.x = v.x * m.m11 + v.y * m.m21 + v.z * m.m31 + w * m.m41;
		result.y = v.x * m.m12 + v.y * m.m22 + v.z * m.m32 + w * m.m42;
		result.z = v.x * m.m13 + v.y * m.m23 + v.z * m.m33 + w * m.m43;
		return result;
	}

	Vector4 Matrix4::Transform(const Vector4& v, const Matrix4& m)
	{
		Vector4 result;
		result.x = v.x * m.m11 + v.y * m.m21 + v.z * m.m31 + v.w * m.m41;
		result.y = v.x * m.m12 + v.y * m.m22 + v.z * m.m32 + v.w * m.m42;
		result.z = v.x * m.m13 + v.y * m.m23 + v.z * m.m33 + v.w * m.m43;
		result.w = v.x * m.m14 + v.y * m.m24 + v.z * m.m34 + v.w * m.m44;
		return result;
	}

	Matrix4 Matrix4::Transform(const Vector3& position, const Vector3& rotation, const Vector3& scale)
	{
		Matrix4 S = Scale(scale);
		Matrix4 R = Rotate(rotation);
		Matrix4 T = Translate(position);
		return S * R * T;
	}

	Matrix4 Matrix4::LookAt(const Vector3& eye, const Vector3& lookAt, const Vector3& up)
	{
		// Z Axis
		Vector3 N = Vector3::Normalize(lookAt - eye);
		// X Axis
		Vector3 U = Vector3::Normalize(Vector3::Cross(up, N));
		// Y Axis
		Vector3 V = Vector3::Normalize(Vector3::Cross(N, U));

		float dx = -Vector3::Dot(eye, U);
		float dy = -Vector3::Dot(eye, V);
		float dz = -Vector3::Dot(eye, N);
		float values[4][4] =
		{
			U.x, V.x, N.x, 0.0f,
			U.y, V.y, N.y, 0.0f,
			U.z, V.z, N.z, 0.0f,
			 dx,  dy,  dz, 1.0f,
		};
		return Matrix4(values);
	}

	Matrix4 Matrix4::Orthographic(float l, float r, float b, float t, float n, float f)
	{
		float values[4][4] =
		{
			   2.0f / (r - l),              0.0f,           0.0f, 0.0f,
			             0.0f,    2.0f / (t - b),           0.0f, 0.0f,
			             0.0f,              0.0f, 1.0f / (f - n), 0.0f,
			(l + r) / (l - r), (b + t) / (b - t),    n / (n - f), 1.0f,
		};
		return Matrix4(values);
	}

	Matrix4 Matrix4::Orthographic(float w, float h, float n, float f)
	{
		float values[4][4] =
		{
			2.0f / w, 	  0.0f,           0.0f, 0.0f,
				0.0f, 2.0f / h,           0.0f, 0.0f,
				0.0f,     0.0f, 1.0f / (f - n), 0.0f,
				0.0f,     0.0f,    n / (n - f), 1.0f,
		};
		return Matrix4(values);
	}

	Matrix4 Matrix4::Perspective(float l, float r, float b, float t, float n, float f)
	{
		float values[4][4] =
		{
			2.0f * n / (r - l),               0.0f,              0.0f, 0.0f,
			              0.0f, 2.0f * n / (t - b),              0.0f, 0.0f,
			 (l + r) / (l - r),  (b + t) / (b - t),       f / (f - n), 1.0f,
			              0.0f,               0.0f, (n * f) / (n - f), 0.0f,
		};
		return Matrix4(values);
	}

	Matrix4 Matrix4::Perspective(float w, float h, float n, float f)
	{
		float values[4][4] =
		{
			2.0f * n / w,         0.0f,              0.0f, 0.0f,
			        0.0f, 2.0f * n / h,              0.0f, 0.0f,
			        0.0f,         0.0f,       f / (f - n), 1.0f,
			        0.0f,         0.0f, (n * f) / (n - f), 0.0f,
		};
		return Matrix4(values);
	}

	Matrix4 Matrix4::PerspectiveFov(float fov, float aspectRatio, float n, float f)
	{
		float t = tanf(fov / 2.0f);
		float values[4][4] =
		{
			1.0f / (t * aspectRatio),     0.0f,              0.0f, 0.0f,
			       				0.0f, 1.0f / t,              0.0f, 0.0f,
			       				0.0f,     0.0f,       f / (f - n), 1.0f,
			       				0.0f,     0.0f, (n * f) / (n - f), 0.0f,
		};
		return Matrix4(values);
	}
	#pragma endregion
}