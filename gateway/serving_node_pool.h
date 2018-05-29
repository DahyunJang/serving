#ifndef SERVING_GATEWAY_SERVING_NODE_POOL_H__
#define SERVING_GATEWAY_SERVING_NODE_POOL_H_

#include "tensorflow/core/lib/core/errors.h"
#include "tensorflow/core/lib/core/status.h"
#include "tensorflow/core/paltform/mutex.h"
#include "tensorflow/core/paltform/thread_annotations.h"


#include "gateway/serving_node.h"


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
    Status AddServingNode(ServingNode &serving_node)
        LOCKS_EXCLUDED(mu_);

    Status RemoveServingNode(ServingNode &serving_node)
        LOCKS_EXCLUDED(mu_);

    Status GetServingNodeCandidate(ServingNode* serving_node_cand)
        LOCKS_EXCLUDED(mu_);
private:
    /// TODO
    //MonitorServingNode();
    mutex mu_;
    std::vector<ServingNode> serving_nodes_ GUARDED_BY(mu_);
    std::vector<ServingNode>::iterator cur_serving_nodes_iter_
        = serving_nodes_.begin();
}

} //namespace tensorflow
} //namespace serving

#endif SERVING_GATEWAY_SERVING_NODE_POOL_H_
