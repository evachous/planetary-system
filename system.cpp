#include "system.h"

System::System(Planet p) : planet(p)
{

}

System::~System()
{
	for (int i = 0; i < cubes.size(); i++)
		delete cubes[i];
}

void System::addCube(string const& texture_path, string const& name, Shader shader, glm::vec3 scale, glm::vec3 spin, float rot_step, float spin_step, float radius, float y_pos)
{
	Cube* newCube = new Cube(texture_path, name, shader, scale, spin, rot_step, spin_step, radius, y_pos);
	cubes.push_back(newCube);
}

void System::Draw(glm::mat4 projection, glm::mat4 view, glm::vec3 camera_position, float time)
{
	planet.drawPlanet(projection, view, time);
	for (int i = 0; i < cubes.size(); i++)
		cubes[i]->drawCube(projection, view, camera_position, planet.getAngle(), planet.getRadius(), time);
}

void System::GUI(ImVec4* clear_color, bool* mouse)
{
    // start the imgui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    static float planet_rotation = planet.getRotationStep();
    static float planet_radius = planet.getRadius();
    static float cube_rotations[] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
    static float cube_spins[] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
    static float cube_radius[] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
    static float y_positions[] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

    // initialize all cube properties
    for (int i = 0; i < cubes.size(); i++)
    {
        cube_rotations[i] = cubes[i]->getRotationStep();
        cube_spins[i] = cubes[i]->getSpinStep();
        cube_radius[i] = cubes[i]->getRadius();
        y_positions[i] = cubes[i]->getYPosition();
    }
    
    ImGui::Begin("Configuration", NULL);
    ImGui::SetWindowSize(ImVec2(400.0f, 400.0f));
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Checkbox("Capture mouse", mouse);
    ImGui::ColorEdit3("Clear color", (float*)clear_color);

    if (ImGui::CollapsingHeader("Planet"))
    {
        ImGui::InputFloat("Rotation step", &planet_rotation, 0.1f);
        planet.setRotationStep(planet_rotation);
        ImGui::InputFloat("Rotation radius", &planet_radius, 0.5f);
        planet.setRadius(planet_radius);
    }

    if (ImGui::CollapsingHeader("Cubes"))
    {
        for (int i = 0; i < cubes.size(); i++)
        {
            if (ImGui::TreeNode(cubes[i]->getName()))
            {
                ImGui::InputFloat("Rotation step", &cube_rotations[i], 0.1f);
                cubes[i]->setRotationStep(cube_rotations[i]);
                ImGui::InputFloat("Spin step", &cube_spins[i], 0.1f);
                cubes[i]->setSpinStep(cube_spins[i]);
                ImGui::InputFloat("Rotation radius", &cube_radius[i], 0.1f);
                ImGui::InputFloat("Y position", &y_positions[i], 0.1f);
                checkCubes(i, cube_radius[i], y_positions[i]);
                ImGui::TreePop();
            }
        }
    }

    ImGui::End();

    // rendering imgui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void System::checkCubes(int cube_num, float radius, float y_pos)
{
    for (int i = 0; i < cubes.size(); i++)
    {
        if (i != cube_num)  // check all other 5 cubes
        {
            // if one of these restrictions fail, changes to cube's properties are invalid
            if (abs(y_pos - cubes[i]->getYPosition()) < 1.5f && abs(radius - cubes[i]->getRadius()) < 1.5f)
            {
                ImGui::Text("!!! Cube overlap.");
                cubes[cube_num]->setRadius(cubes[cube_num]->getRadius());
                cubes[cube_num]->setYPosition(cubes[cube_num]->getYPosition());
                return;
            }
            if (radius < 4.0f)
            {
                ImGui::Text("!!! Cube too close to planet.");
                cubes[cube_num]->setRadius(cubes[cube_num]->getRadius());
                cubes[cube_num]->setYPosition(cubes[cube_num]->getYPosition());
                return;
            }
            if (abs(y_pos) > 4.0f)
            {
                ImGui::Text("!!! Cube too far away from planet.");
                cubes[cube_num]->setRadius(cubes[cube_num]->getRadius());
                cubes[cube_num]->setYPosition(cubes[cube_num]->getYPosition());
                return;
            }
        }
    }
    // changes to cube's properties are valid
    cubes[cube_num]->setRadius(radius);
    cubes[cube_num]->setYPosition(y_pos);
}