/*
 * Filename:	Bone_Animation.h
 * Date: 		10/30/2022
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

public:

	// Here the head of each vector is the root bone
	vector<vec3> scale_vector;
	vector<vec3> rotation_degree_vector;
	vector<vec4> colors;

	vec3 root_position;

	int num_of_bones;

	vector<float> angle_y;
	vector<float> angle_z;
	vector<float> angle_x;

	vector<mat4> rotate_y;
	vector<mat4> rotate_z;
	vector<mat4> rotate_x;

	vector<mat4> bone_mat = { mat4(1.0f), mat4(1.0f), mat4(1.0f), mat4(1.0f) };
	vector<mat4> translate_origin_mat = { mat4(1.0f), mat4(1.0f), mat4(1.0f), mat4(1.0f) };
	vector<mat4> translate_bone2bone_mat = { mat4(1.0f), mat4(1.0f), mat4(1.0f), mat4(1.0f) };
	vector<mat4> rotate_mat = { mat4(1.0f), mat4(1.0f), mat4(1.0f), mat4(1.0f) };
	vector<mat4> world_mat = { mat4(1.0f), mat4(1.0f), mat4(1.0f), mat4(1.0f) };
};

