#pragma once
#include <string>
#include <memory>
#include "lasvsim/HttpClient.h"

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

    // AEB 状态响应
    class SetAEBStatusRes {
    public:
        SetAEBStatusRes() = default;
    };

    class Simulator {
    public:
        Simulator(std::shared_ptr<HttpClient> client,
                  const std::shared_ptr<SimulatorConfig>& config);
        virtual ~Simulator() = default;

        void Stop();
        StepRes Step();
        
        // 设置车辆 AEB(自动紧急制动) 状态
        // vehicleId: 车辆 ID
        // emergencyBraking: 紧急制动状态
        // firstCollisionWarning: 第一次碰撞预警报警
        // secondCollisionWarning: 第二次碰撞预警报警
        SetAEBStatusRes SetAEBStatus(const std::string& vehicleId, 
                                      bool emergencyBraking, 
                                      bool firstCollisionWarning, 
                                      bool secondCollisionWarning);
    private:
        std::shared_ptr<HttpClient> client_;
        std::shared_ptr<SimulatorConfig> config_;
        std::string simulation_id_;
    };
}