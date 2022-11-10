/*
 * Filename:	Bone_Animation.cpp
 * Date: 		10/30/2022
 * Author: 		Sanjeev Penupala
 * Email: 		sanjeev.penupala@utdallas.edu
 * Course: 		CS 6323.501 Fall 2022
 * Version: 	1.0
 *
 */

#include "Bone_Animation.h"


Bone_Animation::Bone_Animation()
{
}


Bone_Animation::~Bone_Animation()
{
}

void Bone_Animation::init()
{
	root_position = { 2.0f,1.0f,2.0f };

	scale_vector =
	{
		{1.0f,1.0f,1.0f},
		{0.5f,4.0f,0.5f},
		{0.5f,3.0f,0.5f},
		{0.5f,2.0f,0.5f}
	};

	rotation_degree_vector = 
	{
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f}
	};

	colors = 
	{
		{0.7f,0.0f,0.0f,1.0f},
		{0.7f,0.7f,0.0f,1.0f},
		{0.7f,0.0f,0.7f,1.0f},
		{0.0f,0.7f,0.7f,1.0f}
	};

	num_of_bones = scale_vector.size();
}

void Bone_Animation::update(float delta_time)
{
	angle_y = { radians(rotation_degree_vector[0][0]), radians(rotation_degree_vector[1][0]), radians(rotation_degree_vector[2][0]), radians(rotation_degree_vector[3][0]) };
	angle_z = { radians(rotation_degree_vector[0][1]), radians(rotation_degree_vector[1][1]), radians(rotation_degree_vector[2][1]), radians(rotation_degree_vector[3][1]) };
	angle_x = { radians(rotation_degree_vector[0][2]), radians(rotation_degree_vector[1][2]), radians(rotation_degree_vector[2][2]), radians(rotation_degree_vector[3][2]) };

	rotate_y = { eulerAngleX(angle_x[0]), eulerAngleY(angle_y[1]), eulerAngleY(angle_y[2]), eulerAngleY(angle_y[3]) };
	rotate_z = { eulerAngleX(angle_x[0]), eulerAngleZ(angle_z[1]), eulerAngleZ(angle_z[2]), eulerAngleZ(angle_z[3]) };
	rotate_x = { eulerAngleX(angle_x[0]), eulerAngleX(angle_x[1]), eulerAngleX(angle_x[2]), eulerAngleX(angle_x[3]) };

	for (int i = 0; i < num_of_bones; i++)
	{
		rotate_mat[i] = rotate_x[i] * rotate_z[i] * rotate_y[i];

		if (i == 0) // root bone
		{
			translate_bone2bone_mat[i] = translate(mat4(1.0f), {root_position[0], (root_position[1] - scale_vector[i][1]/2.0f), root_position[2]});
			world_mat[i] = translate_bone2bone_mat[i] * rotate_mat[i];

			bone_mat[i] = translate(mat4(1.0f), root_position);
		}
		else
		{
			translate_origin_mat[i] = translate(mat4(1.0f), {0, scale_vector[i][1]/2.0f, 0});
			translate_bone2bone_mat[i] = translate(mat4(1.0f), {0, scale_vector[i-1][1], 0});
			world_mat[i] = world_mat[i-1] * translate_bone2bone_mat[i] * rotate_mat[i];

			bone_mat[i] = world_mat[i] * translate_origin_mat[i];
		}
	}
}

void Bone_Animation::reset()
{
	rotation_degree_vector =
	{
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f}
	};

	bone_mat = {mat4(1.0f), mat4(1.0f), mat4(1.0f), mat4(1.0f)};
}

