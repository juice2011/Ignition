#include "components/rendering/meshrenderer.hpp"
#include "GLFW/glfw3.h"

#include <iostream>
namespace Ignition::Rendering {
   void MeshRenderer::LoadModel(Model m)
   {
      unsigned int VBO, EBO;
      this->model = m;
      glGenVertexArrays(1, &this->vao);
      glGenBuffers(1, &VBO);
      glGenBuffers(1, &EBO);
      glBindVertexArray(this->vao);
      
      std::vector<float> buffer;

      for (int i = 0; i < this->model.vertices.size(); i+=3)
      {
         buffer.push_back(this->model.vertices[i]);
         buffer.push_back(this->model.vertices[i+1]);
         buffer.push_back(this->model.vertices[i+2]);
         buffer.push_back(this->model.normals[i]);
         buffer.push_back(this->model.normals[i+1]);
         buffer.push_back(this->model.normals[i+2]);
         buffer.push_back(this->model.uv[i]);
         buffer.push_back(this->model.uv[i+1]);
      }

      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(float), &buffer[0], GL_STATIC_DRAW);
      

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->model.indices.size() * sizeof(int), &this->model.indices[0], GL_STATIC_DRAW);
      

      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
      glEnableVertexAttribArray(2);


       
   }
   void MeshRenderer::Update() {
      glUseProgram(this->shader.program);
      glBindVertexArray(this->vao); 

      Matrix4 model = Matrix4(1);
      model = glm::translate(model, this->transform->position);
      model = glm::scale(model, this->transform->scale);
      
      this->shader.SetVec4(Vector4(1,0,0,1), "color");
      
      this->shader.SetMatrix4(camera->view_projection() * model, "projection");
      this->shader.SetMatrix4(model, "model");

      glDrawElements(GL_TRIANGLES, this->model.indices.size(), GL_UNSIGNED_INT, 0);
   }
}