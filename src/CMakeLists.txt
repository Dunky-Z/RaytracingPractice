
cmake_minimum_required (VERSION 3.8)

project ("RaytracingPractices")

#PMP
#find_package(PMP)

#Eigen
find_package(Eigen3 REQUIRED)

add_executable(RaytracingPractices camera.h hitable.h hitable_list.h material.h ray.h RaytracingPractice.cpp sphere.h )

target_link_libraries(RaytracingPractices Eigen3::Eigen)
