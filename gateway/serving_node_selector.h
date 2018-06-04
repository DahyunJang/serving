#ifndef SERVING_GATEWAY_SERVING_NODE_SELECTOR_H_
#define SERVING_GATEWAY_SERVING_NODE_SELECTOR_H_

#include "gateway/serving_node.h"


/* TODO
   ServingNodeSelector, ServingNodeSelectorFactory, ServingNodeSelectorType  */


class ServingNodeSelectorBase{
public:
    virtual ~ServingNodeSelector(){};
    virtual SP_ServingNode select(const SP_ServingNodeVector& sp_serving_nodes);
}

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

        /* case for other type... */
        /* default? */
        }

        return selector;
    }
}


#endif SERVING_GATEWAY_SERVING_NODE_SELECTOR_H_
