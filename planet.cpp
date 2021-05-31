#include "planet.h"

Planet::Planet(string const& model_path, string const& n, Shader shader, glm::vec3 sc, float st, float r) : Model(model_path), planet_shader(shader), scale(sc), step(st), angle(st), radius(r)
{

}

Planet::~Planet()
{

}

void Planet::drawPlanet(glm::mat4 projection, glm::mat4 view, float time)
{
    planet_shader.use();

    // view/projection transformations
    planet_shader.setMat4("projection", projection);
    planet_shader.setMat4("view", view);

    angle = step * time; // current rotation angle
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, scale);   // scale planet down
    model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f)); // rotate planet around y axis
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, radius));
    planet_shader.setMat4("model", model);
    this->Draw(planet_shader);
}

float Planet::getAngle()
{
    return angle;
}

float Planet::getRotationStep()
{
    return step;
}

float Planet::getRadius()
{
    return radius;
}

void Planet::setRotationStep(float s)
{
    step = s;
}

void Planet::setRadius(float r)
{
    radius = r;
}