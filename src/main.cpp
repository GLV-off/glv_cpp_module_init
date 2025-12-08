#include <iostream>
#include <windows.h>
#include <array>
#include <vector>
#include <string>
#include <fstream>

static void 
mkdir(const char* dir);

static void 
mkdir_win(const char* dir);

static std::string 
current_dir(const int argc, char** argv);

static void 
create_build_script(const char* path);

int main(int argc, char** argv) {
  std::cout << argv[0] << std::endl;

  std::string root_path = current_dir(argc, argv);
  std::cout << root_path << std::endl;
  std::vector<std::string> paths;
  std::string sub_path = root_path + "\\out";
  paths.push_back(sub_path);
  sub_path = root_path + "\\pkg";
  paths.push_back(sub_path);
  sub_path = root_path + "\\pkg\\raw";
  /* todo: glv: 08122025 - 
    #1 Определить 
    что мы хотим сгенерировать билд скрипт 
    библиотеки или  
    #2 Если файл уже существует то не слепо 
    заменять его */
  create_build_script((sub_path + "\\build.bat").c_str());
  paths.push_back(sub_path);
  sub_path = root_path + "\\src";
  paths.push_back(sub_path);
  sub_path = root_path + "\\doc";
  paths.push_back(sub_path);

  for(auto it = paths.begin(); it != paths.end(); ++it) {
    mkdir((*it).c_str());   
  }
  return 0;
}

static std::string current_dir(const int argc, char** argv) {
  DWORD size = GetCurrentDirectory(0, NULL);
  char* bfr = new char[size];
  DWORD res = GetCurrentDirectory(size, bfr);
  std::string s = std::string(bfr);
  delete[] bfr;
  return s;
}

static void mkdir(const char* dir) {
  mkdir_win(dir);
}

static void 
mkdir_win(const char* dir) {
  BOOL b = ::CreateDirectory(dir, NULL);
  if(b) {
    std::cout << "Dir created" << std::endl;
  } else {
    std::cout << "Dir not created. LastErrorCode = " << GetLastError<< std::endl;
  }
}

static void 
create_build_script(const char* path) {
  std::ofstream stream(path);
  stream << "@echo off" << std::endl
    << std::endl
    << "@set OUT_NAME=app.exe" << std::endl
    << "@set OUT_D=..\\..\\out" << std::endl
    << "@set CC=g++" << std::endl
    << "@set CF=-std=c++2a -g -c";

// @set OUT_NAME=glv_cmi.exe
// @set OUT_D=..\..\out
// @set CC=g++
// @set CF=-std=c++2a -g -c
// @set CL=-std=c++2a -g
// @set SRC_D=..\..\src

// if not exist %OUT_D% (mkdir %OUT_D%)

// %CC% %CF% %SRC_D%\main.cpp -o %OUT_D%\main.o || exit /b
// %CC% %CL% %OUT_D%\main.o -o %OUT_D%\%OUT_NAME% || exit /b 




  stream.close();
}