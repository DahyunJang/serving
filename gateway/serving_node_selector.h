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


#endif SERVING_GATEWAY_SERVING_NODE_SELECTOR_H_
