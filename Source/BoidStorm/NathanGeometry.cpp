// Fill out your copyright notice in the Description page of Project Settings.

#include "BoidStorm.h"
#include "NathanGeometry.h"
#include <cmath>

Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z) { }

Vec3 Vec3::cross(Vec3 vec) {
	return Vec3(((y * vec.z) - (z * vec.y)), ((z * vec.x) - (x * vec.z)), ((x * vec.y) - (y * vec.x)));}
Vec3 Vec3::truncate(float s) {
	if (length() > s) {
		Vec3 toReturn = normalized();
		return s * toReturn;
	}
	return Vec3(x, y, z);
}

Vec3 Vec3::operator-() {
	return Vec3(-x, -y, -z);}

float Vec3::length() {
	return sqrt((x * x) + (y * y) + (z * z));}

Vec3 Vec3::normalized() {
	float len = length();
	return Vec3(x / len, y / len, z / len);}

Vec3 Vec3::squared() {
	return Vec3(x * x, y * y, z * z);}

float operator*(Vec3 v1, Vec3 v2) {
	return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);}

Vec3 operator*(float scalar, Vec3 v) {
	return Vec3(scalar * v.x, scalar * v.y, scalar * v.z);}

Vec3 operator+(Vec3 v1, Vec3 v2) {
	return Vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);}

Vec3 operator-(Vec3 v1, Vec3 v2) {
	return Vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);}

bool operator==(Vec3 v1, Vec3 v2) {
	return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;}

Rotation::Rotation(float roll, float pitch, float yaw) : roll(roll), pitch(pitch), yaw(yaw) { }
Vec3 operator*(Rotation r, Vec3 v) {

	float y1 = (v.y * cosf(r.roll)) - (v.z * sinf(r.roll));
	float z1 = (v.y * sinf(r.roll)) + (v.z * cosf(r.roll));

	float x2 = (v.x * cosf(r.pitch)) + (z1 * sinf(r.pitch));
	float z2 = -(v.x * cosf(r.pitch)) + (y1 * sinf(r.pitch));

	float x3 = (x2 * cosf(r.yaw)) - (y1 * sinf(r.yaw));
	float y3 = (x2 * sinf(r.yaw)) - (y1 * cosf(r.yaw));

	return Vec3(x3, y3, z2);

}

