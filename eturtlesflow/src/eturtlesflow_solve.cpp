#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/transform_listener.h"
#include "tf2_ros/buffer.h"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "geometry_msgs/msg/twist.hpp"

class TurtleTFSubscriber : public rclcpp::Node
{
public:
    TurtleTFSubscriber()
        : Node("sub_tf")
    {
        // 创建 TF 订阅对象
        tf_buffer_ = std::make_shared<tf2_ros::Buffer>(this->get_clock());
        tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);

        // 创建发布器
        pub_ = this->create_publisher<geometry_msgs::msg::Twist>("/turtle2/cmd_vel", 10);

        // 设置循环频率
        timer_ = this->create_wall_timer(std::chrono::milliseconds(100), std::bind(&TurtleTFSubscriber::processTF, this));
    }

private:
    void processTF()
    {
        try
        {
            // 获取 turtle1 相对 turtle2 的坐标信息
            geometry_msgs::msg::TransformStamped tfs = tf_buffer_->lookupTransform("turtle2", "turtle1", tf2::TimePointZero);

            // 根据坐标信息生成速度信息
            geometry_msgs::msg::Twist twist;
            twist.linear.x = 0.5 * std::sqrt(std::pow(tfs.transform.translation.x, 2) + std::pow(tfs.transform.translation.y, 2));
            twist.angular.z = 4 * std::atan2(tfs.transform.translation.y, tfs.transform.translation.x);

            // 发布速度信息
            pub_->publish(twist);
        }
        catch (const std::exception &e)
        {
            RCLCPP_INFO(this->get_logger(), "错误提示: %s", e.what());
        }
    }

    std::shared_ptr<tf2_ros::Buffer> tf_buffer_;
    std::shared_ptr<tf2_ros::TransformListener> tf_listener_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr pub_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char *argv[])
{
    setlocale(LC_ALL,"");
    
    rclcpp::init(argc, argv);
    auto node = std::make_shared<TurtleTFSubscriber>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
