cmake_minimum_required(VERSION 4.0)
project(musicli C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS_DEBUG 
  "${CMAKE_C_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer"
)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LUAJIT REQUIRED IMPORTED_TARGET luajit)
pkg_check_modules(NOTCURSES REQUIRED IMPORTED_TARGET notcurses)
add_executable(musicli
  src/config.c
  src/config.h
  src/global_var.h
  src/utils.c
  src/ui.c
  src/ui.h
  src/main.c
)
target_include_directories(musicli PRIVATE include)
target_link_libraries(musicli PRIVATE
  PkgConfig::LUAJIT
  PkgConfig::NOTCURSES
)
