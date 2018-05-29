#include "gateway/serving_node_pool.h"


//흠 거의 단일 스레드 워크인데 뮤텍스가 필요할까?
namespace tensorflow {
namesapce serving{

Status ServingNodePool::AddServingNode(ServingNode &serving_node)
{
    mutex_lock l(mu_);
    if (find(serving_nodes_.begin(), serving_nodes_.end(), serving_node)
        == serving_nodes_.end()){
        serving_nodes_.push_back(serving_node);
    }
}
Status ServingNodePool::RemoveServingNode(ServingNode &serving_node)
{
    mutex_lock l(mu_);

    if (serving_node == *cur_serving_nodes_iter_){
        cur_serving_nodes_iter_++;
    }
    serving_nodes_.remove(serving_nodes_.begin(), serving_nodes_.end(),
                          serving_node);
}


Status ServingNodePool::GetServingNodeCandidate(ServingNode* serving_node_cand)
{
    //lock is required. iter can be chaned because of reallocation.
    mutex_lock l(mu_);

    // if the vector is emtpy..
    if(serving_nodes_.empty()){
        return errors::OutOfRangeError("vector(serving_nodes) is emtpy");
    }

    //currently, it deos not consider acutral resource.
    //Just use RR policy
    serving_node_cand = *cur_serving_nodes_iter_;

    cur_serving_nodes_iter++;

    if (cur_serving_nodes_iter_ == serving_nodes_.end()){
        cur_serving_nodes_iter_ = serving_nodes_.begin();
    }

    return Stauts::OK();
}

} //namespace tensorflow
} //namespace serving
