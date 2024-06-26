#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"

int main(int argc, char *argv[])
{
    rclcpp::init (argc, argv);
    auto node = rclcpp::Node::make_shared("my_control");
    // auto pub = nh.advertise<geometry_msgs::Twist>("/turtlel/cmd_vel", 10);
    auto pub = node->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);

    rclcpp::Rate rate(10);
    geometry_msgs::msg::Twist twist;
    twist.linear.x = 1.0;
    twist.linear.y = 0.0;
    twist.linear.z = 0.0;
    twist.angular.x = 0.0;
    twist.angular.y = 0.0;
    twist.angular.z = 0.5;

    while (rclcpp::ok())
    {
        pub->publish(twist);
        rate.sleep();
        rclcpp::spin_some(node);
    }
    rclcpp::shutdown();
    return 0;
}
