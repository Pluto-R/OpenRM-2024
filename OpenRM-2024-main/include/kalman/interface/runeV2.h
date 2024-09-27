#ifndef __OPENRM_KALMAN_INTERFACE_RUNE_V2_H__
#define __OPENRM_KALMAN_INTERFACE_RUNE_V2_H__
#include <utils/timer.h>
#include <kalman/filter/ekf.h>
#include <kalman/filter/kf.h>
#include <structure/slidestd.hpp>
#include <algorithm>

// a in [0.780, 1.045]
// w in [1.884, 2.000]
// b = 2.090 - a

// x, y, z:    预测序列中代表符的中心点坐标，观测序列中代表符的靶心坐标
// theta:      符的朝向
// angle:      当前激活符叶的角度
// spd:        符的旋转角速度
// a:          符的速度三角函数振幅
// w:          符的转动参数omega
// p:          符的角速度相位
// r:          符的半径

// [ x, y, z, theta, angle, spd ]       [ x, y, z, theta, angle]    
// [ 0, 1, 2,   3,     4,    5  ]       [ 0, 1, 2,   3,     4  ]

// [ x, y, z, theta, angle, p, a, w ]   [ x, y, z, theta, angle]    
// [ 0, 1, 2,   3,     4,   5, 6, 7 ]   [ 0, 1, 2,   3,     4  ]

// [ angle, spd ]   [ angle ]
// [   0  ,  1  ]   [   0   ]

namespace rm {

constexpr double A_MIN = 0.780;
constexpr double A_MAX = 1.045;
constexpr double W_MIN = 1.884;
constexpr double W_MAX = 2.000;
constexpr double B_BASE = 2.090;
constexpr double SMALL_RUNE_SPD = M_PI / 3;
constexpr double R = 0.69852;

struct SmallRuneV2_FuncA {
    template<class T>
    void operator()(const T x0[6], T x1[6]) {
        x1[0] = x0[0];
        x1[1] = x0[1];
        x1[2] = x0[2];
        x1[3] = x0[3];
        x1[4] = x0[4] + dt * x0[5];
        x1[5] = x0[5];
    }
    double dt;
};

struct BigRuneV2_FuncA {
    template<class T>
    void operator()(const T x0[8], T x1[8]) {
        x1[0] = x0[0];
        x1[1] = x0[1];
        x1[2] = x0[2];
        x1[3] = x0[3];
        x1[4] = x0[4]
              + sign * dt * (2.090 - x0[6])
              + sign * x0[6] * ceres::sin(x0[5]) * dt;
        x1[5] = x0[5] + x0[7] * dt;
        x1[6] = x0[6];
        x1[7] = x0[7];
        
    }
    double dt;
    double sign = 0.0;
};

struct SmallRuneV2_FuncH {
    template<typename T>
    void operator()(const T x[6], T y[5]) {
        y[0] = x[0] + R * ceres::cos(x[4]) * ceres::sin(x[3]);
        y[1] = x[1] - R * ceres::cos(x[4]) * ceres::cos(x[3]);
        y[2] = x[2] + R * ceres::sin(x[4]);
        y[3] = x[3];
        y[4] = x[4];
    }
};

struct BigRuneV2_FuncH {
    template<typename T>
    void operator()(const T x[8], T y[5]) {
        y[0] = x[0] + R * ceres::cos(x[4]) * ceres::sin(x[3]);
        y[1] = x[1] - R * ceres::cos(x[4]) * ceres::cos(x[3]);
        y[2] = x[2] + R * ceres::sin(x[4]);
        y[3] = x[3];
        y[4] = x[4];
    }
};

struct RuneV2_SpdFuncA {
    double dt;
    template<class T>
    void operator()(T& A) {
        A = T::Identity();
        A(0, 1) = dt;
    }
};

struct RuneV2_SpdFuncH {
    template<class T>
    void operator()(T& H) {
        H = T::Zero();
        H(0, 0) = 1;
    }
};

class RuneV2 {
public:
    RuneV2();
    ~RuneV2() {}

    void push(const Eigen::Matrix<double, 5, 1>& pose, TimePoint t);
    Eigen::Matrix<double, 4, 1> getPose(double append_delay);

    void getStateStr(std::vector<std::string>& str);
    bool getFireFlag(double append_delay);
    void setSmallMatrixQ(double, double, double, double, double, double);
    void setSmallMatrixR(double, double, double, double, double);
    void setBigMatrixQ(double, double, double, double, double, double, double, double);
    void setBigMatrixR(double, double, double, double, double);
    void setSpdMatrixQ(double, double);
    void setSpdMatrixR(double);
    void setRuneType(bool is_big_rune) { is_big_rune_ = is_big_rune; }
    void setAutoFire(double big_spd, double fire_after, double fire_flag_keep, double fire_interval, double to_center) {
        big_rune_fire_spd_ = big_spd;
        fire_after_trans_delay_ = fire_after;
        fire_flag_keep_delay_ = fire_flag_keep;
        fire_interval_delay_ = fire_interval;
        turn_to_center_delay_ = to_center;
    }


private:
    double getAngleTrans(const double, const double);               // 将模型内角度转换为接近新角度
    bool   getRuneTrans(const double, const double);                // 判断是否发生了符页切换
    double getSafeSub(const double, const double);                  // 安全减法

    int      toggle_ = 0;                                           // 切换标签
    int      update_num_ = 0;                                       // 更新次数
    bool     is_big_rune_ = false;                                  // 是否是大符
    bool     is_rune_trans_ = false;                                // 符是否切换
    bool     is_fire_flag_  = false;                                // 当前是否开火

    double   big_rune_fire_spd_      = 1.0;                         // 大符开火角速度
    double   fire_after_trans_delay_ = 0.1;                         // 符切换后多久开火
    double   fire_flag_keep_delay_   = 0.1;                         // 开火信号保留时间
    double   fire_interval_delay_    = 0.5;                         // 两次开火间隔
    double   turn_to_center_delay_   = 1.0;                         // 模型保留时间
    
    
    EKF<6, 5>          small_model_;                                // 运动模型
    EKF<8, 5>          big_model_;                                  // 运动模型
    KF<2, 1>           spd_model_;                                  // 角速度模型

    SmallRuneV2_FuncA  small_funcA_;                                // 运动模型的状态转移函数
    BigRuneV2_FuncA    big_funcA_;                                  // 运动模型的状态转移函数
    RuneV2_SpdFuncA    spd_funcA_;                                  // 角速度模型的状态转移函数

    SmallRuneV2_FuncH  small_funcH_;                                // 运动模型的观测函数
    BigRuneV2_FuncH    big_funcH_;                                  // 运动模型的观测函数
    RuneV2_SpdFuncH    spd_funcH_;                                  // 角速度模型的观测函数

    TimePoint t_;                                                   // 上一次更新的时间
    TimePoint t_trans_;                                             // 上一次符切换时间
    TimePoint t_fire_;                                              // 上一次开火时间

    SlideAvg<double> center_x_;                                     // 符的中心点x坐标
    SlideAvg<double> center_y_;                                     // 符的中心点y坐标
    SlideAvg<double> center_z_;                                     // 符的中心点z坐标
    SlideAvg<double> theta_;                                        // 符的朝向
    SlideAvg<double> spd_;                                          // 符的速度
};


};

#endif