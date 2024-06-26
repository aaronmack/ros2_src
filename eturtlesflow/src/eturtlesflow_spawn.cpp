#include "rclcpp/rclcpp.hpp"
#include "turtlesim/srv/spawn.hpp"

int main(int argc, char *argv[])
{
    setlocale(LC_ALL,"");

    rclcpp::init(argc, argv);
    auto node = rclcpp::Node::make_shared("create_turtle");

    auto client = node->create_client<turtlesim::srv::Spawn>("/spawn");
    if (!client->wait_for_service(std::chrono::seconds(5))) {
        RCLCPP_ERROR(node->get_logger(), "Service not available");
        return 1;
    }

    auto request = std::make_shared<turtlesim::srv::Spawn::Request>();
    request->name = "turtle2";
    request->x = 1.0;
    request->y = 2.0;
    request->theta = 3.12415926;

    auto result = client->async_send_request(request);

    if (rclcpp::spin_until_future_complete(node, result) == rclcpp::FutureReturnCode::SUCCESS) {
        RCLCPP_INFO(node->get_logger(), "新的乌龟生成,名字: %s", result.get()->name.c_str());
    } else {
        RCLCPP_ERROR(node->get_logger(), "乌龟生成失败！！！");
    }

    rclcpp::shutdown();
    return 0;
}
