#pragma once
#include <string>
#include <memory>
#include "lasvsim/HttpClient.h"
#include <vector>
#include <optional>
#include <unordered_map>

namespace lasvsim {
    class SimulatorConfig {

    public:
        std::string scenId;
        std::string scenVer;
        std::string simRecordId;
        int maxStep;

        SimulatorConfig(const std::string& scenId, const std::string& scenVer,  const std::string& simRecordId, int maxStep)
            : scenId(scenId), scenVer(scenVer), simRecordId(simRecordId), maxStep(maxStep) {}
    };

    enum class StepCode {
        RUNNING = 0,
        FINISHED = 1001,
        FAILED = 1002
    };

    class StepRes {
    public:
        StepCode code;
        std::string message;
        StepRes(StepCode code, std::string message) : code(code), message(message) {}
    };

    // 位置类型枚举
    enum class PositionType {
        POSITION_TYPE_UNKNOWN = 0,    // 未知
        POSITION_TYPE_IN_LANE = 1,    // 在车道内
        POSITION_TYPE_IN_JUNCTION = 2, // 在路口内
        POSITION_TYPE_OUT_ROAD = 3    // 在道路外
    };


    // 坐标点结构
    class Point {
        public:
            double x;  // x轴坐标
            double y;  // y轴坐标
            double z;  // z轴坐标

            Point() = default;
            Point(double x, double y, double z) : x(x), y(y), z(z) {};
    };

    // 位置信息结构
    class Position {
        public:
            std::string junction_id;  // junction id
            double lane_offset;                      // 车道偏移
            std::string link_id;      // link id
            double patch;             // 俯仰角
            double phi;               // Depracated: 使用heading
            PositionType type;        // 类型(1:在车道内 | 2:在路口内 | 3:在道路外)
            double s;                                // s值
            double dis_to_lane_end;   // 距离车道终点的距离, 单位:[m]
            double t;                                // t值
            double heading;           // 朝向
            std::string lane_id;      // lane id
            int32_t lane_index;                      // 车道编号
            Point point;                             // 位置坐标
            double roll;              // 横滚角
            std::string segment_id;   // segment id

            Position(const std::string& junction_id,
                    double lane_offset,
                    const std::string& link_id,
                    double patch,
                    double phi,
                    PositionType type,
                    double s,
                    double dis_to_lane_end,
                    double t,
                    double heading,
                    const std::string& lane_id,
                    int32_t lane_index,
                    const Point& point,
                    double roll,
                    const std::string& segment_id
            ) {}
    };

    // 基础信息结构
    class ObjBaseInfo {
        public:
            double weight;   // 重量, 单位: [kg]
            double width;    // 宽, 单位: [m]
            double height;   // 高, 单位: [m]
            double length;   // 长, 单位: [m]

            ObjBaseInfo(double weight,
                    double width,
                    double height,
                    double length)
                : weight(weight), width(width), height(height), length(length) {}
    };

    // 动力学信息
    class DynamicInfo {
        public:
            double front_axle_to_center;     // 前轴到重心距离, 单位: [m]
            double front_wheel_stiffness;    // 前轮转弯刚度, 单位: [N/rad]
            double rear_axle_to_center;      // 后轴到重心距离, 单位: [m]
            double rear_wheel_stiffness;     // 后轮转弯刚度, 单位: [N/rad]
            double yaw_moment_of_inertia;    // 重心处的极惯性矩, 单位: [kg*m^2]

            DynamicInfo(double front_axle_to_center,
                    double front_wheel_stiffness,
                    double rear_axle_to_center,
                    double rear_wheel_stiffness,
                    double yaw_moment_of_inertia)
                : front_axle_to_center(front_axle_to_center), front_wheel_stiffness(front_wheel_stiffness),
                rear_axle_to_center(rear_axle_to_center), rear_wheel_stiffness(rear_wheel_stiffness),
                yaw_moment_of_inertia(yaw_moment_of_inertia) {}
    };

    // 运动信息结构
    class ObjMovingInfo {
        public:
            double u;     // 纵向速度, 单位:[m/s]
            double u_acc; // 纵向加速度, 单位:[m/s^2]
            double v;     // 横向速度, 单位:[m/s]
            double v_acc; // 横向加速度, 单位:[m/s^2]
            double w;     // 角速度, 单位:[m/s]
            double w_acc; // 角加速度, 单位:[m/s^2]
        
        ObjMovingInfo(double u,
                    double u_acc,
                    double v,
                    double v_acc,
                    double w,
                    double w_acc)
                : u(u), u_acc(u_acc), v(v), v_acc(v_acc), w(w), w_acc(w_acc) {}
    };

    // 感知目标对象结构
    class PerceptionObject {
        public:
            ObjBaseInfo base_info;      // 感知对象的基础信息
            ObjMovingInfo moving_info;  // 感知对象的运动信息
            std::string obj_id;      // 感知对象的ID
            Position position;       // 感知对象的位置信息

            PerceptionObject(const ObjBaseInfo& base_info,
                            const ObjMovingInfo& moving_info,
                            const std::string& obj_id,
                            const Position& position)
                : base_info(base_info), moving_info(moving_info), obj_id(obj_id), position(position) {}
    };

    // 交通信号灯状态结构
    class MovementSignal {
        public:
            int32_t current_signal;  // 当前灯色(0:无信号灯或信号灯损坏 | 1:红灯 | 2:绿灯 | 3:黄灯)
            int32_t countdown;       // 倒计时, 单位:[s]

            MovementSignal(int32_t current_signal, int32_t countdown)
                : current_signal(current_signal), countdown(countdown) {}
    };

    // 导航信息类
    class NavigationInfo {
        public:
            std::vector<std::string> link_path_nav;        // 子路段导航
            std::string next_junction_id;                  // 下一个路口
            std::string next_movement_id;                  // 下一个movement
            double dis_to_next_junction;                   // 距离下一个路口距离, 路口内为0.0, 单位:[m]
            std::vector<std::string> link_junction_nav;    // 子路段与路口拼接

        public:
            // 构造函数
            NavigationInfo() = default;
            
            NavigationInfo(const std::vector<std::string>& link_path_nav,
                        const std::string& next_junction_id,
                        const std::string& next_movement_id,
                        double dis_to_next_junction,
                        const std::vector<std::string>& link_junction_nav) {}
    };

    // 车辆基本信息
    class VehicleBaseInfo {
        public:
            ObjBaseInfo base_info;
            DynamicInfo dynamic_info;

            VehicleBaseInfo(const ObjBaseInfo& base_info,
                    const DynamicInfo& dynamic_info)
                : base_info(base_info), dynamic_info(dynamic_info) {}
    };

    class Simulator {
        public:
            Simulator() = default;
            
            Simulator(std::shared_ptr<HttpClient> client,
                    const std::shared_ptr<SimulatorConfig>& config);
            virtual ~Simulator() = default;

            void Stop();
            StepRes Step();
            MovementSignal GetMovementSignal(const std::string& movement_id);
            NavigationInfo GetNavigationInfo(const std::string& vehicle_id);
            std::vector<PerceptionObject> GetPerceptionList(const std::string& vehicle_id);
            // 根据车辆id列表获取车辆位置信息
            std::unordered_map<std::string, Position> GetVehiclesPosition(const std::vector<std::string>& vehicle_ids);
            // 设置车辆控制参数
            void SetVehicleControlParam(const std::string& vehicle_id,
                                    double lon_acc,
                                    double ste_wheel);
            // 获取测试车辆id列表
            std::vector<std::string> GetTestVehicleIdList();
            // 获取车辆基本信息
            std::unordered_map<std::string, VehicleBaseInfo> GetVehiclesBaseInfo(const std::vector<std::string>& vehicle_ids);
            // 根据id列表获取车辆运动信息
            std::unordered_map<std::string, ObjMovingInfo> GetVehiclesMovingInfo(const std::vector<std::string>& vehicle_ids);
        private:
            std::shared_ptr<HttpClient> client_;
            std::shared_ptr<SimulatorConfig> config_;
            std::string simulation_id_;
    };
}