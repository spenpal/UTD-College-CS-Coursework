/*
 * Filename:	Animation.cpp
 * Date: 		09/22/2022
 * Author: 		Sanjeev Penupala
 * Email: 		sanjeev.penupala@utdallas.edu
 * Course: 		CS 6323.501 Fall 2022
 * Version: 	1.0
 *
 */

#include "Animation.h"

Animation::Animation()
{
	this->m_model_mat = glm::mat4(1.0f);
}

Animation::~Animation()
{
}

void Animation::init()
{
	reset();
}

void Animation::update(float delta_time)
{
}

void Animation::reset()
{
	angle_a = 0, angle_b = 0;
	new_angle_a = 0, new_angle_b = 0;

	current_translation_vec = translation_vec;

	m_model_mat = glm::mat4(1.0f);
	m_model_mat = glm::translate(m_model_mat, translation_vec);
}

void Animation::rotateLocalXAxis()
{
	new_angle_a += (float) angle_a;

	glm::mat4 rotated_model_mat = glm::mat4();
	rotated_model_mat = glm::translate(rotated_model_mat, current_translation_vec);
	rotated_model_mat = glm::rotate(rotated_model_mat, glm::radians(new_angle_b), global_y_axis);
	rotated_model_mat = glm::rotate(rotated_model_mat, glm::radians(new_angle_a), local_x_axis);

	m_model_mat = rotated_model_mat;
}

void Animation::rotateGlobalYAxis()
{
	new_angle_b += (float) angle_b;

	float sin_of_angle_b = glm::sin(glm::radians(new_angle_b));
	float cos_of_angle_b = glm::cos(glm::radians(new_angle_b));
	float y_axis_rotation_arr[9] = {cos_of_angle_b,	0,	-sin_of_angle_b,
									0,				1,	0,
									sin_of_angle_b,	0,	cos_of_angle_b };

	glm::mat3 y_axis_rotation_mat = glm::make_mat3(y_axis_rotation_arr);
	current_translation_vec = y_axis_rotation_mat * translation_vec;

	glm::mat4 rotated_model_mat = glm::mat4();
	rotated_model_mat = glm::translate(rotated_model_mat, current_translation_vec);
	rotated_model_mat = glm::rotate(rotated_model_mat, glm::radians(new_angle_b), global_y_axis);
	rotated_model_mat = glm::rotate(rotated_model_mat, glm::radians(new_angle_a), local_x_axis);

	m_model_mat = rotated_model_mat;
}

