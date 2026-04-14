
#include "ApiNodes.hpp"
#include "../../esp/esp_http_server/IEspHttpServer.hpp"
#include "../../node/NodesStaticInfo.hpp"

#include <string>

namespace
{
    std::string json_escape(const std::string &input)
    {
        std::string output;
        output.reserve(input.size() + 8);
        for (char c : input)
        {
            if (c == '\\' || c == '"')
            {
                output.push_back('\\');
            }
            output.push_back(c);
        }
        return output;
    }
} // namespace

ApiNodes::ApiNodes(IEspHttpServer &espHttpServer)
    : espHttpServer_(espHttpServer), nodesStaticInfo_(nullptr) {}
ApiNodes::~ApiNodes() = default;

void ApiNodes::set_nodes_static_info(const NodesStaticInfo &nodesStaticInfo)
{
    nodesStaticInfo_ = &nodesStaticInfo;
}

esp_err_t ApiNodes::get_nodes_info_handler(httpd_req_t *req)
{
    std::string response = "[]";
    if (nodesStaticInfo_ != nullptr)
    {
        response = "[";
        bool isFirstNode = true;
        for (const NodeStaticInfo &nodeStaticInfo : nodesStaticInfo_->nodesStaticInfo)
        {
            if (!isFirstNode)
            {
                response += ",";
            }
            isFirstNode = false;

            response += "{\"id\":" + std::to_string(nodeStaticInfo.nodeId);
            response += ",\"name\":\"" + json_escape(nodeStaticInfo.nodeName) + "\"";
            response += ",\"mac_address\":\"" + json_escape(nodeStaticInfo.macAddress) + "\"";
            response += ",\"firmware_version\":\"" + json_escape(nodeStaticInfo.firmwareVersion) + "\"";
            response += ",\"configuration_version\":\"" + json_escape(nodeStaticInfo.configurationVersion) + "\"";
            response += ",\"ip_address\":\"" + json_escape(nodeStaticInfo.ipAddress) + "\"";
            response += ",\"tasks\":[";

            bool isFirstTask = true;
            for (const TaskStaticInfo &taskStaticInfo : nodeStaticInfo.tasksStaticInfo)
            {
                if (!isFirstTask)
                {
                    response += ",";
                }
                isFirstTask = false;
                response += "{\"id\":" + std::to_string(taskStaticInfo.taskId);
                response += ",\"name\":\"" + json_escape(taskStaticInfo.taskName) + "\"}";
            }

            response += "]}";
        }
        response += "]";
    }

    espHttpServer_.httpd_resp_set_type(req, "application/json");
    espHttpServer_.httpd_resp_send(req, response.c_str(), HTTPD_RESP_USE_STRLEN);
    return 0;
}

esp_err_t ApiNodes::save_nodes_info_handler(httpd_req_t *req)
{
    espHttpServer_.httpd_resp_set_type(req, "application/json");
    espHttpServer_.httpd_resp_send(req, "{\"ack\":\"ok\"}", HTTPD_RESP_USE_STRLEN);
    return 0;
}
