#include <nlohmann/json.hpp> // 需要集成JSON库

#include <iostream>

#include "lasvsim/Simulator.h"

using json = nlohmann::json;

namespace lasvsim { 
    Simulator::Simulator(std::shared_ptr<HttpClient> client,
                       const std::shared_ptr<SimulatorConfig>& config)
        : config_(config) {

        std::shared_ptr<HttpClient> sharedClient(client.get()->clone()); // clone the HttpClient
        client_ = sharedClient;
        if (!config_) {
            throw std::invalid_argument("SimulatorConfig cannot be null");
        }

         json reqJson;
         reqJson["scen_id"] = config_->scenId;
         reqJson["scen_ver"] = config_->scenVer;
         reqJson["sim_record_id"] = config_->simRecordId;
         reqJson["max_step"] = config_->maxStep;

        std::string reply = client_->Post("/openapi/cosim/v2/simulation/init", reqJson.dump());
        
        json replyJson = json::parse(reply);
        client_->addHeader("x-md-simulation_id", replyJson["simulation_id"]);
        client_->addHeader("x-md-rl-direct-addr", replyJson["simulation_addr"]);

        simulation_id_ = replyJson["simulation_id"];
    }

    void Simulator::stop(){
        json reqJson;
        reqJson["simulation_id"] = simulation_id_;
        client_->Post("/openapi/cosim/v2/simulation/stop", reqJson.dump());
    }
}