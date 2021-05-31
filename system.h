#ifndef SYSTEM_H
#define SYSTEM_H

#include "planet.h"
#include "cube.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class System
{
	private:
		Planet planet;
		vector<Cube*> cubes;
	public:
		System(Planet p);
		~System();
		
		void addCube(string const& texture_path, string const& name, Shader shader, glm::vec3 scale, glm::vec3 spin, float rot_step, float spin_step, float radius, float y_pos);
		void Draw(glm::mat4 projection, glm::mat4 view, glm::vec3 camera_position, float time);
		void GUI(ImVec4* clear_color, bool* mouse);
		void checkCubes(int cube_num, float radius, float y_pos);
};

#endif