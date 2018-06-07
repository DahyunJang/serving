#ifndef SERVING_GATEWAY_SERVING_NODE_POOL_H_
#define SERVING_GATEWAY_SERVING_NODE_POOL_H_

#include "tensorflow/core/lib/core/errors.h"
#include "tensorflow/core/lib/core/status.h"
#include "tensorflow/core/paltform/mutex.h"
#include "tensorflow/core/paltform/thread_annotations.h"


#include "gateway/serving_node.h"
#include "gateway/serving_nodes.h"
#include "gateway/serving_node_selector.h"


namespace tensorflow {
namesapce serving{

/*
  ServingNodePool holds available serving nodes.
  it does not hold information of models.
  it holds only attribute of resource.
*/

class ServingNodePool {
public:
    ServingNodePool(ServingNodeSelectorType selector_type
                    = ServingNodeSelectorType::RR);

    /* create with default selector */
    ServingNodePool();

    void AddServingNode(SptrServingNode serving_node)
        LOCKS_EXCLUDED(mu_);

    void RemoveServingNode(const ServingNode& serving_node)
        LOCKS_EXCLUDED(mu_);

    void RemoveServingNode(const SptrServingNode& sp_serving_node)
        LOCKS_EXCLUDED(mu_);

    SptrServingNode GetServingNodeCandidate()
        LOCKS_EXCLUDED(mu_);
private:
    mutex mu_;
    SptrServingNodes sp_serving_nodes_ GUARDED_BY(mu_);
    ServingNodeSelector selector_;
}

} //namespace tensorflow
} //namespace serving

#endif //SERVING_GATEWAY_SERVING_NODE_POOL_H_
