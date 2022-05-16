#pragma once
#include "Terrain.h"
#include "glutils.h"
#include <glad/glad.h>
#include "glutils.h"

#include <cstdio>
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

class HeightGenerator {
private:
	float amplitude;
	int seed;

	float getNoise(int x, int z) {

		srand((x *152932) + (z *295032) + seed);
		return static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 2.0f - 1.0f;
	}

	float getSmoothNoise(int x, int z) {
		float corners = (getNoise(x - 1, z - 1) + getNoise(x + 1, z - 1) + getNoise(x - 1, z + 1) + getNoise(x+1, z+1)) / 16.0f;

		float sides = (getNoise(x - 1, z) + getNoise(x + 1, z) + getNoise(x, z - 1) + getNoise(x, z + 1)) / 8.0f;

		float center = getNoise(x, z) / 4.0f;

		return (corners + sides + center);
	}

	float getInterpolatedNoise(float x, float z) {
		int intX = (int)x;
		int intZ = (int)z;
		float fracX = x - intX;
		float fracZ = z - intZ;

		float v1 = getSmoothNoise(intX, intZ);
		float v2 = getSmoothNoise(intX+ 1, intZ);
		float v3 = getSmoothNoise(intX, intZ+1);
		float v4 = getSmoothNoise(intX+1, intZ+1);
		float i1 = interpolate(v1, v2, fracX);
		float i2 = interpolate(v3, v4, fracX);

		return interpolate(i1, i2, fracZ);

	}

	float interpolate(float a, float b, float blend) {
		double theta = blend * 3.14159265358979323846f;
		float f = (1.0f - cos(theta)) * 0.5f;
		return a * (1.0f - f) + b * f;

	}

public:
	HeightGenerator()  {
		amplitude = 10.0f;
		seed = (int) static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 1000);
	}

	float generateHeight(int x, int z) {
		float total = getInterpolatedNoise(x/4.0f, z/4.0f) * amplitude;

		//total += getInterpolatedNoise(x / 2.0f, z / 2.0f) * amplitude / 3.0f;
		//total += getInterpolatedNoise(x , z ) * amplitude / 9.0f;
		return total;
	}




};