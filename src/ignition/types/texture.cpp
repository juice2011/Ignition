#include "types/texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "utils/stb_image.h"

#include <iostream>
int i = 0;
namespace Ignition::Rendering {
   void Texture::LoadData(std::string file)
   {
      /*
      if (flags & TextureFlags::Repeat) {
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      }
      if (flags & TextureFlags::Clamp) {
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      }
      if (flags & TextureFlags::Mirrored_Repeat) {
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
      }
      
      if (flags & TextureFlags::Linear) {
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      }
      if (flags & TextureFlags::Nearest) {
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      }
      */
      glGenTextures(1, &this->location);
      glBindTexture(GL_TEXTURE_2D, this->location);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      int w, h, nr;
      stbi_set_flip_vertically_on_load(true);
      unsigned char* d = stbi_load(file.data(), &w, &h, &nr, 0);
      if (d)
      {
         GLenum format;
         switch (nr) {
            case(1):
               format = GL_RED;
               break;
            case(2):
               format = GL_RG;
               break;
            case(3):
               format = GL_RGB;
               break;
            case(4):
               format = GL_RGBA;
               break;
            default:
               format = GL_RGB;
               break;
         }
         glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, d);
         glGenerateMipmap(GL_TEXTURE_2D);
      }
      else
      {
         std::cerr << "texture can't be loaded\n";
      }  
       
      std::cout << this->location << " " << w << " " << h << "\n\n";
      i++;
      this->id = i-1;
      std::cout << this->id << "\n\n";
   }
}
