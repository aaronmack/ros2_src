
#include "rclcpp/rclcpp.hpp"
#include "turtlesim/srv/spawn.hpp"

int main(int argc, char *argv[])
{
    setlocale(LC_ALL,"");

    rclcpp::init(argc,argv);
    auto node = rclcpp::Node::make_shared("set_turtle");

    //auto client = nh.serviceClient<turtlesim::srv::Spawn>("/spawn");
    auto client = node->create_client<turtlesim::srv::Spawn>("/spawn");
    
    // client.waitForExistence();
    //ros::service::waitForService("/spawn");

    if (!client->wait_for_service(std::chrono::seconds(5))) {
        RCLCPP_ERROR(node->get_logger(), "服务未启动");
        return 1;
    }

    //turtlesim::Spawn spawn;
    //spawn.request.x = 1.0;
    //spawn.request.y = 1.0;
    //spawn.request.theta = 1.57;
    //spawn.request.name = "my_turtle";

    auto request = std::make_shared<turtlesim::srv::Spawn::Request>();
    request->x = 1.0;
    request->y = 1.0;
    request->theta = 1.57;
    request->name = "my_turtle";

    //bool flag = client.call(spawn);
    auto result = client->async_send_request(request);

    if (rclcpp::spin_until_future_complete(node, result) == rclcpp::FutureReturnCode::SUCCESS) {
        RCLCPP_INFO(node->get_logger(), "新的乌龟生成,名字: %s", result.get()->name.c_str());
    } else {
        RCLCPP_ERROR(node->get_logger(), "乌龟生成失败！！！");
    }

    rclcpp::shutdown();

    return 0;
}