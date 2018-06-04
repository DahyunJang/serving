#ifndef SERVING_GATEWAY_SERVING_NODE_POOL_H_
#define SERVING_GATEWAY_SERVING_NODE_POOL_H_

#include "tensorflow/core/lib/core/errors.h"
#include "tensorflow/core/lib/core/status.h"
#include "tensorflow/core/paltform/mutex.h"
#include "tensorflow/core/paltform/thread_annotations.h"


#include "gateway/serving_node.h"
#include "gateway/serving_node_selector.h"

//흠 거의 단일 스레드 워크인데 뮤텍스가 필요할까?
namespace tensorflow {
namesapce serving{

/*
  ServingNodePool holds available serving nodes.
  it does not hold information of models.
  it holds only attribute of resource.
*/

class ServingNodePool {
public:
    ServingNodePool(std::unique_ptr<ServingNodeSelector> selector);

    /* create with default selector */
    ServingNodePool();

    void AddServingNode(const SP_ServingNode& serving_node)
        LOCKS_EXCLUDED(mu_);

    void RemoveServingNode(const ServingNode& serving_node)
        LOCKS_EXCLUDED(mu_);

    void RemoveServingNode(const SP_ServingNode& sp_serving_node);

    SP_ServingNode GetServingNodeCandidate()
        LOCKS_EXCLUDED(mu_);
private:
    mutex mu_;
    SP_ServingNodeVector sp_serving_nodes_ GUARDED_BY(mu_);
    std::unique_ptr<ServingNodeSelector> serving_node_selector_;
}

} //namespace tensorflow
} //namespace serving

#endif //SERVING_GATEWAY_SERVING_NODE_POOL_H_
