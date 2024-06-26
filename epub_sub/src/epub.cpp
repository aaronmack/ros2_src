//#include "ros/ros.h"
#include "rclcpp/rclcpp.hpp"

//#include "std_msgs/String.h"
#include "std_msgs/msg/string.hpp"

#include <sstream>

int main(int argc, char  *argv[])
{   
    setlocale(LC_ALL,"");

    // ros::init(argc, argv, "talker");
    // ros::NodeHandle nh;
    rclcpp::init(argc,argv);
    auto node = rclcpp::Node::make_shared("talker");

    //rclcpp::Publisher pub = nh.advertise<std_msgs::String>("chatter",10);
    auto pub = node->create_publisher<std_msgs::msg::String>("chatter",10);


    std_msgs::msg::String msg;
    // msg.data = "你好啊！！！";
    std::string msg_front = "Hello 你好！";
    int count = 0;

    // ros::Rate r(1);
    rclcpp::Rate loop_rate(1);

  	// while (ros::ok())
  	while (rclcpp::ok())
    {
        //使用 stringstream 拼接字符串与编号
        std::stringstream ss;
        ss << msg_front << count;
        msg.data = ss.str();

        // pub.publish(msg);
		    pub->publish(msg);

		    // ROS_INFO("%s", msg.data.c_str());
    	  RCLCPP_INFO(node->get_logger(), "%s\n", msg.data.c_str());

        loop_rate.sleep();

        count++;//循环结束前，让 count 自增

        //暂无应用
        // ros::spinOnce();
        rclcpp::spin_some(node);
    }


    return 0;
}