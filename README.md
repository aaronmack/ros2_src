# ROS2

## Create A New Package

1. C++

`ros2 pkg create --build-type ament_cmake --node-name <node_name> <package_name> --dependencies std_msgs`

https://answers.ros.org/question/289018/how-to-create-a-ros2-new-package/

2. Python

`ros2 pkg create --build-type ament_python --node-name <node_name> <package_name>`

https://docs.ros.org/en/foxy/Tutorials/Beginner-Client-Libraries/Creating-Your-First-ROS2-Package.html

## Build A Package

`colcon build`

https://docs.ros.org/en/jazzy/How-To-Guides/Migrating-from-ROS1/Migrating-CPP-Packages.html

https://docs.ros.org/en/crystal/Tutorials/Creating-Your-First-ROS2-Package.html#build-a-package


## Run A Package

1. C++

`ros2 run eros2cpp_package eros2cpp_node`

2. Python

`ros2 run eros2py_package eros2py_node`

## Run Launch File

`ros2 launch eros2cpp_package start_turtlesim.xml`

https://docs.ros.org/en/jazzy/Tutorials/Intermediate/Launch/Launch-system.html

## Examples

1. turtlesim


`ros2 run turtlesim turtle_teleop_key`
`ros2 run turtlesim turtlesim_node`

2. turtle_tf2

`ros2 launch turtle_tf2_py turtle_tf2_demo.launch.py`
`ros2 run tf2_ros tf2_echo turtle2 turtle1`
`ros2 run tf2_tools view_frames`
`ros2 run rviz2 rviz2 -d $(ros2 pkg prefix --share turtle_tf2_py)/rviz/turtle_rviz.rviz`

## Commands of ROS2


`ros2 topic list`
`ros2 topic echo <topic_name>`
`ros2 param list`
`ros2 daemon status`
`ros2 interface show turtlesim/msg/Pose`
`ros2 service list`
`ros2 service type /spawn`
`ros2 param set turtlesim background_b 255`
`ros2 bag record -a -o a.bag`
`ros2 bag info a.bag`
`ros2 bag play a.bag`

`rqt_graph`
`rqt`
`rviz2`

`gz sim`
`gz sim empty.sdf`
`gz service -l`
`gz service -is /world/empty/create`
`gz service -s /world/empty/create --reqtype gz.msgs.EntityFactory --reptype gz.msgs.Boolean --timeout 1000 --req 'sdf_filename: "urdf/01.urdf", name: "urdf_model"'`
`gz topic -t "/cmd_vel" -m gz.msgs.Twist -p "linear: {x: 0.2}, angular: {z: 0.5}"`
`gz topic -l`
`gz sdf -p urdf/my_car.urdf > urdf/my_car.sdf`

`check_urdf <filename>.urdf`
`urdf_to_graphviz <filename>.urdf`

`ros2 run rqt_console rqt_console`
`ros2 topic pub /turtle1/cmd_vel geometry_msgs/msg/Twist "{linear: {x: 1.0, y: 0.0, z: 0.0}, angular: {x: 0.0, y: 0.0, z: 0.5}}"`