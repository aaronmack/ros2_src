cmake_minimum_required(VERSION 3.8)
project(eturtlesflow)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(std_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclpy REQUIRED)
find_package(turtlesim REQUIRED)
find_package(geometry_msgs REQUIRED)

add_executable(eturtlesflow_spawn src/eturtlesflow_spawn.cpp)
add_executable(eturtlesflow_pub src/eturtlesflow_pub.cpp)
add_executable(eturtlesflow_solve src/eturtlesflow_solve.cpp)

target_include_directories(eturtlesflow_spawn PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>)
  target_include_directories(eturtlesflow_pub PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>)
  target_include_directories(eturtlesflow_solve PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>)
target_compile_features(eturtlesflow_spawn PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17
target_compile_features(eturtlesflow_pub PUBLIC c_std_99 cxx_std_17)
target_compile_features(eturtlesflow_solve PUBLIC c_std_99 cxx_std_17)

ament_target_dependencies(
  eturtlesflow_spawn
  "std_msgs"
  "tf2"
  "tf2_ros"
  "tf2_geometry_msgs"
  "rclcpp"
  "rclpy"
  "turtlesim"
  "geometry_msgs"
)
ament_target_dependencies(
  eturtlesflow_pub
  "std_msgs"
  "tf2"
  "tf2_ros"
  "tf2_geometry_msgs"
  "rclcpp"
  "rclpy"
  "turtlesim"
  "geometry_msgs"
)
ament_target_dependencies(
  eturtlesflow_solve
  "std_msgs"
  "tf2"
  "tf2_ros"
  "tf2_geometry_msgs"
  "rclcpp"
  "rclpy"
  "turtlesim"
  "geometry_msgs"
)
install(TARGETS eturtlesflow_spawn
  DESTINATION lib/${PROJECT_NAME})
  install(TARGETS eturtlesflow_pub
  DESTINATION lib/${PROJECT_NAME})
  install(TARGETS eturtlesflow_solve
  DESTINATION lib/${PROJECT_NAME})

install(DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME}/
)
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
