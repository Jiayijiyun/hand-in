
#ifndef ANSWER_IMAGE_SUBSCRIBER_H
#define ANSWER_IMAGE_SUBSCRIBER_H

#include <utility>

#include "memory"
#include "chrono"
#include "functional"

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "geometry_msgs/msg/point32.hpp"
#include "cv_bridge/cv_bridge.h"
#include "opencv2/opencv.hpp"

#include "deal_image.h"


static rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr subscription_;
static rclcpp::Publisher<geometry_msgs::msg::Point32>::SharedPtr publisher_;

class ImagesSubscriber : public rclcpp::Node {
public:
    ImagesSubscriber(); //: Node("image_subscriber");

private:
    //CallBack parameters
    cv_bridge::CvImagePtr image_all_status;
    cv::Mat test_image;
    DealImage dealImage;
    std::vector<cv::Vec4i> lines;
    std::vector<std::vector<cv::Point>> contours;
    bool judge_click_now;
    geometry_msgs::msg::Point32 to_click;
    cv::Point line_location;

    //InvokingCalculate parameters
    cv::Point click_location;
    int line_locationy;

    //CalculateAverage parameters
    int y;
    bool start;
    int temporary = 0;

    //GetLowCLick parameters
    cv::Point low;
    bool start1;
    cv::Vec2i cal;
    bool start2;

    void CallBack(sensor_msgs::msg::Image::SharedPtr image_raw);

    void InvokingCalculate(std::vector<cv::Vec4i> lines, std::vector<std::vector<cv::Point>> contours,
                           geometry_msgs::msg::Point32 &to_click, bool &judge_click_now, cv::Point &line_location);

    int CalculateAverage(std::vector<cv::Vec4i> lines);

    cv::Point GetLowCLick(std::vector<std::vector<cv::Point>> contours);
};


#endif //ANSWER_IMAGE_SUBSCRIBER_H