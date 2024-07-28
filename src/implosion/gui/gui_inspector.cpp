#include "gui/gui.hpp"
#include "imgui_stdlib.h"

#include <glm/gtc/type_ptr.hpp>

#include <cstring>
#include <iostream>

namespace Implosion {
   void GUI::Inspector(Ignition::Object* obj)
   {
      if (ImGui::Begin("Inspector")) {
         if (obj == nullptr)
         {
            ImGui::Text("Select Something to Use This");
         } else {
            ImGui::Checkbox("##Enabled", &obj->enabled);
            
            ImGui::SameLine();
            
            ImGui::InputText("Name", &obj->name);
            ImGui::InputText("Tag", &obj->tag);
            
            ImGui::Separator();

            if (ImGui::BeginChild("Transform"))
            {
               Ignition::Vector3 pos = obj->transform.position;
               Ignition::Quat rot = obj->transform.rotation;
               Ignition::Vector3 sca = obj->transform.scale;
               if (ImGui::InputFloat3("Position", glm::value_ptr(pos)))
               {
                  obj->transform.position = pos;
               }
               if (ImGui::InputFloat4("Rotation", glm::value_ptr(rot)))
               {
                  obj->transform.rotation = rot;
               }
               if (ImGui::InputFloat3("Scale", glm::value_ptr(sca)))
               {
                  obj->transform.scale = sca;
               }
               
               if (ImGui::CollapsingHeader("Vectors"))
               {
                  ImGui::Text("Forward : %f, %f, %f", obj->transform.forward.x, obj->transform.forward.y, obj->transform.forward.z);
                  ImGui::Text("Right : %f, %f, %f", obj->transform.right.x, obj->transform.right.y, obj->transform.right.z);
                  ImGui::Text("Up : %f, %f, %f", obj->transform.up.x, obj->transform.up.y, obj->transform.up.z);
               }

               ImGui::EndChild();
            }
            ImGui::Separator();
         }
      }

      ImGui::End();
   }
}
