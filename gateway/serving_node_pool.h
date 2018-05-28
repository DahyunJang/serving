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
        LOCKS_EXCLUDED(mu_)
    {
        mutex_lock l(mu_);
        if (find(serving_nodes_.begin(), serving_nodes_.end(), serving_node)
            == serving_nodes_.end()){
            serving_nodes_.push_back(serving_node);
        }
    }
    Status RemoveServingNode(ServingNode &serving_node)
        LOCKS_EXCLUDED(mu_)
    {
        mutex_lock l(mu_);
        serving_nodes_.remove(serving_nodes_.begin(), serving_nodes_.end(),
                              serving_node);
    }

    //TODO return type?
    ServingNode& GetServingNodeCandidate()
        LOCKS_EXCLUDED(mu_)
    {
        //currently, it deos not consider acutral resource.
        //Just use RR policy
        if (serving_nodes_iter_ == serving_nodes_.end()){
            serving_nodes_iter_ = serving_nodes_.begin();
        }

        serving_nodes_iter_++;

        return *serving_nodes_iter_;
    }

private:
    /// TODO
    //MonitorServingNode();
    mutex mu_;
    std::vector<ServingNode> serving_nodes_ GUARDED_BY(mu_);
    std::vector<ServingNode>::iterator serving_nodes_iter_
        = serving_nodes_.begin();
}

} //namespace tensorflow
} //namespace serving

#endif SERVING_GATEWAY_SERVING_NODE_POOL_H_
