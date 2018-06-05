#ifndef SERVING_GATEWAY_SERVING_NODE_SELECTOR_RR_H_
#define SERVING_GATEWAY_SERVING_NODE_SELECTOR_RR_H_

#include "serving_node_selector.h"

class ServingNodeSelectorRR : public ServingNodeSelectorBase{
public:
    ServingNodeSelectorRR();

    virtual SptrServingNode select(
        const SptrServingNodes& sp_serving_nodes);

private:
    int RR_counter_;
}

#endif
