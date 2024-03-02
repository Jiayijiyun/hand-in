#ifndef ANSWER_DEAL_IMAGE_H
#define ANSWER_DEAL_IMAGE_H

#include "chrono"
#include "functional"
#include "memory"
#include "string"

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

#include "geometry_msgs/msg/point32.hpp"

#include "cv_bridge/cv_bridge.h"
#include "opencv2/opencv.hpp"

class DealImage {
public:
    void Invoking(cv::Mat image_copy,std::vector<cv::Vec4i> &lines,std::vector<std::vector<cv::Point>> &contours){
        DealImage::SetImage(image_copy,lines,contours);
    }

private:
   void SetImage(cv::Mat image_copy,std::vector<cv::Vec4i> &lines,std::vector<std::vector<cv::Point>> &contours){
        //change to HSV
       cv::Mat line_image;
       cv::Mat click_image;
        cv::cvtColor(image_copy,image_copy,cv::COLOR_BGR2HSV);

        //only target color becomes white
        cv::inRange(image_copy,cv::Scalar(0,0,254),cv::Scalar(40,1,255),line_image);
        cv::inRange(image_copy,cv::Scalar(78, 43, 46), cv::Scalar(124, 255, 255),click_image);

        //make line clear
       cv::Mat kernel1 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(4, 4));
       cv::morphologyEx(line_image, line_image, cv::MORPH_CLOSE, kernel1);

       //find edge
       cv::Canny(click_image, click_image, 30, 10);
       //cv::Canny(line_image, line_image, 30, 10);

       //find line status
       HoughLinesP(line_image, lines, 1, CV_PI/180, 150, 150, 10 );

       //find click status
       std::vector<cv::Vec4i> hierarchy;
       cv::findContours(click_image, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    }
};


#endif //ANSWER_DEAL_IMAGE_H
