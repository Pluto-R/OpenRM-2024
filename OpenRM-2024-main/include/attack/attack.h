#ifndef __OPENRM_ATTACK_ATTACK_H__
#define __OPENRM_ATTACK_ATTACK_H__
#include <utils/timer.h>
#include <structure/stamp.hpp>
#include <structure/enums.hpp>

namespace rm {

enum ValidByteMask {
    VALID_BYTE_MASK_SENTRY    = 0x01,    // 0000 0001  0x01  哨兵
    VALID_BYTE_MASK_HERO      = 0x02,    // 0000 0010  0x02  英雄
    VALID_BYTE_MASK_ENGINEER  = 0x04,    // 0000 0100  0x04  工程
    VALID_BYTE_MASK_INFANTRY3 = 0x08,    // 0000 1000  0x08  步兵3
    VALID_BYTE_MASK_INFANTRY4 = 0x10,    // 0001 0000  0x10  步兵4
    VALID_BYTE_MASK_INFANTRY5 = 0x20,    // 0010 0000  0x20  步兵5
    VALID_BYTE_MASK_TOWER     = 0x40,    // 0100 0000  0x40  前哨站 基地
    VALID_BYTE_MASK_ALL       = 0x80     // 1000 0000  0x80  全打
};

bool isValidArmorID(ArmorID armor_id, char valid_byte);

//向裁判获取角度偏移目标
double getAngleOffsetTargetToReferee(
    const double yaw,
    const double pitch,
    const double target_x,
    const double target_y,
    const double target_z,
    const double referee_x,
    const double referee_y,
    const double referee_z,
    const double referee_yaw = 0.0,
    const double referee_pitch = 0.0,
    const double axis_x = 0.0,
    const double axis_y = 0.0,
    const double axis_z = 0.0);


//处理装甲板的基本操作
/*
一个接口类，定义了处理装甲板的基本操作
 四个纯虚函数：
 push\pop\refresh\clear
push：一个纯虚函数，用于添加一个新的装甲板，包含装甲板ID、角度和时间戳。
pop：一个纯虚函数，用于移除一个装甲板，并返回其ID。
refresh：一个纯虚函数，用于更新装甲板的状态。
clear：一个纯虚函数，用于清除所有装甲板。
 三个设置函数：
setFocusID：设置当前焦点的装甲板ID。
setExistDt：设置装甲板的存在时间。
setValidID：设置有效的装甲板ID字节。
 成员变量：
focus_id_：当前焦点的装甲板ID。
exist_dt_：装甲板的存在时间，默认为1.0。
valid_byte_：有效的装甲板ID字节，默认为0xFF。
 */
class AttackInterface {
public:
    AttackInterface() {};
    ~AttackInterface() {};

    //armor盔甲板
    virtual void push(ArmorID armor_id, double angle, TimePoint t) = 0;
    virtual ArmorID pop() = 0;
    virtual void refresh() = 0;
    virtual void clear() = 0;

    void setFocusID(ArmorID armor_id) { focus_id_ = armor_id; };
    void setExistDt(double dt) { exist_dt_ = dt; }
    void setValidID(char valid_byte) { valid_byte_ = valid_byte; };


protected:
    ArmorID focus_id_;
    double exist_dt_ = 1.0;
    char valid_byte_ = 0xFF;

};
/*
dist距离,显示攻击范围    canvas_side这是一个变量，代表创建图像的矩阵的宽度和高度
cv::Scalar对象，用于初始化图像矩阵的每个像素值 图像将被初始化为一个灰色的背景
*/

//画布上展示攻击
/*
canvas_side构造函数初始化画布大小和dist最大攻击距离，cv::Scalr初始化图像矩阵的每个像素值为一个灰色背景
push：用于在画布上添加新的攻击点或更新攻击角度。
refresh：用于刷新画布，将更新后的攻击范围显示出来。
 */
class AttackDisplayer {

public:
    AttackDisplayer(int canvas_side = 500, double max_dist = 5) : canvas_side_(canvas_side), max_dist_(max_dist) {
        canvas_ = cv::Mat(canvas_side_, canvas_side_, CV_8UC3, cv::Scalar(50, 50, 50));
    }
    ~AttackDisplayer() {};

    void push(int id, int color, double x, double y);
    void push(int id, double x, double y);
    void push(double body_angle, double head_angle);
    cv::Mat refresh();


private:
    int canvas_side_ = 500;
    double max_dist_ = 5;
    cv::Mat canvas_;

};

}

#endif