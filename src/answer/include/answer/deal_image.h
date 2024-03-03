#ifndef ANSWER_DEAL_IMAGE_H
#define ANSWER_DEAL_IMAGE_H

#include "memory"
#include "opencv2/opencv.hpp"

class DealImage {
public:
    void Invoking(cv::Mat image_copy, std::vector<cv::Vec4i> &lines, std::vector<std::vector<cv::Point>> &contours);

private:
    void SetImage(cv::Mat image_copy, std::vector<cv::Vec4i> &lines, std::vector<std::vector<cv::Point>> &contours);

    cv::Mat line_image;
    cv::Mat click_image;
    cv::Mat kernel1;
    std::vector<cv::Vec4i> hierarchy;
};

#endif //ANSWER_DEAL_IMAGE_H
