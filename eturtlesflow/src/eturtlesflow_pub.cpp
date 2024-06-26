#include "rclcpp/rclcpp.hpp"
#include "turtlesim/msg/pose.hpp"
#include "tf2_ros/transform_broadcaster.h"
#include "tf2/LinearMath/Quaternion.h"
#include "geometry_msgs/msg/transform_stamped.hpp"

class TurtlePoseBroadcaster : public rclcpp::Node
{
public:
    TurtlePoseBroadcaster(const std::string &turtle_name)
        : Node("pub_tf"), turtle_name_(turtle_name)
    {
        // 创建订阅对象
        subscription_ = this->create_subscription<turtlesim::msg::Pose>(
            turtle_name_ + "/pose", 10,
            std::bind(&TurtlePoseBroadcaster::doPose, this, std::placeholders::_1));

        // 创建 TF 广播器
        broadcaster_ = std::make_shared<tf2_ros::TransformBroadcaster>(this);
    }

private:
    void doPose(const turtlesim::msg::Pose::SharedPtr pose)
    {
        // 将 pose 信息转换成 TransformStamped
        geometry_msgs::msg::TransformStamped tfs;
        tfs.header.frame_id = "world";
        tfs.header.stamp = this->now();
        tfs.child_frame_id = turtle_name_;
        tfs.transform.translation.x = pose->x;
        tfs.transform.translation.y = pose->y;
        tfs.transform.translation.z = 0.0;
        tf2::Quaternion qtn;
        qtn.setRPY(0, 0, pose->theta);
        tfs.transform.rotation.x = qtn.getX();
        tfs.transform.rotation.y = qtn.getY();
        tfs.transform.rotation.z = qtn.getZ();
        tfs.transform.rotation.w = qtn.getW();

        // 发布
        broadcaster_->sendTransform(tfs);
    }

    std::string turtle_name_;
    rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr subscription_;
    std::shared_ptr<tf2_ros::TransformBroadcaster> broadcaster_;
};

int main(int argc, char *argv[])
{
    setlocale(LC_ALL,"");

    rclcpp::init(argc, argv);
    std::string turtle_name;

    if (argc < 2)
    {
        RCLCPP_ERROR(rclcpp::get_logger("pub_tf"), "请传入正确的参数 --turtle_name <turtle_name>");
        return 1;
    }
    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg == "--turtle_name")
        {
            if (i + 1 < argc)
            {
                turtle_name = argv[i + 1];
                RCLCPP_INFO(rclcpp::get_logger("pub_tf"), "乌龟 %s 坐标发送启动",turtle_name.c_str());
            }
            else
            {
                std::cerr << "Error: Missing value for --turtle_name" << std::endl;
                return 1;
            }
        }
    }

    auto node = std::make_shared<TurtlePoseBroadcaster>(turtle_name.c_str());
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
