#include <vector>
#include <algorithm>

#ifndef MATH_H
#define MATH_H

#define PI 3.14159265f
#define DEGREE 180 / PI
#define RADIAN PI / 180

using namespace std;

// My general purpose 3D math library
class Vec3f {
public:
		float values[3];
		float &x, &y, &z;
		Vec3f() :  x(values[0]), y(values[1]), z(values[2]) { values[0] = values[1] = values[2] = 0; }
		Vec3f(float x, float y, float z) : x(values[0]), y(values[1]), z(values[2])  { values[0] = x; values[1] = y; values[2] = z; }
		Vec3f operator=(const Vec3f &other);
		Vec3f &operator-();
		Vec3f operator*(const Vec3f &other) const;
		Vec3f operator+(const Vec3f &other) const;
		Vec3f operator-(const Vec3f &other) const;
		void operator+=(const Vec3f &other);
		void operator-=(const Vec3f &other);
		Vec3f operator*(const float &other) const;
		Vec3f operator-(const float &other) const;
		bool operator<=(const Vec3f &other) const;
		bool operator>=(const Vec3f &other) const;
		bool operator!=(const Vec3f &other) const;
		bool operator==(const Vec3f &other) const;
		void Abs();
		Vec3f Scale(const Vec3f &other) const;
		static Vec3f Lerp(const Vec3f &point1, const Vec3f &point2, float percent);
		float Mag() const;
		Vec3f &Normalize();
        void Zero();
		static bool RayIntersection(const Vec3f &rayStart, const Vec3f &ray, 
			const Vec3f &triPoint1, const Vec3f &triPoint2, const Vec3f &triPoint3, Vec3f &intersectionPt);
		static bool LineSegmentIntersection(const Vec3f &lineStart, const Vec3f &lineEnd, 
			                const Vec3f &a, const Vec3f &b, const Vec3f &c, 
						    Vec3f &intersectionPt);
		static Vec3f CalculateNormal(const Vec3f &point1, const Vec3f &point2, const Vec3f &point3);
		static float ScalarTriple(const Vec3f &vec1, const Vec3f &vec2, const Vec3f &vec3);
}; 

struct MatrixRow 
{
	MatrixRow(float vals[4]) : x(vals[0]), y(vals[1]), z(vals[2]), b(vals[3]) { }
	void SetComponents(float x, float y, float z, float b) {
		this->x = x; this->y = y; this->z = z; this->b = b; 
	}
	float &x, &y, &z, &b;
	friend class Matrix;
};

class Matrix {
protected:
	void RotateX(const float t);
	void RotateY(const float t);
	void RotateZ(const float t);
	void Translate(const Vec3f &v);
	void Scale(const float f);
public:  
	MatrixRow row1, row2, row3, row4;
	float m4x4[4][4];
	void MakeColumnMajor();
	Matrix() : row1(m4x4[0]), row2(m4x4[1]), row3(m4x4[2]), row4(m4x4[3])
				{ Identity(); }
	Vec3f TransformP(const Vec3f &p) const;
	void Identity();
	Matrix &operator=(const Matrix &other);
	Matrix(const Matrix &other);
	friend Matrix MatrixForXRotation(float t);
	friend Matrix MatrixForYRotation(float t);
	friend Matrix MatrixForZRotation(float t);
	friend Matrix MatrixForTranslation(const Vec3f &v);
	friend Matrix operator*(const Matrix &lhs, const Matrix &rhs);
};
Matrix operator*(const Matrix &lhs, const Matrix &rhs);
Matrix MatrixForXRotation(float t);
Matrix MatrixForYRotation(float t);
Matrix MatrixForZRotation(float t);
Matrix MatrixForTranslation(const Vec3f &v);

float dot(const Vec3f &a, const Vec3f &b);

void AddToVector(vector<float> &v, float a, float b, float c);

#endif
