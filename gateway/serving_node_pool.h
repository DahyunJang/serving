#ifndef SERVING_GATEWAY_SERVING_NODE_POOL_H__
#define SERVING_GATEWAY_SERVING_NODE_POOL_H_

#include "tensorflow/core/lib/core/errors.h"
#include "tensorflow/core/lib/core/status.h"

#include "gateway/serving_node.h"


namespace tensorflow {
namesapce serving{

/*
  ServingNodePool holds available serving nodes.
  it does not hold information of models.
  it holds only attribute of resource.
*/
class ServingNodePool {
public:
    //vector element 는 자동 destruct 되나 ??


    Status AddServingNode(ServingNode &serving_node){
        // Does ServingNode need to implement equal comparator ?

    }
    Status RemoveServingNode(ServingNode &serving_node){

    }

    ServingNode GetServingNodeCandidate(){
        //returns
        //currently, has no policy though.

    }

private:
    /// TODO
    //MonitorServingNode();

    std::vector<ServingNode> serving_nodes_;
}

} //namespace tensorflow
} //namespace serving

#endif SERVING_GATEWAY_SERVING_NODE_POOL_H_
