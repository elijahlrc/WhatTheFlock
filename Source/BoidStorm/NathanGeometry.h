// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <tuple>

class Vec3 {
public:
	float x;
	float y;
	float z;

	Vec3(float, float, float);
	Vec3() : x(0.0), y(0.0), z(0.0) { }

	float length();
	Vec3 squared();
	Vec3 normalized();
	Vec3 cross(Vec3 vec);
	Vec3 truncate(float s);

	Vec3 operator-();
};

class Rotation {
public:
	float roll;
	float pitch;
	float yaw;

	Rotation(float, float, float);
	Rotation() : roll(0.0), pitch(0.0), yaw(0.0) { }
};

float operator*(Vec3 v1, Vec3 v2); // Dot product
Vec3 operator*(float scalar, Vec3 vec);

Vec3 operator+(Vec3 v1, Vec3 v2);
Vec3 operator-(Vec3 v1, Vec3 v2);
bool operator==(Vec3 v1, Vec3 v2);

Vec3 operator*(Rotation r, Vec3 v); // Apply rotation

