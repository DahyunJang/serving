#ifndef SERVING_GATEWAY_SERVING_NODE_SELECTOR_H_
#define SERVING_GATEWAY_SERVING_NODE_SELECTOR_H_

#include "gateway/serving_node.h"

/* base function */
class ServingNodeSelector{
public:
    virtual ~ServingNodeSelector(){};
    virtual SptrServingNode select(const SptrServingNodeVector& sp_serving_nodes);
}


#endif //SERVING_GATEWAY_SERVING_NODE_SELECTOR_H_
