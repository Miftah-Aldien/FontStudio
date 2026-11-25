cmake_minimum_required(VERSION 3.16)
project(FontStudio15 LANGUAGES CXX)

if(NOT WIN32)
  message(FATAL_ERROR "FontStudio15 currently targets Windows only.")
endif()

add_executable(FontStudio15 WIN32
  src/main.cpp
  src/main_window.cpp
  src/main_window.h
)

target_compile_features(FontStudio15 PRIVATE cxx_std_14)
target_compile_definitions(FontStudio15 PRIVATE
  UNICODE
  _UNICODE
  WIN32_LEAN_AND_MEAN
  NOMINMAX
  _WIN32_WINNT=0x0601
  WINVER=0x0601
)

if(MSVC)
  target_compile_options(FontStudio15 PRIVATE /W4 /permissive-)
  set_property(TARGET FontStudio15 PROPERTY
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()
