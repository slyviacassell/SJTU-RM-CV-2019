//
// Created by xinyang on 19-3-27.
//
#include <log.h>
#include <armor_finder/armor_finder.h>

ArmorFinder::ArmorFinder(EnemyColor color, Uart &u, string paras_folder) :
                            uart(u),
                            enemy_color(color),
                            state(STANDBY_STATE),
                            classifier(std::move(paras_folder)),
                            contour_area(0)
                            {
    auto para = TrackerToUse::Params();
    para.desc_npca = 1;
    para.desc_pca = 0;
    tracker = TrackerToUse::create(para);
    if(!tracker){
        LOGW("Tracker Not init");
    }
}

void ArmorFinder::run(cv::Mat &src) {
    cv::Mat src_use;
//    if (src.type() == CV_8UC3) {
//        cv::cvtColor(src, src_use, CV_RGB2GRAY);
//    }else{
        src_use = src.clone();
//    }
    cv::cvtColor(src_use, src_gray, CV_RGB2GRAY);

    stateSearchingTarget(src_use);
    return;

    switch (state){
        case SEARCHING_STATE:
            if(stateSearchingTarget(src_use)){
                if((armor_box & cv::Rect2d(0, 0, 640, 480)) == armor_box) {
                    cv::Mat roi = src_use.clone()(armor_box);
                    cv::threshold(roi, roi, 200, 255, cv::THRESH_BINARY);
                    contour_area = cv::countNonZero(roi);
                    auto para = TrackerToUse::Params();
                    para.desc_npca = 1;
                    para.desc_pca = 0;
                    tracker = TrackerToUse::create(para);
                    tracker->init(src_use, armor_box);
                    state = TRACKING_STATE;
                    LOGW("into track");
                }
            }
            break;
        case TRACKING_STATE:
            if(!stateTrackingTarget(src_gray)){
                state = SEARCHING_STATE;
                //std::cout << "into search!" << std::endl;
            }
            break;
        case STANDBY_STATE:
        default:
            stateStandBy();
    }
}

#define FOCUS_PIXAL      (0.36/0.48*640)

bool ArmorFinder::sendBoxPosition() {
    auto rect = armor_box;
    double dx = rect.x + rect.width/2 - 320;
    double dy = rect.y + rect.height/2 - 240;
    double yaw   = atan(dx / FOCUS_PIXAL) * 180 / 3.14159265459;
    double pitch = atan(dy / FOCUS_PIXAL) * 180 / 3.14159265459;
    uart.sendTarget(yaw, pitch, 0);
    return true;
}