#include "cube.h"
#include <GLFW/glfw3.h>

Cube::Cube(string const& texture_path, string const& n, Shader shader, glm::vec3 sc, glm::vec3 sp, float rot_s, float spin_s, float r, float y) : name(n), cube_shader(shader), scale(sc), spin(sp), rot_step(rot_s), spin_step(spin_s), radius(r), y_pos(y)
{
    rot_angle = 0.0;
    spin_angle = 0.0;

    float vertices[] = {
        // positions          // normals           // texture coords
        -0.3f, -0.3f, -0.3f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.3f, -0.3f, -0.3f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.3f,  0.3f, -0.3f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.3f,  0.3f, -0.3f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.3f,  0.3f, -0.3f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.3f, -0.3f, -0.3f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.3f, -0.3f,  0.3f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.3f, -0.3f,  0.3f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.3f,  0.3f,  0.3f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.3f,  0.3f,  0.3f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.3f,  0.3f,  0.3f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.3f, -0.3f,  0.3f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.3f,  0.3f,  0.3f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.3f,  0.3f, -0.3f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.3f, -0.3f, -0.3f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.3f, -0.3f, -0.3f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.3f, -0.3f,  0.3f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.3f,  0.3f,  0.3f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.3f,  0.3f,  0.3f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.3f,  0.3f, -0.3f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.3f, -0.3f, -0.3f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.3f, -0.3f, -0.3f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.3f, -0.3f,  0.3f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.3f,  0.3f,  0.3f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.3f, -0.3f, -0.3f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.3f, -0.3f, -0.3f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.3f, -0.3f,  0.3f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.3f, -0.3f,  0.3f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.3f, -0.3f,  0.3f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.3f, -0.3f, -0.3f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.3f,  0.3f, -0.3f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.3f,  0.3f, -0.3f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.3f,  0.3f,  0.3f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.3f,  0.3f,  0.3f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.3f,  0.3f,  0.3f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.3f,  0.3f, -0.3f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    loadTexture(texture_path);
}

Cube::~Cube()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Cube::drawCube(glm::mat4 projection, glm::mat4 view, glm::vec3 camera_position, float planet_angle, float planet_radius, float time)
{
    glBindTexture(GL_TEXTURE_2D, texture);

    // current rotation and spin angles
    rot_angle = rot_step * time;
    spin_angle = spin_step * time;

    cube_shader.use();

    // calculate current planet position and cube position relative to the planet
    glm::vec3 planet_position = glm::vec3(sin(planet_angle) * planet_radius, 0.0f, cos(planet_angle) * planet_radius);
    glm::vec3 cube_position = glm::vec3(planet_position.x + (sin(rot_angle) * radius), y_pos, planet_position.z + (cos(rot_angle) * radius));

    // light properties
    cube_shader.setVec3("light.position", planet_position-cube_position); // light cube's side that is facing the planet
    cube_shader.setVec3("viewPos", camera_position);

    cube_shader.setVec3("light.ambient", 0.25f, 0.25f, 0.25f);
    cube_shader.setVec3("light.diffuse", 1.8f, 1.8f, 1.8f);
    cube_shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
    cube_shader.setFloat("light.constant", 1.0f);
    cube_shader.setFloat("light.linear", 0.09f);
    cube_shader.setFloat("light.quadratic", 0.032f);

    // material properties
    cube_shader.setFloat("material.shininess", 32.0f);

    // view/projection transformations
    cube_shader.setMat4("projection", projection);
    cube_shader.setMat4("view", view);

    // world transformation
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, scale);	// scale cube down
    model = glm::translate(model, cube_position);   // move cube to its position near the planet
    model = glm::rotate(model, spin_angle, spin);   // rotate cube around itself
    cube_shader.setMat4("model", model);

    // render cube
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Cube::loadTexture(string const& texture_path)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    unsigned char* data = stbi_load(texture_path.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        cout << "Failed to load texture" << endl;
    }
    stbi_image_free(data);
}

const char* Cube::getName()
{
    return name.c_str();
}

float Cube::getRotationStep()
{
    return rot_step;
}

float Cube::getSpinStep()
{
    return spin_step;
}

float Cube::getRadius()
{
    return radius;
}

float Cube::getYPosition()
{
    return y_pos;
}

void Cube::setRotationStep(float s)
{
    rot_step = s;
}

void Cube::setSpinStep(float s)
{
    spin_step = s;
}

void Cube::setRadius(float r)
{
    radius = r;
}

void Cube::setYPosition(float y)
{
    y_pos = y;
}