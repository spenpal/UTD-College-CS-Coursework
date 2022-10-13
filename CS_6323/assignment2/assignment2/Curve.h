/*
 * Filename:	Curve.h
 * Date: 		10/12/2022
 * Author: 		Sanjeev Penupala
 * Email: 		sanjeev.penupala@utdallas.edu
 * Course: 		CS 6323.501 Fall 2022
 * Version: 	1.0
 *
 */

#pragma once
#include <vector>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;
using namespace std;

class Curve
{
public:
	Curve();
	~Curve();

	void init();
	void calculate_curve();

	float spline_basis_arr[16] = { -0.5, 1.5,-1.5, 0.5,
									1.0, -2.5, 2.0, -0.5,
									-0.5, 0.0, 0.5, 0.0,
									 0.0, 1.0, 0.0, 0.0 };
	mat4 spline_basis_mat = make_mat4(spline_basis_arr);

	vector<vec3> catmull_rom(const mat4x3 &control_points);

public:
	float num_points_per_segment = 200.0f;

	vector<vec3> control_points_pos;
	vector<vec3> curve_points_pos;
};