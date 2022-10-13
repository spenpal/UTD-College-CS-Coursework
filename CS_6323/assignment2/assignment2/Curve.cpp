/*
 * Filename:	Curve.cpp
 * Date: 		10/12/2022
 * Author: 		Sanjeev Penupala
 * Email: 		sanjeev.penupala@utdallas.edu
 * Course: 		CS 6323.501 Fall 2022
 * Version: 	1.0
 *
 */

#include "Curve.h"


Curve::Curve()
{
}


Curve::~Curve()
{
}


void Curve::init()
{
	this->control_points_pos = {
		{ 0.0, 8.5, -2.0 },
		{ -3.0, 11.0, 2.3 },
		{ -6.0, 8.5, -2.5 },
		{ -4.0, 5.5, 2.8 },
		{ 1.0, 2.0, -4.0 },
		{ 4.0, 2.0, 3.0 },
		{ 7.0, 8.0, -2.0 },
		{ 3.0, 10.0, 3.7 }
	};
}


vector<vec3> Curve::catmull_rom(const mat4x3 &control_points)
{
	vec4 tVec;
	vec3 next_point;
	vector<vec3> segment_pts;

	for (float i = 0; i < num_points_per_segment; i++) {
		float t = i / num_points_per_segment;
		tVec = vec4(t*t*t, t*t, t, 1);

		next_point = control_points * spline_basis_mat * tVec; // catmull-rom matrix formula
		segment_pts.push_back(next_point);
	}

	return segment_pts;
}


void Curve::calculate_curve()
{
	int num_of_cp = control_points_pos.size();
	vec3 P0, P1, P2, P3;

	for (int i = 0; i < num_of_cp; i++) {

		P0 = control_points_pos[((i + 7) % num_of_cp)];
		P1 = control_points_pos[i % num_of_cp];
		P2 = control_points_pos[((i + 1) % num_of_cp)];
		P3 = control_points_pos[((i + 2) % num_of_cp)];

		mat4x3 control_points(P0, P1, P2, P3);
		vector<vec3> segment_pts = catmull_rom(control_points);

		curve_points_pos.insert(curve_points_pos.end(), segment_pts.begin(), segment_pts.end());
		curve_points_pos.push_back(P2);
	}
}
