#include "../include/answer/image_subscriber.h"

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ImagesSubscriber>());
    rclcpp::shutdown();
    return 0;
}


