#include <cmath>
#include "Math.h"
#include <stdlib.h>

// Constructs a matrix from a concatenation of two other matrices.
Matrix operator*(const Matrix &lhs, const Matrix &rhs)
{
	Matrix q;

	q.row1.x = lhs.row1.x * rhs.row1.x + lhs.row1.y * rhs.row2.x + lhs.row1.z * rhs.row3.x;
	q.row1.y = lhs.row1.x * rhs.row1.y + lhs.row1.y * rhs.row2.y + lhs.row1.z * rhs.row3.y;
	q.row1.z = lhs.row1.x * rhs.row1.z + lhs.row1.y * rhs.row2.z + lhs.row1.z * rhs.row3.z;
	q.row1.b = lhs.row1.b + rhs.row1.b;

	q.row2.x = lhs.row2.x * rhs.row1.x + lhs.row2.y * rhs.row2.x + lhs.row2.z * rhs.row3.x;
	q.row2.y = lhs.row2.x * rhs.row1.y + lhs.row2.y * rhs.row2.y + lhs.row2.z * rhs.row3.y;
	q.row2.z = lhs.row2.x * rhs.row1.z + lhs.row2.y * rhs.row2.z + lhs.row2.z * rhs.row3.z;
	q.row2.b = lhs.row2.b + rhs.row2.b;

	q.row3.x = lhs.row3.x * rhs.row1.x + lhs.row3.y * rhs.row2.x + lhs.row3.z * rhs.row3.x;
	q.row3.y = lhs.row3.x * rhs.row1.y + lhs.row3.y * rhs.row2.y + lhs.row3.z * rhs.row3.y;
	q.row3.z = lhs.row3.x * rhs.row1.z + lhs.row3.y * rhs.row2.z + lhs.row3.z * rhs.row3.z;
	q.row3.b = lhs.row3.b + rhs.row3.b;

	q.row4.x = 0.0f;
	q.row4.y = 0.0f;
	q.row4.z = 0.0f;
	q.row4.b = 1.0f;

	return q;
}

Matrix MatrixForXRotation(float t) {
	Matrix m;
	m.RotateX(t);
	return m;
}

Matrix MatrixForYRotation(float t) {
	Matrix m;
	m.RotateY(t);
	return m;
}

Matrix MatrixForZRotation(float t) {
	Matrix m;
	m.RotateZ(t);
	return m;
}

Matrix MatrixForTranslation(const Vec3f &v) {
	Matrix m;
	m.Translate(v);
	return m;
}

void Matrix::MakeColumnMajor()
{
	float *v = m4x4[0];
	swap(v[1], v[4]);
	swap(v[6], v[9]);
	swap(v[11], v[14]);
	swap(v[2], v[8]);
	swap(v[7], v[13]);
	swap(v[3], v[12]);
}
	
float dot(const Vec3f &a, const Vec3f &b)
{
	return a.values[0] * b.values[0] + a.values[1] * b.values[1] + a.values[2] * b.values[2];
}

void AddToVector(vector<float> &v, float a, float b, float c)
{
	v.push_back(a);
	v.push_back(b);
	v.push_back(c);
}

void Matrix::Identity()
{
	// Initialize to identity matrix
	row1.SetComponents(1, 0, 0, 0);
	row2.SetComponents(0, 1, 0, 0);
	row3.SetComponents(0, 0, 1, 0);
	row4.SetComponents(0, 0, 0, 1);
}

// Given an angle t in radians, a pure rotation matrix is created (around x-axis)
void Matrix::RotateX(const float t)
{
	row1.SetComponents(1, 0, 0, 0);
	row2.SetComponents(0, cos(t), -sin(t), 0);
	row3.SetComponents(0, sin(t), cos(t), 0);
}

void Matrix::RotateY(const float t)
{
	row1.SetComponents(cos(t), 0, sin(t), 0);
	row2.SetComponents(0, 1, 0, 0);
	row3.SetComponents(-sin(t), 0, cos(t), 0);
}

void Matrix::RotateZ(const float t)
{
	row1.SetComponents(cos(t), -sin(t), 0, 0);
	row2.SetComponents(sin(t), cos(t), 0, 0);
	row3.SetComponents(0, 0, 1, 0);
} 

void Matrix::Translate(const Vec3f &v)
{
	row1.b = v.x;
	row2.b = v.y;
	row3.b = v.z;
}

// Transforms a vector by a matrix
Vec3f Matrix::TransformP(const Vec3f &p) const
{
	Vec3f r = Vec3f(0, 0, 0);

	// New point = Matrix * Vec3f
	
	// Matrix is 3 x 4
	// Vec3f is  4 x 1 (x, y, z, 1)

	// Result is 3 x 1

	r.x = p.x * row1.x + p.y * row1.y + p.z * row1.z + row1.b + row4.x;
	r.y = p.x * row2.x + p.y * row2.y + p.z * row2.z + row2.b + row4.y;
	r.z = p.x * row3.x + p.y * row3.y + p.z * row3.z + row3.b + row4.z;
	
	return r;
}

Matrix::Matrix(const Matrix &other)
	 : row1(m4x4[0]), row2(m4x4[1]), row3(m4x4[2]), row4(m4x4[3])
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			m4x4[i][j] = other.m4x4[i][j];
}

void Matrix::Scale(const float f)
{
	row1.x = row2.y = row3.z = f;
}

Vec3f Vec3f::operator*(const Vec3f &v) const {
	return Vec3f(this->values[1] * v.values[2] - this->values[2] * v.values[1], 
				 this->values[2] * v.values[0] - this->values[0] * v.values[2],
				 this->values[0] * v.values[1] - this->values[1] * v.values[0]);
}

Vec3f Vec3f::operator+(const Vec3f &v) const {
	x = values[0];
	y = values[1];
	z = values[2];
	return Vec3f(this->x + v.x, this->y + v.y, this->z + v.z);
}

Vec3f Vec3f::operator-(const Vec3f &v) const {
	return Vec3f(values[0] - v.values[0], values[1] - v.values[1], values[2] - v.values[2]);
}

Vec3f Vec3f::operator=(const Vec3f &other)
{
	values[0] = other.values[0];
	values[1] = other.values[1];
	values[2] = other.values[2];
	return *this;
}

void Vec3f::operator+=(const Vec3f &v)  {
	this->x += v.x; this->y += v.y; this->z += v.z;
}

void Vec3f::operator-=(const Vec3f &v)	{
	this->x -= v.x; this->y -= v.y; this->z -= v.z;
}

Vec3f Vec3f::operator*(const float &s) const {
	return Vec3f(this->x * s, this->y * s, this->z * s);
}

Vec3f Vec3f::operator-(const float &s) const {
	return Vec3f(this->x - s, this->y - s, this->z - s);
}

bool Vec3f::operator!=(const Vec3f &other) const
{
	if (this->x == other.x && this->y == other.y && this->z == other.z)
		return false;
	else 
		return true;
}

bool Vec3f::operator==(const Vec3f &other) const
{
	if (this->x == other.x && this->y == other.y && this->z == other.z)
		return true;
	else 
		return false;
}

// Used to find a point that is t percent along the way from A to B
Vec3f Vec3f::Lerp(const Vec3f &a, const Vec3f &b, float t)
{
	return a + (b - a) * t;
}

float Vec3f::Mag() const
{
	return sqrt(pow(x, 2.0f) + pow(y, 2.0f) + pow(z, 2.0f));
}

Vec3f& Vec3f::Normalize()
{
	float mag = this->Mag();
	if (mag != 0) {
		x /= mag; y /= mag; z /= mag;
	}
	else {
		exit(1);
	}
	return *this;
}

void Vec3f::Zero()
{
      this->x = this->y = this->z = 0.0f;
}

void Vec3f::Abs()
{
      this->x = abs(this->x);
	  this->y = abs(this->y);
	  this->z = abs(this->z);
}

// Tests if a line segment is intersecting with a triangle
bool Vec3f::LineSegmentIntersection(const Vec3f &lineStart, const Vec3f &lineEnd, 
			                const Vec3f &a, const Vec3f &b, const Vec3f &c, 
						    Vec3f &intersectionPt)
{ 
	Vec3f p = lineStart;
	Vec3f q = lineEnd;

	Vec3f pq = q - p;
	Vec3f pa = a - p;
	Vec3f pb = b - p;
	Vec3f pc = c - p;

	float u = ScalarTriple(pq, pc, pb);
	if (u < 0.0f) return false;

	float v = ScalarTriple(pq, pa, pc);
	if (v < 0.0f) return false;

	float w = ScalarTriple(pq, pb, pa);
	if (w < 0.0f) return false;

	float denom = 1.0f / (u + v + w);
	u *= denom;
	v *= denom;
	w *= denom; 
	intersectionPt = a * u + b * v + c * w;
	
	// Adjustment to rayIntersection for line segments
	if (lineStart.y - intersectionPt.y > lineStart.y - lineEnd.y)
		return false;

	return true;
}

// Tests if a ray is intersecting with a triangle
bool Vec3f::RayIntersection(const Vec3f &lineStart, const Vec3f &direction, 
			                const Vec3f &a, const Vec3f &b, const Vec3f &c, 
						    Vec3f &intersectionPt)
{ 
	Vec3f pa = a - lineStart;
	Vec3f pb = b - lineStart;
	Vec3f pc = c - lineStart;

	float u = ScalarTriple(direction, pc, pb);
	if (u < 0.0f) return false;

	float v = ScalarTriple(direction, pa, pc);
	if (v < 0.0f) return false;

	float w = ScalarTriple(direction, pb, pa);
	if (w < 0.0f) return false;

	if (u + v + w == 0)
		return false;

	float denom = 1.0f / (u + v + w);
	u *= denom;
	v *= denom;
	w *= denom; 
	intersectionPt = a * u + b * v + c * w;

	return true;
}
// Assumes CCW order
Vec3f Vec3f::CalculateNormal(const Vec3f &p1, const Vec3f &p2, const Vec3f &p3)
{
	Vec3f u, v;
	u = (p2 - p1).Normalize();
	v = (p3 - p1).Normalize();
	return (u * v).Normalize();
}

float Vec3f::ScalarTriple(const Vec3f &u, const Vec3f &v, const Vec3f &w)
{
	return dot((u * v), w);
}

bool Vec3f::operator<=(const Vec3f &other) const
{
	if (x > other.x)
		return false;
	else if (y > other.y)
		return false;
	else if (z > other.z)
		return false;
	else
		return true;
}

bool Vec3f::operator>=(const Vec3f &other) const
{
	if (x < other.x)
		return false;
	else if (y < other.y)
		return false;
	else if (z < other.z)
		return false;
	else
		return true;
}

Vec3f &Vec3f::operator-()
{
	x = -x; y = -y; z = -z;
	return *this;
}

Vec3f Vec3f::Scale(const Vec3f &other) const
{
	return Vec3f(this->x * other.x, this->y * other.y, this->z * other.z);
}
