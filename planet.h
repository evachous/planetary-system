#ifndef PLANET_H
#define PLANET_H

#include <learnopengl/shader.h>
#include <learnopengl/model.h>
#include <GLFW/glfw3.h>

class Planet : public Model
{
	private:
		string name;
		Shader planet_shader;
		glm::vec3 scale;
		float step, angle, radius;
	public:
		Planet(string const& model_path, string const& n, Shader shader, glm::vec3 sc, float st, float r);
		~Planet();
		void drawPlanet(glm::mat4 projection, glm::mat4 view, float time);
		float getAngle();
		float getRotationStep();
		float getRadius();
		void setRotationStep(float s);
		void setRadius(float r);
};

#endif