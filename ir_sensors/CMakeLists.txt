cmake_minimum_required(VERSION 2.8.3)
project(ir_sensors)

find_package(catkin REQUIRED COMPONENTS
  ras_arduino_msgs
  roscpp
  rosutil
  std_msgs
)

catkin_package(
#  DEPENDS orocos_kdl
  CATKIN_DEPENDS ras_arduino_msgs roscpp rosutil std_msgs
#  INCLUDE_DIRS
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(ir_sensors src/IRSensors.cpp)
target_link_libraries(ir_sensors ${catkin_LIBRARIES} rosutil)
add_dependencies(ir_sensors ${catkin_EXPORTED_TARGETS})
