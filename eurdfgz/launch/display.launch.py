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
    gui_arg = DeclareLaunchArgument(name='gui', default_value='true', choices=['true', 'false'],
                                    description='Flag to enable joint_state_publisher_gui')
    
    # ==========================
    pkg_ros_gz_sim = get_package_share_directory('ros_gz_sim')  # In the /opt/ros/jazzy/...
    gz_sim = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(pkg_ros_gz_sim, 'launch', 'gz_sim.launch.py')),
            launch_arguments={
                'gz_args': '-r world/d.sdf'
            }.items(),
    )

    # ==========================
    jsp = Node(
        package='joint_state_publisher',
        executable='joint_state_publisher',
        condition=IfCondition(LaunchConfiguration('gui'))
    )

    jsp_gui = Node(
        package='joint_state_publisher_gui',
        executable='joint_state_publisher_gui',
        condition=IfCondition(LaunchConfiguration('gui'))
    )

    # ==========================
    sdf_file = os.path.join(eurdfgz, 'urdf', 'my_car.urdf')
    assert os.path.exists(sdf_file)
    with open(sdf_file, 'r') as infp:
        robot_desc = infp.read()
    robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        name='robot_state_publisher',
        output='both',
        parameters=[
            {'use_sim_time': True},
            {'robot_description': robot_desc},
        ]
    )

    # ==========================
    rviz_file = os.path.join(eurdfgz, 'rviz', 'u.rviz')
    assert os.path.join(rviz_file)
    rviz = Node(
        package='rviz2',
        executable='rviz2',
        output='screen',
        arguments=['-d', rviz_file],
        condition=IfCondition(LaunchConfiguration('gui'))
    )

    # ==========================
    # https://gazebosim.org/docs/harmonic/ros2_integration
    # https://github.com/gazebosim/ros_gz/blob/ros2/ros_gz_bridge/README.md
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
            # Sensor Lidar (Gazebo -> ROS2)
            '/lidar@sensor_msgs/msg/LaserScan[gz.msgs.LaserScan',
            '/lidar/points@sensor_msgs/msg/PointCloud2[gz.msgs.PointCloudPacked'
        ],
        remappings=[
            (joint_state_gz_topic, 'joint_states'),
            (link_pose_gz_topic, '/tf'),
            (link_pose_gz_topic + '_static', '/tf_static'),
        ],
        parameters=[{'qos_overrides./tf_static.publisher.durability': 'transient_local'},
                    {'qos_overrides./model/my_car.subscriber.reliability': 'reliable'}],
        output='screen'
    )

    # ==========================
    ld.add_action(gz_sim)
    ld.add_action(gui_arg)
    # ld.add_action(jsp)
    # ld.add_action(jsp_gui)
    ld.add_action(robot_state_publisher)
    ld.add_action(bridge)
    ld.add_action(rviz)

    return ld
