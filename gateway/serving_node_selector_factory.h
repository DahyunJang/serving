#ifndef SERVING_GATEWAY_SERVING_NODE_SELECTOR_FACTORY_H_
#define SERVING_GATEWAY_SERVING_NODE_SELECTOR_FACTORY_H_


#include "gateway/serving_node_selector.h"
#include "gateway/serving_node_selector_rr.h"


enum class ServingNodeSelectorType {
    RR,
}


class ServingNodeSelectorFactory {
    static std::unique_ptr<ServingNodeSelector>
        Create(ServingNodeSelectorType type = ServingNodeSelectorType::RR);
}

#endif //SERVING_GATEWAY_SERVING_NODE_SELECTOR_FACTRORY_H_
