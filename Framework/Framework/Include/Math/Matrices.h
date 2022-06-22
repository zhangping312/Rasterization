#pragma once

namespace FW::Math
{
	struct Vector2;
	struct Vector3;
	struct Vector4;

	struct Matrix2
	{
		union
		{
			struct
			{
				float m11, m12;
				float m21, m22;
			};
			struct
			{
				float row1[2];
				float row2[3];
			};
			float values[2][2];
		};

		Matrix2();
		explicit Matrix2(const float values[2][2]);
		explicit Matrix2(const float values[4]);
		Matrix2(const Matrix2& rhs);
		Matrix2& operator=(const Matrix2& rhs);

		friend Matrix2 operator*(float s, const Matrix2& m);
		friend Matrix2 operator*(const Matrix2& m, float s);
		friend Matrix2 operator*(const Matrix2& lhs, const Matrix2& rhs);
		friend Vector2 operator*(const Vector2& v, const Matrix2& m);

		static Matrix2 Transpose(const Matrix2& m);

		static Matrix2 Scale(const Vector2& scale);
		static Matrix2 Scale(float x, float y);
		static Matrix2 Rotate(float radius);
	};

	struct Matrix3
	{
		union
		{
			struct
			{
				float m11, m12, m13;
				float m21, m22, m23;
				float m31, m32, m33;
			};
			struct
			{
				float row1[3];
				float row2[3];
				float row3[3];
			};
			float values[3][3];
		};

		Matrix3();
		explicit Matrix3(const float values[3][3]);
		explicit Matrix3(const float values[9]);
		Matrix3(const Matrix3& rhs);
		Matrix3& operator=(const Matrix3& rhs);

		friend Matrix3 operator*(float s, const Matrix3& m);
		friend Matrix3 operator*(const Matrix3& m, float s);
		friend Matrix3 operator*(const Matrix3& lhs, const Matrix3& rhs);
		friend Vector3 operator*(const Vector3& v, const Matrix3& m);

		static Matrix3 Transpose(const Matrix3& m);

		static Matrix3 Scale(const Vector3& scale);
		static Matrix3 Scale(float x, float y, float z);
		static Matrix3 RotateX(float radius);
		static Matrix3 RotateY(float radius);
		static Matrix3 RotateZ(float radius);
		/**
		 * Rotate in this order: x, y, z (radius)
		 */
		static Matrix3 Rotate(const Vector3& rotation);
		/**
		 * Rotate in this order: x, y, z (radius)
		 */
		static Matrix3 Rotate(float x, float y, float z);
		static Matrix3 RotateArbitraryAxis(const Vector3& axis, float radius);
	};

	struct Matrix4
	{
		union
		{
			struct
			{
				float m11, m12, m13, m14;
				float m21, m22, m23, m24;
				float m31, m32, m33, m34;
				float m41, m42, m43, m44;
			};
			struct
			{
				float row1[4];
				float row2[4];
				float row3[4];
				float row4[4];
			};
			float values[4][4];
		};

		Matrix4();
		explicit Matrix4(const float values[4][4]);
		explicit Matrix4(const float values[16]);
		Matrix4(const Matrix4& rhs);
		Matrix4& operator=(const Matrix4& rhs);

		void Inverse();

		friend Matrix4 operator*(float s, const Matrix4& m);
		friend Matrix4 operator*(const Matrix4& m, float s);
		friend Matrix4 operator*(const Matrix4& lhs, const Matrix4& rhs);

		static Matrix4 Transpose(const Matrix4& m);
		static Matrix4 Invert(const Matrix4& m);

		static Matrix4 Scale(const Vector3& scale);
		static Matrix4 Scale(float x, float y, float z);
		static Matrix4 RotateX(float radius);
		static Matrix4 RotateY(float radius);
		static Matrix4 RotateZ(float radius);
		/**
		 * Rotate in this order: x, y, z (radius)
		 */
		static Matrix4 Rotate(const Vector3& rotation);
		/**
		 * Rotate in this order: x, y, z (radius)
		 */
		static Matrix4 Rotate(float x, float y, float z);
		static Matrix4 RotateArbitraryAxis(const Vector3& axis, float radius);
		static Matrix4 Translate(const Vector3& position);
		static Matrix4 Translate(float x, float y, float z);

		/**
		* w = 0.0f: transform the vector
		* w = 1.0f: transform the point
		*/
		static Vector3 Transform(const Vector3& v, const Matrix4& m, float w = 1.0f);
		static Vector4 Transform(const Vector4& v, const Matrix4& m);
		/**
		* Get the transform matrix
		*/
		static Matrix4 Transform(const Vector3& position, const Vector3& rotation, const Vector3& scale);
		/**
		* UVN Camera
		* First get the translate matrix(Translate camera to origin point):
		* T = [ 1 0 0 0 ]
		*	  [ 0 1 0 0 ]
		*	  [ 0 0 1 0 ]
		*	  [ x y z 1 ]
		* x = -camera.x, y = -camera.y, z = -camera.z.
		* Rotate camera, make UVN coincide with XYZ
		* we can get the rotate matrix
		* R = [ Ux Vx Nz 0 ]
		*	  [ Uy Vy Ny 0 ]
		*	  [ Uz Vz Nz 0 ]
		*	  [  0  0  0 1 ]
		* Finish calculate the view matrix(T * R)
		*/
		static Matrix4 LookAt(const Vector3& eye, const Vector3& lookAt, const Vector3& up);

		static Matrix4 Orthographic(float l, float r, float b, float t, float n, float f);
		static Matrix4 Orthographic(float w, float h, float n, float f);
		static Matrix4 Perspective(float l, float r, float b, float t, float n, float f);
		static Matrix4 Perspective(float w, float h, float n, float f);
		/**
		 * fov: Vertical fov (radius)
		 */
		static Matrix4 PerspectiveFov(float fov, float aspectRatio, float n, float f);
	};
}