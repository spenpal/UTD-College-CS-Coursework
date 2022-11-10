/*
 * Filename:	Bone_Animation.h
 * Date: 		11/10/2022
 * Author: 		Sanjeev Penupala
 * Email: 		sanjeev.penupala@utdallas.edu
 * Course: 		CS 6323.501 Fall 2022
 * Version: 	1.0
 *
 */

#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>	
#include <glm/gtx/euler_angles.hpp>
//#include <glm/gtx/string_cast.hpp>

using namespace std;
using namespace glm;

class Bone_Animation
{
public:
	Bone_Animation();
	~Bone_Animation();

	void init();
	void update(float delta_time);
	void reset();

	void rotate();
	void jacobianTranspose();

public:

	bool moving;
	int num_of_bones;
	int num_of_joints;

	float threshold = 1e-4;
	float distance;

	// Here the head of each vector is the root bone
	vector<vec3> scale_vector;
	vector<vec3> rotation_degree_vector;
	vector<vec4> bone_colors;

	vec3 root_position;
	vec3 end_effector;
	vec3 target_position;
	vec4 target_color;

	vector<mat4> rotate_y;
	vector<mat4> rotate_z;
	vector<mat4> rotate_x;

	vector<vec3> pivot;

	vector<vec3> y_axis;
	vector<vec3> z_axis;
	vector<vec3> x_axis;

	vector<mat4> bone_mat =					{mat4(1.0f), mat4(1.0f), mat4(1.0f), mat4(1.0f)};
	vector<mat4> translate_origin_mat =		{mat4(1.0f), mat4(1.0f), mat4(1.0f), mat4(1.0f)};
	vector<mat4> translate_bone2bone_mat =	{mat4(1.0f), mat4(1.0f), mat4(1.0f), mat4(1.0f)};
	vector<mat4> translate_end_eff_mat =	{mat4(1.0f), mat4(1.0f), mat4(1.0f), mat4(1.0f)};
	vector<mat4> rotate_mat =				{mat4(1.0f), mat4(1.0f), mat4(1.0f), mat4(1.0f)};
	vector<mat4> world_mat =				{mat4(1.0f), mat4(1.0f), mat4(1.0f), mat4(1.0f)};
};

