
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


rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr subscription_;
rclcpp::Publisher<geometry_msgs::msg::Point32>::SharedPtr publisher_;

class ImagesSubscriber : public rclcpp::Node {
public:
    ImagesSubscriber() : Node("image_subscriber") {
        subscription_ = this->create_subscription<sensor_msgs::msg::Image>(
                "/raw_image", 10, std::bind(&ImagesSubscriber::CallBack, this, std::placeholders::_1));
        publisher_ = create_publisher<geometry_msgs::msg::Point32>("/click_position", 10);

    };

private:
    void CallBack(sensor_msgs::msg::Image::SharedPtr image_raw) {

        //bring pointer to mat
        cv_bridge::CvImagePtr image_all_status;
        image_all_status = cv_bridge::toCvCopy(image_raw, sensor_msgs::image_encodings::BGR8);
        cv::resize(image_all_status->image, image_all_status->image, cv::Size(image_raw->width, image_raw->height));
        cv::Mat test_image = image_all_status->image;
        RCLCPP_INFO_STREAM(this->get_logger(),"Get image");

        //deal with image
        DealImage DealImage;
        std::vector<cv::Vec4i> lines;
        std::vector<std::vector<cv::Point>> contours;
        DealImage.Invoking(test_image, lines, contours);


        //judge whether and where click
        bool judge_click_now = false;
        geometry_msgs::msg::Point32 to_click;
        cv::Point line_location;
        InvokingCalculate(lines, contours, to_click, judge_click_now,line_location);
        if (judge_click_now == 1) {
            judge_click_now = false;
            RCLCPP_FATAL_STREAM(this->get_logger(),
                                "Send position: " << "(" << to_click.x <<","<<to_click.y<< ")"<<"Get line position:"<< line_location.y);
            publisher_->publish(to_click);

        }
    }

    void InvokingCalculate(std::vector<cv::Vec4i> lines, std::vector<std::vector<cv::Point>> contours,
                           geometry_msgs::msg::Point32 &to_click, bool &judge_click_now,cv::Point &line_location) {
        //get the locations of click and line
        cv::Point click_location = GetLowCLick(std::move(contours));
        int line_locationy = CalculateAverage(std::move(lines));
        line_location =cv::Point(0,line_locationy);
        //geometry_msgs::msg::Point32 to_click;
        if (((line_location.y - click_location.y) <= 10)&&((line_location.y - click_location.y) >= -10)) {
            judge_click_now = false;
            to_click.x = click_location.x;
            to_click.y = click_location.y;
        }
    }

    //get the average of line 'y'
    int CalculateAverage(std::vector<cv::Vec4i> lines) {
        int y = 0;
        bool start = true;
        //sum up all 'x' and 'y' in status
        for (const auto &vec: lines) {
            int temporary = 0;
            temporary = (vec[1] + vec[3]) / 2;
            if (start) {
                y = temporary;
                start = 0;
            } else {
                y = (y + temporary) / 2;
            }
        }
        return y;
    }

    //get the lowest click
    cv::Point GetLowCLick(std::vector<std::vector<cv::Point>> contours) {
        cv::Point low;
        bool start1 = 1;
        for (const auto &vec: contours) {
            //get the average of click contours
            cv::Vec2i cal = {0, 0};
            bool start2 = 1;
            for (const auto &p: vec) {
                if (start2) {
                    cal[0] = p.x;
                    cal[1] = p.y;
                    start2 = 0;
                } else {
                    cal[0] = (cal[0] + p.x) / 2;
                    cal[1] = (cal[1] + p.y) / 2;
                }
            }
            //judge the click center whether is the lowest
            if (start1) {
                low.x = cal[0];
                low.y = cal[1];
                start1 = 0;
            } else {
                if (low.y < cal[1]) {
                    low.y = cal[1];
                    low.x = cal[0];
                }
            }
        }
        low.x -=56;
        low.y +=3;
        return low;
    }
};


#endif //ANSWER_IMAGE_SUBSCRIBER_H