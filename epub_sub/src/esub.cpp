#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using std::placeholders::_1;

void doMsg(const std_msgs::msg::String::ConstPtr& msg_p){
    RCLCPP_INFO(rclcpp::get_logger("my_logger"), "我听见: %s",msg_p->data.c_str());
    // ROS_INFO("我听见:%s",(*msg_p).data.c_str());
}
int main(int argc, char  *argv[])
{
    setlocale(LC_ALL,"");
    rclcpp::init(argc,argv);
    auto node = rclcpp::Node::make_shared("listener");

    //ros::Subscriber sub = nh.subscribe<std_msgs::String>("chatter",10,doMsg);
    auto sub = node->create_subscription<std_msgs::msg::String>("/chatter", 10, doMsg);  

    rclcpp::spin(node);
    return 0;
}