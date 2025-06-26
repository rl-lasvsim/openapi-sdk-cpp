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

    class Simulator {
    public:
        Simulator(std::shared_ptr<HttpClient> client,
                  const std::shared_ptr<SimulatorConfig>& config);
        virtual ~Simulator() = default;

        void Stop();
    private:
        std::shared_ptr<HttpClient> client_;
        std::shared_ptr<SimulatorConfig> config_;
        std::string simulation_id_;
    };
}