@echo off

@set OUT_NAME=glv_cmi.exe
@set OUT_D=..\..\out
@set CC=g++
@set CF=-std=c++2a -g -c
@set CL=-std=c++2a -g
@set SRC_D=..\..\src

if not exist %OUT_D% (mkdir %OUT_D%)

%CC% %CF% %SRC_D%\main.cpp -o %OUT_D%\main.o || exit /b 
%CC% %CL% %OUT_D%\main.o -o %OUT_D%\%OUT_NAME% || exit /b 

