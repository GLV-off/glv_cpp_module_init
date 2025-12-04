#include <iostream>
#include <windows.h>
#include <array>
#include <vector>
#include <string>

static void mkdir(const char* dir);
static void mkdir_win(const char* dir);
static std::string current_dir(const int argc, char** argv);

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

static void mkdir_win(const char* dir) {
  BOOL b = ::CreateDirectory(dir, NULL);
  if(b) {
    std::cout << "Dir created" << std::endl;
  } else {
    std::cout << "Dir not created. LastErrorCode = " << GetLastError<< std::endl;
  }
}
