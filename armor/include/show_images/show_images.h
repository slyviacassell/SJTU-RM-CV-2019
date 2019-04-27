//
// Created by xinyang on 19-3-27.
//

#ifndef _SHOW_IMAGES_H_
#define _SHOW_IMAGES_H_

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <armor_finder/armor_finder.h>

void showArmorBoxVector(std::string windows_name, const cv::Mat &src, const std::vector<cv::Rect2d> &armor_box);
void showArmorBox(std::string windows_name, const cv::Mat &src, cv::Rect2d armor_box);
void showContours(std::string windows_name, const cv::Mat &src, const std::vector<LightBlob> &light_blobs);
void showArmorBoxClass(std::string window_names, const cv::Mat &src, vector<cv::Rect2d> boxes_one,
                       vector<cv::Rect2d> boxes_two, vector<cv::Rect2d> boxes_three);


#endif /* _SHOW_IMAGES_H_ */
