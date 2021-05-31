#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "cube.h"
#include "system.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 900;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 4.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
float cameraRadius = 2.0f;
float cameraAngle = 1.0f;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float spaceFrame = 0.0f;
float timeSinceLastSpace = 1.0f;

// pause/start
bool space = false;
// mouse capture
bool mouse = false;

int main()
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Planets", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);

    Shader planet_shader("planet_shader.vs", "planet_shader.fs");
    Shader cube_shader("cube_shader.vs", "cube_shader.fs");

    Planet ourPlanet("Resources/planet/planet.obj", "Planet", planet_shader, glm::vec3(0.1f, 0.1f, 0.1f), 1.0f, 15.0f);
    System ourSystem(ourPlanet);
    ourSystem.addCube("Resources/texture/container.png", "Cube1", cube_shader, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.0f, 0.0f, 1.0f), 4.0f, 2.0f, 5.0f, 0.0f);
    ourSystem.addCube("Resources/texture/container.png", "Cube2", cube_shader, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.0f, 1.0f, 0.0f), 3.0f, 3.0f, 6.5f, 1.0f);
    ourSystem.addCube("Resources/texture/container.png", "Cube3", cube_shader, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.0f, 1.0f, 1.0f), 3.5f, 4.0f, 8.0f, 0.0f);
    ourSystem.addCube("Resources/texture/container.png", "Cube4", cube_shader, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(1.0f, 0.0f, 0.0f), 2.0f, 5.0f, 9.5f, -1.0f);
    ourSystem.addCube("Resources/texture/container.png", "Cube5", cube_shader, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(1.0f, 0.0f, 1.0f), 4.5f, 6.0f, 11.0f, -3.0f);
    ourSystem.addCube("Resources/texture/container.png", "Cube6", cube_shader, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(1.0f, 1.0f, 1.0f), 2.5f, 7.0f, 12.5f, 2.0f);

    // setup imgui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImVec4 clear_color = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);

    // setup imgui binding
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    bool show_gui_window = true;

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        timeSinceLastSpace += deltaTime;
        if (space)
            spaceFrame += deltaTime;

        // input
        processInput(window);

        // render
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourSystem.Draw(projection, view, camera.Position, spaceFrame);
        ourSystem.GUI(&clear_color, &mouse);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // imgui cleanup
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow* window)
{
    // pause/start
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        if (timeSinceLastSpace > 0.3) // if enough time has passed since last press of space
        {
            space = !space;
            timeSinceLastSpace = 0.0f;
        }
    }

    // exit/escape
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // camera up rotation
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessRotation(UP, cameraAngle, cameraRadius);
    // camera down rotation
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessRotation(DOWN, cameraAngle, cameraRadius);
    // camera left rotation
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessRotation(LEFT, cameraAngle, cameraRadius);
    // camera right rotation
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessRotation(RIGHT, cameraAngle, cameraRadius);
    // camera forward movement
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    // camera backward movement
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function execute
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (mouse)
    {
        if (firstMouse)
        {
            lastX = (float)xpos;
            lastY = (float)ypos;
            firstMouse = false;
        }

        float xoffset = (float)(xpos - lastX);
        float yoffset = (float)(lastY - ypos); // reversed since y-coordinates go from bottom to top

        lastX = (float)xpos;
        lastY = (float)ypos;

        camera.ProcessMouseMovement(xoffset, yoffset);
    }
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll((float)yoffset);
}