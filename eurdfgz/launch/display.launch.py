import os

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.substitutions import FindPackageShare
from launch_ros.actions import Node
from launch.conditions import IfCondition
from launch.launch_description_sources import PythonLaunchDescriptionSource

from ament_index_python.packages import get_package_share_directory


def generate_launch_description():
    ld = LaunchDescription()
    eurdfgz = get_package_share_directory('eurdfgz')
    
    pkg_ros_gz_sim = get_package_share_directory('ros_gz_sim')  # In the /opt/ros/jazzy/...
    gz_sim = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(pkg_ros_gz_sim, 'launch', 'gz_sim.launch.py')),
            launch_arguments={
                'gz_args': '-r world/d.sdf'
            }.items(),
    )

    gui_arg = DeclareLaunchArgument(name='gui', default_value='true', choices=['true', 'false'],
                                    description='Flag to enable joint_state_publisher_gui')
    
    eurdfgz_path = FindPackageShare('eurdfgz')
    rviz_config_path = PathJoinSubstitution([eurdfgz_path, 'rviz', 'u.rviz'])
    rviz_arg = DeclareLaunchArgument(name='rvizconfig', default_value=rviz_config_path,
                                     description='Absolute path to rviz config file')

    model_path = PathJoinSubstitution(['urdf', 'my_car.urdf'])
    model_arg = DeclareLaunchArgument(name='model', default_value=model_path,
                                        description='Path to robot urdf file relative to urdf_tutorial package')
    
    print("eurdfgz path1: ", eurdfgz)
    print("eurdfgz path2: ", eurdfgz_path)
    rviz = IncludeLaunchDescription(
        PathJoinSubstitution([FindPackageShare('urdf_launch'), 'launch', 'display.launch.py']),
        launch_arguments={
            'urdf_package': 'eurdfgz',
            'urdf_package_path': LaunchConfiguration('model'),
            'rviz_config': LaunchConfiguration('rvizconfig'),
            'jsp_gui': LaunchConfiguration('gui')}.items()
    )

    # rviz = Node(
    #     package='rviz2',
    #     executable='rviz2',
    #     arguments=['-d', os.path.join(eurdfgz, 'rviz', 'urdf.rviz')],
    #     condition=IfCondition(LaunchConfiguration('gui'))
    # )

    # https://gazebosim.org/docs/harmonic/ros2_integration
    gz_topic = '/model/my_car'
    joint_state_gz_topic = '/world/car_demo' + gz_topic + '/joint_state'
    link_pose_gz_topic = gz_topic + '/pose'
    bridge = Node(
        package='ros_gz_bridge',
        executable='parameter_bridge',
        arguments=[
            # Clock (Gazebo -> ROS2)
            '/clock@rosgraph_msgs/msg/Clock[gz.msgs.Clock',
            # Joint states (Gazebo -> ROS2)
            joint_state_gz_topic + '@sensor_msgs/msg/JointState[gz.msgs.Model',
            # Link poses (Gazebo -> ROS2)
            link_pose_gz_topic + '@tf2_msgs/msg/TFMessage[gz.msgs.Pose_V',
            link_pose_gz_topic + '_static@tf2_msgs/msg/TFMessage[gz.msgs.Pose_V',
            # Velocity and odometry (Gazebo -> ROS2)
            gz_topic + '/cmd_vel@geometry_msgs/msg/Twist@gz.msgs.Twist',
            gz_topic + '/odometry@nav_msgs/msg/Odometry@gz.msgs.Odometry',
        ],
        remappings=[
            (joint_state_gz_topic, 'joint_states'),
            (link_pose_gz_topic, '/tf'),
            (link_pose_gz_topic + '_static', '/tf_static'),
        ],
        parameters=[{'qos_overrides./tf_static.publisher.durability': 'transient_local'}],
        output='screen'
    )

    # Bridge
    # bridge = Node(
    #     package='ros_gz_bridge',
    #     executable='parameter_bridge',
    #     arguments=['/cmd_vel@geometry_msgs/msg/Twist@gz.msgs.Twist',
    #                '/model/my_car/odometry@nav_msgs/msg/Odometry@gz.msgs.Odometry',
    #                '/model/my_car/odometry@nav_msgs/msg/Odometry@gz.msgs.Odometry'],
    #     parameters=[{'qos_overrides./model/my_car.subscriber.reliability': 'reliable'}],
    #     output='screen'
    # )

    ld.add_action(gz_sim)
    ld.add_action(gui_arg)
    ld.add_action(rviz_arg)
    ld.add_action(model_arg)
    ld.add_action(bridge)
    ld.add_action(rviz)

    return ld
