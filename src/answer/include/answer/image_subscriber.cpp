//
// Created by jianyu on 24-2-24.
//

#include "image_subscriber.h"

ImagesSubscriber::ImagesSubscriber() : Node("image_subscriber") {
    subscription_ = this->create_subscription<sensor_msgs::msg::Image>(
            "/raw_image", 10, std::bind(&ImagesSubscriber::CallBack, this, std::placeholders::_1));
    publisher_ = create_publisher<geometry_msgs::msg::Point32>("/click_position", 10);
};

void ImagesSubscriber::CallBack(sensor_msgs::msg::Image::SharedPtr image_raw) {

    //bring pointer to mat
    image_all_status = cv_bridge::toCvCopy(image_raw, sensor_msgs::image_encodings::BGR8);
    cv::resize(image_all_status->image, image_all_status->image, cv::Size(image_raw->width, image_raw->height));
    cv::Mat test_image = image_all_status->image;
    //RCLCPP_INFO_STREAM(this->get_logger(),"Get image");

    //deal with image
    dealImage.Invoking(test_image, lines, contours);


    //judge whether and where click
    judge_click_now = false;
    InvokingCalculate(lines, contours, to_click, judge_click_now, line_location);
    if (judge_click_now == 1) {
        //judge_click_now = false;
        RCLCPP_FATAL_STREAM(this->get_logger(),
                            "Send position: " << "(" << to_click.x << "," << to_click.y << ")" << line_location.y);
        publisher_->publish(to_click);
    }
}

void ImagesSubscriber::InvokingCalculate(std::vector<cv::Vec4i> lines, std::vector<std::vector<cv::Point>> contours,
                                         geometry_msgs::msg::Point32 &to_click, bool &judge_click_now,
                                         cv::Point &line_location) {
    //get the locations of click and line
    click_location = GetLowCLick(std::move(contours));
    line_locationy = CalculateAverage(std::move(lines));
    line_location = cv::Point(0, line_locationy);
    //geometry_msgs::msg::Point32 to_click;
    if (((line_location.y - click_location.y) <= 26) && ((line_location.y - click_location.y) >= 0)) {
        judge_click_now = true;
        to_click.x = click_location.x;
        to_click.y = click_location.y;
    }
}

//get the average of line 'y'
int ImagesSubscriber::CalculateAverage(std::vector<cv::Vec4i> lines) {
    y = 0;
    start = true;
    //sum up all 'x' and 'y' in status
    for (const auto &vec: lines) {
        temporary = 0;
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
cv::Point ImagesSubscriber::GetLowCLick(std::vector<std::vector<cv::Point>> contours) {
    start1 = 1;
    for (const auto &vec: contours) {
        //get the average of click contours
        cal = {0, 0};
        start2 = 1;
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
    low.x -= 56;
    //low.y +=3;
    return low;
}


