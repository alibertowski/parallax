#include "opengl/camera.hpp"
#include "controls.hpp"
#include "time.hpp"
#include "engine.hpp"

#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/matrix_transform.hpp>

void Camera2::update_camera(GLuint program) {
   auto window{ Engine::PrimaryEngine->get_window() };
   constexpr float DistancePerSecond{ 1.0f };
   const float DistanceMoving{ DistancePerSecond * Time::get_delta_time() };

   if (Input::IsControlPressed(Input::Control::Up)) {
      model = glm::translate(model, glm::vec3{ 0.0f, DistanceMoving, 0.0f });
   }

   if (Input::IsControlPressed(Input::Control::Down)) {
      model = glm::translate(model, glm::vec3{ 0.0f, -DistanceMoving, 0.0f });
   }
   
   if (Input::IsControlPressed(Input::Control::Left)) {
      model = glm::translate(model, glm::vec3{ -DistanceMoving, 0.0f, 0.0f });
   }
   
   if (Input::IsControlPressed(Input::Control::Right)) {
      model = glm::translate(model, glm::vec3{ DistanceMoving, 0.0f, 0.0f });
   }

   projection = glm::ortho(0.0f, static_cast<float>(window->get_width()), static_cast<float>(window->get_height()), 0.0f, -9999.0f, 9999.0f);

   GLint modelLoc = glGetUniformLocation(program, "model");
   GLint viewLoc = glGetUniformLocation(program, "view");
   GLint projectionLoc = glGetUniformLocation(program, "projection");

   glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
   glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
   glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
}