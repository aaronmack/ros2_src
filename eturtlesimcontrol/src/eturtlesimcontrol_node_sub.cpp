#include <cstdio>
#include "rclcpp/rclcpp.hpp"
#include "turtlesim/msg/pose.hpp"
// #include "geometry_msgs/msg/Twist.h"

void doPose(const turtlesim::msg::Pose::ConstPtr& p){
    RCLCPP_INFO(rclcpp::get_logger("my_logger"), "乌龟位姿信息:x=%.2f,y=%.2f,theta=%.2f,lv=%.2f,av=%.2f",
        p->x,p->y,p->theta,p->linear_velocity,p->angular_velocity
    );
}

int main(int argc, char *argv[])
{
    setlocale(LC_ALL,"");
    rclcpp::init(argc,argv);
    auto node = rclcpp::Node::make_shared("sub_pose");
    auto sub = node->create_subscription<turtlesim::msg::Pose>("/turtle1/pose", 1000, doPose);
    rclcpp::spin(node);
    return 0;
}