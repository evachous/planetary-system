#ifndef CUBE_H
#define CUBE_H

#include <learnopengl/shader.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

class Cube
{
	private:
		string name;
		Shader cube_shader;
		glm::vec3 scale, spin;
		float rot_step, spin_step, rot_angle, spin_angle, radius, y_pos;
		unsigned int VBO, VAO, texture;
	public:
		Cube(string const& texture_path, string const& n, Shader shader, glm::vec3 sc, glm::vec3 sp, float rot_s, float spin_s, float r, float y);
		~Cube();
		void drawCube(glm::mat4 projection, glm::mat4 view, glm::vec3 camera_position, float planet_angle, float planet_radius, float time);
		void loadTexture(string const& texture_path);
		const char* getName();
		float getRotationStep();
		float getSpinStep();
		float getRadius();
		float getYPosition();
		void setRotationStep(float s);
		void setSpinStep(float s);
		void setRadius(float r);
		void setYPosition(float y);
};

#endif