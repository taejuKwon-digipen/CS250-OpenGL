#pragma once
#include <cmath>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

using Vec2 = glm::vec2;
using Vec3 = glm::vec3;
using Vec4 = glm::vec4;
using Mat4 = glm::mat4;

const float PI = 3.14159f;
const float TWO_PI = 2.0f * PI;
const float HALF_PI = 0.5f * PI;
const float QUARTER_PI = 0.25f * PI;
const float EIGTH_PI = 0.125F * PI;
const float SIXTEENTH_PI = 0.0625F * PI;

const float DEG_TO_RAO = PI / 180.0f;
const float RAD_TO_DEG = 180.0f / PI;

const float EPSILON = 0.00001f;

const int XINDEX = 0;
const int YINDEX = 1;
const int ZINDEX = 2;

const Vec3 XAXIS(1.0f, 0.0f, 0.0f);
const Vec3 YAXIS(0.0f, 1.0f, 0.0f);
const Vec3 ZAXIS(0.0f, 0.0f, 1.0f);

const int NUM_STEPS_PI = 36;
const int TWO_NUM_STEPS_PI = 2 * NUM_STEPS_PI;
const float ONE_STEP = PI / NUM_STEPS_PI;

static float length_(Vec3 v) {
	return v.x * v.x + v.y * v.y + v.z * v.z;
}

static Mat4 Scale(float sx, float sy, float sz)
{
	return scale(Mat4(1.0f), Vec3(sx, sy, sz));
}

static Mat4 Scale(const Vec3& s)
{
	return scale(Mat4(1.0f), s);
}

static glm::mat4 Rotate(float angle, const glm::vec3& axis)
{
	return rotate(glm::mat4(1.0f), angle, axis);
}

static Mat4 Rotate(float angle, float ax, float ay, float az)
{
	return rotate(Mat4(1.0f), angle, Vec3(ax, ay, az));
}

static Mat4 Translate(const Vec3& t)
{
	return translate(Mat4(1.0f), t);
}

static Mat4 Frustum(float left, float right, float bottom, float top, float near, float far)
{
	return glm::frustum(left, right, bottom, top, near, far);
}

static Mat4 LookAt(const Vec3& eye, const Vec3& center, const Vec3& up)
{
	return lookAt(eye, center, up);
}

static Mat4 LookAtOrigin(float radius, float alpha, float beta)
{
	Vec4 position{ radius, 0.f, 0.f, 1.f };
	Vec3 up{ 0.f, 1.f, 0.f };

	Mat4 rotation = rotate(Mat4{ 1 }, alpha, Vec3{ 0.f, 0.f, -1.f });
	rotation = rotation * rotate(Mat4{ 1 }, beta, Vec3{ 0.f, 1.f, 0.f });

	Vec3 left = cross(Vec3{ position }, up);
	up = cross(left, Vec3{ position });

	return LookAt(Vec3{ position }, Vec3{ 0,0,0 }, up);
}

template <typename T>
static T Normalize(T vec)
{
	return glm::normalize(vec);
}

template <typename T>
static T Cross(T vec1, T vec2)
{
	return glm::cross(vec1, vec2);
}

template <typename T>
static float const* ValuePtr(T const& value)
{
	return glm::value_ptr(value);
}

static float RoundDecimal(float input)
{
	return std::floor(input * 10000.0f + 0.5f) / 10000.0f;
}

static Vec3 RoundDecimal(const Vec3& input)
{
	return Vec3(RoundDecimal(input[0]), RoundDecimal(input[1]), RoundDecimal(input[2]));
}

static Vec3 RoundDecimal(const Vec4& input)
{
	return Vec4(RoundDecimal(input[0]), RoundDecimal(input[1]), RoundDecimal(input[2]), RoundDecimal(input[3]));
}

static bool DegenerateTri(const Vec3& v0, const Vec3& v1, const Vec3& v2)
{
	return(distance(v0, v1) < EPSILON || distance(v1, v2) < EPSILON || distance(v2, v0) < EPSILON);
}