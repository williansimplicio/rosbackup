cmake_minimum_required(VERSION 2.8.3)
project(imu_subs)

find_package(catkin REQUIRED COMPONENTS
  sensor_msgs
  roscpp
  rospy
)

catkin_package()

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(imu_listener src/imu_listener.cpp)
add_dependencies(imu_listener sensor_msgs_generate_messages_cpp)
 target_link_libraries(imu_listener
   ${catkin_LIBRARIES}
 )

add_executable(teste src/teste.cpp)
add_dependencies(teste sensor_msgs_generate_messages_cpp)
 target_link_libraries(teste
   ${catkin_LIBRARIES}
 )

add_executable(imu_raw src/imu_raw.cpp)
add_dependencies(imu_raw sensor_msgs_generate_messages_cpp)
 target_link_libraries(imu_raw
   ${catkin_LIBRARIES}
 )
