/*
 * Filename:	Bone_Animation.cpp
 * Date: 		11/10/2022
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
	root_position = {2.0f,0.5f,2.0f};
	target_position = {3.0f,8.0f,3.0f};

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
		{0.0f,30.0f,0.0f},
		{0.0f,30.0f,0.0f},
		{0.0f,30.0f,0.0f}
	};

	bone_colors = 
	{
		{0.7f,0.0f,0.0f,1.0f},
		{0.7f,0.7f,0.0f,1.0f},
		{0.7f,0.0f,0.7f,1.0f},
		{0.0f,0.7f,0.7f,1.0f}
	};

	target_color = {0.0f,1.0f,0.0f,1.0f}; // green

	moving = false;
	num_of_bones = bone_colors.size();
	num_of_joints = num_of_bones-1;

	rotate();
}


void Bone_Animation::update(float delta_time)
{
	//cout << glm::to_string(end_effector) << endl;
	//cout << distance << endl;
	pivot.clear();
	x_axis.clear();
	y_axis.clear();
	z_axis.clear();

	rotate();

	if (moving)
	{
		jacobianTranspose();
	}
}


void Bone_Animation::rotate()
{
	rotate_y = { eulerAngleY(radians(rotation_degree_vector[0][0])), 
				 eulerAngleY(radians(rotation_degree_vector[1][0])),
				 eulerAngleY(radians(rotation_degree_vector[2][0])),
				 eulerAngleY(radians(rotation_degree_vector[3][0])) };

	rotate_z = { eulerAngleZ(radians(rotation_degree_vector[0][1])),
				 eulerAngleZ(radians(rotation_degree_vector[1][1])),
				 eulerAngleZ(radians(rotation_degree_vector[2][1])),
				 eulerAngleZ(radians(rotation_degree_vector[3][1])) };

	rotate_x = { eulerAngleX(radians(rotation_degree_vector[0][2])),
				 eulerAngleX(radians(rotation_degree_vector[1][2])),
				 eulerAngleX(radians(rotation_degree_vector[2][2])),
				 eulerAngleX(radians(rotation_degree_vector[3][2])) };

	for (int i = 0; i < num_of_bones; i++)
	{
		rotate_mat[i] = rotate_x[i] * rotate_z[i] * rotate_y[i];

		translate_origin_mat[i]  =	translate(mat4(1.0f), {0, scale_vector[i][1]/2.0f, 0});
		translate_end_eff_mat[i] =	translate(mat4(1.0f), {0, scale_vector[i][1], 0});

		if (i == 0) // root
		{
			translate_bone2bone_mat[i] = translate(mat4(1.0f), {root_position[0], (root_position[1]-scale_vector[i][1] / 2.0f), root_position[2]});

			world_mat[i] = translate_bone2bone_mat[i] * rotate_mat[i];
			bone_mat[i] = translate(mat4(1.0f), root_position);
		}
		else
		{
			translate_bone2bone_mat[i] = translate(mat4(1.0f), {0, scale_vector[i-1][1], 0});

			world_mat[i] = world_mat[i-1] * translate_bone2bone_mat[i] * rotate_mat[i];
			bone_mat[i] = world_mat[i] * translate_origin_mat[i];

			pivot.push_back(vec3(world_mat[i-1] * translate_end_eff_mat[i-1] * vec4(0.0f, 0.0f, 0.0f, 1.0f)));

			y_axis.push_back(normalize(vec3(world_mat[i-1] * rotate_x[i] * rotate_z[i] * vec4(0.0f, 1.0f, 0.0f, 0.0f))));
			z_axis.push_back(normalize(vec3(world_mat[i-1] * rotate_x[i] * vec4(0.0f, 0.0f, 1.0f, 0.0f))));
			x_axis.push_back(normalize(vec3(world_mat[i-1] * vec4(1.0f, 0.0f, 0.0f, 0.0f))));
		}
	}

	end_effector = world_mat[num_of_joints] * translate_end_eff_mat[num_of_joints] * vec4(0.0f, 0.0f, 0.0f, 1.0f);
	distance = glm::distance(target_position, end_effector);
}


void Bone_Animation::jacobianTranspose()
{
	if (distance > threshold)
	{
		vector<mat3> jacob_transposed_mats;
		float alpha_num = 0.0f;
		vec3 alpha_den_vec = {0.0f,0.0f,0.0f};
		vec3 dist_vec = target_position - end_effector;

		for (int i = 0; i < num_of_joints; i++)
		{
			mat3 jacob_mat = mat3(cross(x_axis[i], (end_effector - pivot[i])),
								  cross(y_axis[i], (end_effector - pivot[i])),
								  cross(z_axis[i], (end_effector - pivot[i])));
			mat3 transposed_jacob_mat = transpose(jacob_mat);

			vec3 alpha_num_part = transposed_jacob_mat * dist_vec;
			alpha_num += dot(alpha_num_part, alpha_num_part);
			alpha_den_vec += jacob_mat * alpha_num_part;

			jacob_transposed_mats.push_back(transposed_jacob_mat);
		}

		float alpha_den = dot(alpha_den_vec, alpha_den_vec);
		float alpha = alpha_num / alpha_den;

		for (int i = 0; i < num_of_joints; i++)
		{
			vec3 delta_theta = alpha * jacob_transposed_mats[i] * dist_vec;
			rotation_degree_vector[i+1] += vec3(delta_theta[1], delta_theta[2], delta_theta[0]);
		}
	}
}


void Bone_Animation::reset()
{
	rotation_degree_vector =
	{
		{0.0f,0.0f,0.0f},
		{0.0f,30.0f,0.0f},
		{0.0f,30.0f,0.0f},
		{0.0f,30.0f,0.0f}
	};

	moving = false;
	bone_mat = {mat4(1.0f), mat4(1.0f), mat4(1.0f), mat4(1.0f)};

	target_position = {3.0f,8.0f,3.0f};
}