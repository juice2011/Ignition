#include "utils/files.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <string.h>

#ifdef __linux__
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <shlobj.h>
#include <locale>
#include <codecvt>
#else
#error your out of luck ig (for now)
#endif 

namespace FS {
   std::string ReadTextFile(std::string path) 
   {
      std::ifstream f(path);
      std::stringstream s;
      s << f.rdbuf();
      return s.str();
   }

   std::string ReadFileFromHome(std::string path)
   {
      return ReadTextFile(GetHome() + path);
   }

   void WriteFile(std::string path, std::string data)
   {
      std::ofstream f(path);
      f << data;
   }

   void WriteFileFromHome(std::string path, std::string data)
   {
      WriteFile(GetHome() + path, data);
   }
   
   struct {
      std::ifstream file;
      int i;
      int size;
      std::string data;
   } read;

   struct {
      std::string data;
      std::string path;
   } write;

   void BeginBinaryRead(std::string path)
   {
      read.file = std::ifstream(path);
      std::stringstream s;
      s << read.file.rdbuf();
      read.data = s.str();
      read.size = read.data.size();
      read.i = -1;
   }
   void BeginBinaryWrite(std::string path)
   {
      write.data = "";
      write.path = path;
   }

   uint8_t Read8()
   {
      return read.data[++read.i];
   }
   uint16_t Read16()
   {
      uint16_t a, b;
      a = read.data[++read.i];
      b = read.data[++read.i];
      return (a << 8) + b;
   }
   uint32_t Read32()
   {
      uint32_t a, b, c, d;
      if (read.i + 4 > read.size)
      {
         std::cerr << "reading too far\n";
         return 0;
      }
      a = read.data[++read.i];
      b = read.data[++read.i];
      c = read.data[++read.i];
      d = read.data[++read.i];
      return (a << 24) + (b << 16) + (c << 8) + d;
   }

   float ReadFloat()
   {
      char a, b, c, d;
      float o;
      if (read.i + 4 > read.size)
      {
         std::cerr << "reading too far\n";
         return 0;
      }
      a = read.data[++read.i];
      b = read.data[++read.i];
      c = read.data[++read.i];
      d = read.data[++read.i];
      char e[4] = {a, b, c, d}; 
      memcpy(&o, &e, sizeof(o));
      return o;
   }

   bool CanRead()
   {
      return read.size > 0;
   }

   void Write8(uint8_t data)
   {
      write.data.push_back(data);
   }

   void Write16(uint16_t data)
   {
      write.data.push_back(data >> 8);
      write.data.push_back(data);
   }

   void Write32(uint32_t data)
   {
      write.data.push_back(data >> 24);
      write.data.push_back(data >> 16);
      write.data.push_back(data >> 8);
      write.data.push_back(data);
   }

   void WriteFloat(float data)
   {  
      char b[4];  
      memcpy(&b, &data, sizeof(b));
      write.data.push_back(b[0]);
      write.data.push_back(b[1]);
      write.data.push_back(b[2]);
      write.data.push_back(b[3]);
   }

   void EndBinaryRead()
   {
      read.file.close();
   }
   void EndBinaryWrite()
   {
      std::ofstream o(write.path);
      o << write.data;

      o.close();
      write.data = "";
      write.path = "";
   }


#ifdef __linux__
   std::string GetHome()
   {
      const char* home;
      if ((home = getenv("XDG_CONFIG_HOME")) == NULL)
         if ((home = getenv("HOME")) == NULL)
            home = getpwuid(getuid())->pw_dir;
      return home;
   }
#elif defined(_WIN32) || defined(_WIN64)
   std::string GetHome()
   {
      std::wstringstream ss;
      HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
      if (FAILED(hr)) 
      {
         std::cerr << "Failed to initialize COM library." << std::endl;
      }
      
      PWSTR path = NULL;
      hr = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &path);
      if (SUCCEEDED(hr)) 
      {
         std::filesystem::path appDataPath(path);
      
         ss << appDataPath;

         CoTaskMemFree(path);
      }
      else
      {
         std::cerr << "Failed to get AppData path." << std::endl;
      }
      
      CoUninitialize();
      return std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(ss.str());
   }
#endif 
}
