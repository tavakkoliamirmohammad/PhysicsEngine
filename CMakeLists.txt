cmake_minimum_required(VERSION 3.16)
project(PhysicalEngine)

set(CMAKE_CXX_STANDARD 14)

add_executable(PhysicalEngine main.cpp Box2D.cpp Box2D.h Game.cpp Game.h Scene.cpp Scene.h CollisionDetector.cpp CollisionDetector.h CollisionResolver.cpp CollisionResolver.h PhysicalEngine.cpp PhysicalEngine.h Circle.cpp Circle.h Shape.cpp Shape.h)

target_link_libraries(PhysicalEngine -lglut -lGL -lGLU -lGLEW -lglfw)