#include "deal_image.h"

    void DealImage::Invoking(cv::Mat image_copy,std::vector<cv::Vec4i> &lines,std::vector<std::vector<cv::Point>> &contours){
        DealImage::SetImage(image_copy,lines,contours);
    }

    void DealImage::SetImage(cv::Mat image_copy,std::vector<cv::Vec4i> &lines,std::vector<std::vector<cv::Point>> &contours) {
        //change to HSV
        cv::cvtColor(image_copy, image_copy, cv::COLOR_BGR2HSV);

        //only target color becomes white
        cv::inRange(image_copy, cv::Scalar(0, 0, 254), cv::Scalar(40, 1, 255), line_image);
        cv::inRange(image_copy, cv::Scalar(78, 43, 46), cv::Scalar(124, 255, 255), click_image);

        //make line clear
        kernel1 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(4, 4));
        cv::morphologyEx(line_image, line_image, cv::MORPH_CLOSE, kernel1);

        //find edge
        cv::Canny(click_image, click_image, 30, 10);

        //find line status
        HoughLinesP(line_image, lines, 1, CV_PI / 180, 150, 150, 10);

        //find click status
        cv::findContours(click_image, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    }
