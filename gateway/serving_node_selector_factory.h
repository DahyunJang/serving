#ifndef SERVING_GATEWAY_SERVING_NODE_SELECTOR_FACTORY_H_
#define SERVING_GATEWAY_SERVING_NODE_SELECTOR_FACTORY_H_


#include "gateway/serving_node_selector.h"
#include "gateway/serving_node_selector_rr.h"


enum class ServingNodeSelectorType {
    RR,
}


class ServingNodeSelectorFactory {
    static std::unique_ptr<ServingNodeSelector>
        Create(ServingNodeSelectorType type = ServingNodeSelectorType::RR){

        unique_ptr<ServingNodeSelector> selector = nullptr;
        switch(type) {
        case ServingNodeSelectorType::RR :
            selector = make_unique<ServingNodeSelectorRR>();
            break;
        }

        return selector;
    }
}


#endif SERVING_GATEWAY_SERVING_NODE_SELECTOR_FACTRORY_H_
