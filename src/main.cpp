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

static bool
file_exist(const char* path);

static void 
create_build_script(const char* path);

static bool
is_read_only_dir(const char* path);

int main(int argc, char** argv) {
  /* todo: glv: 08122025
    #5  Сделать проверку на доступность папки в которой 
    мы запустились на запись. Попробовать получить и изменить 
    доступ к чтению\записи у папки\файла */
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
  std::string s_script = sub_path + "\\build.bat";
  create_build_script(s_script.c_str());
  
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

static bool
file_exist(const char* path) 
{
  DWORD dwAttrib = ::GetFileAttributesA(path);

  return (dwAttrib != INVALID_FILE_ATTRIBUTES && 
         !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

static void 
create_build_script(const char* path) {
  if (file_exist(path)) {
    std::cerr << "build script already exist's. Overwrite not implemented or not configured." << std::endl;
    return;
  }
  
  if (is_read_only_dir(path)) {
    std::cerr << "cant write to dir - read only" << std::endl;
    return;
  }

  std::ofstream stream;
  
  stream.open(path, std::ios_base::out);

  if (stream.is_open()) { 
    /* todo: glv: #4 08122025 - Записать UTF-8-BOM заголовок */
    stream << "@echo off" << std::endl
      << std::endl
      << "@set OUT_NAME=app.exe" << std::endl
      << "@set OUT_D=..\\..\\out" << std::endl
      << "@set CC=g++" << std::endl
      << "@set CF=-std=c++2a -g -c" << std::endl
      << "@set CL=-std=c++2a -g" << std::endl
      << "@set SRC_D=..\\..\\src" << std::endl
      << std::endl
      << "if not exist %OUT_D% (mkdir %OUT_D%)" << std::endl;

    /* todo: glv: #3 08122025 Конфигурировать отладочный режим*/
    stream.flush();
    stream.close();
  } else {
    std::cerr << "file not created!: " << path << std::endl;
  }
}

static bool
is_read_only_dir(const char* path) 
{
  DWORD attr = GetFileAttributesA(path); 
  if (attr == INVALID_FILE_ATTRIBUTES) {
      std::cerr << "Directory not exist" << std::endl;
      return false;
  }
  
  // Проверяем права через access control lists (ACL)
  HANDLE hToken;
  if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
      GENERIC_MAPPING mapping = { 0 };
      PRIVILEGE_SET privileges = { 0 };
      DWORD privilegesLength = sizeof(privileges);
      DWORD grantedAccess;
      BOOL result;
      PSECURITY_DESCRIPTOR ACL;
      // Проверяем право на запись
      if (AccessCheck(ACL, hToken, FILE_WRITE_DATA, 
                      &mapping, &privileges, &privilegesLength, 
                      &grantedAccess, &result)) {
          CloseHandle(hToken);
          return result != FALSE;
      }
      CloseHandle(hToken);
  }
  return false;
}
