/*
 * Filename:	Animation.h
 * Date: 		09/22/2022
 * Author: 		Sanjeev Penupala
 * Email: 		sanjeev.penupala@utdallas.edu
 * Course: 		CS 6323.501 Fall 2022
 * Version: 	1.0
 *
 */

#pragma once

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Animation
{
public:

	// Variables
	int angle_a, angle_b;
	float new_angle_a, new_angle_b;

	glm::vec3 local_x_axis = glm::vec3(1, 0, 0);
	glm::vec3 global_y_axis = glm::vec3(0, 1, 0);
	glm::vec3 translation_vec = glm::vec3(5.0f, 0.0f, 0.0f);
	glm::vec3 current_translation_vec;

	// Methods
	Animation();
	~Animation();

	void init();
	void update(float delta_time);
	void reset();
	void rotateLocalXAxis();
	void rotateGlobalYAxis();

	glm::mat4 get_model_mat() { return m_model_mat; };

private:
	glm::mat4 m_model_mat;
};

