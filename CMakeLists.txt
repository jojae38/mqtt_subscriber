cmake_minimum_required(VERSION 3.0.2)
project(mqtt_subscriber)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)
find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
)

set(mosquitto_INCLUDE_DIRS /usr/include)
set(mosquitto_LIBRARIES 
/usr/lib/x86_64-linux-gnu/libmosquitto.so.1
/usr/lib/x86_64-linux-gnu/libmosquitto.so
)

set(jsoncpp_INCLUDE_DIRS /usr/include)
set(jsoncpp_LIBRARIES 
/usr/lib/x86_64-linux-gnu/libjsoncpp.so
/usr/lib/x86_64-linux-gnu/libjsoncpp.so.1
)

catkin_package(
 INCLUDE_DIRS include
 LIBRARIES mqtt_subscriber
 CATKIN_DEPENDS roscpp std_msgs
 DEPENDS system_lib
)
include_directories(
include
  ${catkin_INCLUDE_DIRS}
  ${mosquitto_INCLUDE_DIRS}
  ${jsoncpp_INCLUDE_DIRS}
)

add_executable(mqtt_subscriber src/main.cpp src/mqtt_subscriber.cpp)
target_link_libraries(
  mqtt_subscriber
  ${catkin_LIBRARIES}
  ${mosquitto_LIBRARIES}
  ${jsoncpp_LIBRARIES}
)
add_dependencies(mqtt_subscriber ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
