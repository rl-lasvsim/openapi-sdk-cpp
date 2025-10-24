#include <nlohmann/json.hpp> // 需要集成JSON库

#include <iostream>

#include "lasvsim/Simulator.h"

using json = nlohmann::json;


double getDoubleFromJson(const json& j, const std::string& key, double defaultValue = 0.0) {
    if (j.contains(key) && !j[key].is_null()) {
        return j[key].get<double>();
    }
    return defaultValue;
}

std::string getStringFromJson(const json& j, const std::string& key) {
    if (j.contains(key) && !j[key].is_null() && j[key].is_string()) {
        std::string result = j[key].get<std::string>();
        return result.empty() ? "" : result;
    }
    return "";
}

int32_t getInt32FromJson(const json& j, const std::string& key) {
    if (j.contains(key) && !j[key].is_null()) {
        return j[key].get<int32_t>();
    }
    return 0;
}

namespace lasvsim { 
    Simulator::Simulator(std::shared_ptr<HttpClient> client,
                       const std::shared_ptr<SimulatorConfig>& config)
        : config_(config) {

        std::shared_ptr<HttpClient> sharedClient(client.get()->Clone()); // clone the HttpClient
        client_ = sharedClient;
        if (!config_) {
            throw std::invalid_argument("SimulatorConfig cannot be null");
        }

         std::string path = "/openapi/cosim/v2/simulation/init";

         try
         {
            json reqJson;
            reqJson["scen_id"] = config_->scenId;
            reqJson["scen_ver"] = config_->scenVer;
            reqJson["sim_record_id"] = config_->simRecordId;
            reqJson["max_step"] = config_->maxStep;

            std::string reply = client_->Post(path, reqJson.dump());
            
            json replyJson = json::parse(reply);
            client_->AddHeader("x-md-simulation_id", replyJson["simulation_id"]);
            client_->AddHeader("x-md-rl-direct-addr", replyJson["simulation_addr"]);

            simulation_id_ = replyJson["simulation_id"];
         }
         catch(const SDKException e)
         {
            throw e;
         }
        catch (const std::exception& e) {
            throw SDKException(-1,e.what(),SDK_UNKNOWN,path);
        }
    }

    void Simulator::Stop(){
        std::string path = "/openapi/cosim/v2/simulation/stop";
        try
        {
            json reqJson;
            reqJson["simulation_id"] = simulation_id_;
            client_->Post(path, reqJson.dump());
        }
        catch(const SDKException& e)
        {
            throw e;
        }
        catch(const std::exception& e)
        {
            throw SDKException(-1,e.what(),SDK_UNKNOWN,path);
        }
    }

    lasvsim::StepRes Simulator::Step(){ 
        std::string path = "/openapi/cosim/v2/simulation/step";
        try
        {
            json reqJson;
            reqJson["simulation_id"] = simulation_id_;
            std::string reply = client_->Post(path, reqJson.dump());
            json replyJson = json::parse(reply);

            StepRes res(replyJson["code"],replyJson["message"]);

            return res;
        }
        catch(const SDKException& e)
        {
            throw e;
        }
        catch(const std::exception& e)
        {
            throw SDKException(-1,e.what(),SDK_UNKNOWN,path);
        }
    }

    // 获取流向对应信号灯灯色信息
    lasvsim::MovementSignal Simulator::GetMovementSignal(const std::string& movement_id){
        std::string path = "/openapi/cosim/v2/simulation/map/traffic_light/phase_info/get";
        try
        {
            json reqJson;
            reqJson["simulation_id"] = simulation_id_;
            reqJson["movement_id"] = movement_id;
            std::string reply = client_->Post(path, reqJson.dump());
            json replyJson = json::parse(reply);

            int32_t current_signal = replyJson["current_signal"];
            int32_t countdown = replyJson["countdown"];

            MovementSignal movementSignal(current_signal,countdown);

            return movementSignal;
        }
        catch(const SDKException& e)
        {
            throw e;
        }
        catch(const std::exception& e)
        {
            throw SDKException(-1,e.what(),SDK_UNKNOWN,path);
        }
    }

    // 获取导航信息
    lasvsim::NavigationInfo Simulator::GetNavigationInfo(const std::string& vehicle_id){
        std::string path = "/openapi/cosim/v2/simulation/vehicle/idc_vehicle_nav/get";
        try
        {
            json reqJson;
            reqJson["simulation_id"] = simulation_id_;
            reqJson["vehicle_id"] = vehicle_id;
            std::string reply = client_->Post(path, reqJson.dump());

            json replyJson = json::parse(reply);
            NavigationInfo navigationInfo(replyJson["link_path_nav"],
                                         replyJson["next_junction_id"],
                                         replyJson["next_movement_id"],
                                         replyJson["dis_to_next_junction"],
                                         replyJson["link_junction_nav"]);
            return navigationInfo;
        }
        catch(const SDKException& e)
        {
            throw e;
        }
        catch(const std::exception& e)
        {
            throw SDKException(-1,e.what(),SDK_UNKNOWN,path);
        }
    }

    // 获取车辆位置信息
     std::unordered_map<std::string, Position> Simulator::GetVehiclesPosition(const std::vector<std::string>& vehicle_ids) {
        std::string path = "/openapi/cosim/v2/simulation/vehicle/position/get";
        try
        {
            json reqJson;
            reqJson["simulation_id"] = simulation_id_;
            reqJson["id_list"] = vehicle_ids; 
            std::string reply = client_->Post(path, reqJson.dump());
            json replyJson = json::parse(reply);

            std::unordered_map<std::string, Position> position_dict;
            // 返回的数据结构为：{vehicle_id: {position_info}}
            for (auto& item : replyJson["position_dict"].items()) {
                std::string vehicle_id = item.key();

                json value = item.value();
                std::cout << "value: " << value.dump() << std::endl;
                std::cout << "value x: " <<  value["point"]["x"] << std::endl;
                std::cout << "value lane_index: " <<  value["lane_index"] << std::endl;

                double x = value["point"]["x"];
                double y = value["point"]["y"];
                double z = value["point"]["z"];
                
                Position position(
                    getStringFromJson(value,"junction_id"),
                    getDoubleFromJson(value,"lane_offset"),
                    getStringFromJson(value,"link_id"),
                    getDoubleFromJson(value,"patch"),
                    getDoubleFromJson(value,"phi"),
                    value["type"],
                    getDoubleFromJson(value,"s"),
                    getDoubleFromJson(value,"dis_to_lane_end"),
                    getDoubleFromJson(value,"t"),
                    getDoubleFromJson(value,"heading"),
                    getStringFromJson(value,"lane_id"),
                    // value["lane_index"],
                    getInt32FromJson(value,"lane_index"),
                    Point(x, y, z),
                    getDoubleFromJson(value,"roll"),
                    value["segment_id"]
                );

                position_dict.insert(std::make_pair(vehicle_id,position));
            }

            return position_dict;
        }
        catch(const SDKException& e)
        {
            throw e;
        }
        catch(const std::exception& e)
        {
            throw SDKException(-1,e.what(),SDK_UNKNOWN,path);
        }
    }

    // 获取感知信息
    std::vector<PerceptionObject> Simulator::GetPerceptionList(const std::string& vehicle_id) { 
        std::string path = "/openapi/cosim/v2/simulation/vehicle/perception/get";
        json gjson;
        try
        { 
            json reqJson;
            reqJson["simulation_id"] = simulation_id_;
            reqJson["vehicle_id"] = vehicle_id;

            std::string reply = client_->Post(path, reqJson.dump());
            json replyJson = json::parse(reply);
            gjson = replyJson;

            std::vector<PerceptionObject> perception_list;
            for (auto& item : replyJson["list"]) {
                PerceptionObject perception_object(
                    ObjBaseInfo(
                        getDoubleFromJson(item["base_info"],"weight"),
                        getDoubleFromJson(item["base_info"],"width"),
                        getDoubleFromJson(item["base_info"],"height"),
                        getDoubleFromJson(item["base_info"],"length")
                    ),
                    ObjMovingInfo(
                        getDoubleFromJson(item["moving_info"],"u"),
                        getDoubleFromJson(item["moving_info"],"u_acc"),
                        getDoubleFromJson(item["moving_info"],"v"),
                        getDoubleFromJson(item["moving_info"],"v_acc"),
                        getDoubleFromJson(item["moving_info"],"w"),
                        getDoubleFromJson(item["moving_info"],"w_acc")
                    ),
                    getStringFromJson(item,"obj_id"),

                    /*
                    
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
                     */
                    Position(
                        getStringFromJson(item["position"],"junction_id"),
                        getDoubleFromJson(item["position"],"lane_offset"),
                        getStringFromJson(item["position"],"link_id"),
                        getDoubleFromJson(item["position"],"patch"),
                        getDoubleFromJson(item["position"],"phi"),
                        item["position"]["type"],
                        getDoubleFromJson(item["position"],"s"),
                        getDoubleFromJson(item["position"],"dis_to_lane_end"),
                        getDoubleFromJson(item["position"],"t"),
                        getDoubleFromJson(item["position"],"heading"),
                        getStringFromJson(item["position"],"lane_id"),
                        getInt32FromJson(item["position"],"lane_index"),
                        Point(item["position"]["point"]["x"],item["position"]["point"]["y"],item["position"]["point"]["z"]),
                        getDoubleFromJson(item["position"],"roll"),
                        getStringFromJson(item["position"],"segment_id")
                    )
                );

                perception_list.push_back(perception_object);
            }

            return perception_list;
        }
        catch(const SDKException& e)
        {
            throw e;
        }
        catch(const std::exception& e)
        {
            throw SDKException(-1,e.what(),SDK_UNKNOWN,path);
        }
    }

    // 设置车辆控制参数
    void Simulator::SetVehicleControlParam(const std::string& vehicle_id,double lon_acc,double ste_wheel) {
        std::string path = "/openapi/cosim/v2/simulation/vehicle/control/set";
        try
        {
            json reqJson;
            reqJson["simulation_id"] = simulation_id_;
            reqJson["vehicle_id"] = vehicle_id;
            reqJson["lon_acc"] = lon_acc;
            reqJson["ste_wheel"] = ste_wheel;

            client_->Post(path, reqJson.dump());
            return;
        }
        catch(const SDKException& e)
        {
            throw e;
        }
        catch(const std::exception& e)
        {
            throw SDKException(-1,e.what(),SDK_UNKNOWN,path);
        }
    }

    // 获取测试车辆id列表
    std::vector<std::string> Simulator::GetTestVehicleIdList() {
        std::string path = "/openapi/cosim/v2/simulation/test_vehicle/id_list/get";
        try
        {
            json reqJson;
            reqJson["simulation_id"] = simulation_id_;

            std::string reply =  client_->Post(path, reqJson.dump());
            json replyJson = json::parse(reply);
            std::vector<std::string> vehicle_id_list;
            for (auto& item : replyJson["list"]) {
                vehicle_id_list.push_back(item);
            }

            return vehicle_id_list;
        }
        catch(const SDKException& e)
        {
            throw e;
        }
        catch(const std::exception& e)
        {
            throw SDKException(-1,e.what(),SDK_UNKNOWN,path);
        }
    }
}