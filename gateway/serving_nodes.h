#ifndef SERVING_GATEWAY_SERVING_NODES_H_
#define SERVING_GATEWAY_SERVING_NODES_H_

#include <vector>
#include <memory>

#include "gateway/serving_node.h"


using SptrServingNode = std::shared_ptr<ServingNode>;

class SptrServingNodes{
    // AddServingNode랑 모양이 다른데 ㅠㅠ
    void AddServingNode(SptrServingNode sp_serving_node);
    void RemoveServingNode(const ServingNode& serving_node);
    void RemoveServingNode(const SptrServingNode& sp_serving_node);
    std::vector<SptrServingNode> GetServingNodes();
private:
    std::vector<SptrServingNode> sp_serving_nodes_;
}

#endif //SERVING_GATEWAY_SERVING_NODES_H_
