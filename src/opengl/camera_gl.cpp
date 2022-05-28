#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "opengl/camera_gl.hpp"
#include "controls.hpp"
#include "time.hpp"
#include "lowsystems/engine.hpp"

void CameraGL::update_camera(GLuint program) {
   auto window{ Engine::PrimaryEngine->get_window() };
   constexpr float DistancePerSecond{ 500.0f };
   const float DistanceMoving{ DistancePerSecond * Time::get_delta_time() };

   if (Input::IsControlPressed(Input::Control::Up)) {
      view = glm::translate(view, glm::vec3{ 0.0f, DistanceMoving, 0.0f });
   }

   if (Input::IsControlPressed(Input::Control::Down)) {
      view = glm::translate(view, glm::vec3{ 0.0f, -DistanceMoving, 0.0f });
   }
   
   if (Input::IsControlPressed(Input::Control::Left)) {
      view = glm::translate(view, glm::vec3{ -DistanceMoving, 0.0f, 0.0f });
   }
   
   if (Input::IsControlPressed(Input::Control::Right)) {
      view = glm::translate(view, glm::vec3{ DistanceMoving, 0.0f, 0.0f });
   }

   if (is_orthographic) {
      projection = glm::ortho(0.0f, static_cast<float>(window->get_width()), static_cast<float>(window->get_height()), 0.0f, -1.0f, 1.0f);
   } else {
      projection = glm::mat4{ 1.0f };
   }  

   GLint viewLoc = glGetUniformLocation(program, "view");
   GLint projectionLoc = glGetUniformLocation(program, "projection");

   glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
   glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
}
