//#include "ros/ros.h"
#include "rclcpp/rclcpp.hpp"

//#include "std_msgs/String.h"
#include "std_msgs/msg/string.hpp"

int main(int argc, char** argv)
{
    // ros::init(argc, argv, "eros2_cpp");
    // ros::NodeHandle nh;
    // ROS_INFO("hello world!");

    int count = 0;
    std_msgs::msg::String msg;
    std::stringstream ss;
    ss << "hello world " << count++;
    msg.data = ss.str();

    rclcpp::init(argc, argv);
    auto node = rclcpp::Node::make_shared("eros2_cpp");

    std::cout<<"hello ros2!"<<std::endl;

    RCLCPP_INFO(node->get_logger(), "%s\n", msg.data.c_str());

    return 0;
}
