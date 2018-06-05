#include "gateway/serving_node_pool.h"


namespace tensorflow {
namesapce serving{
    ServingNodePool::ServingNodePool(std::unique_ptr<ServingNodeSelector> selector)
        :serving_node_selector_(std::move(selector)){}

    ServingNodePool::ServingNodePool(){
        ServingNodePool(serving_node_selector_factory
                        .Create(ServingNodeSelectorType::RR));
    }


void ServingNodePool::AddServingNode(SptrServingNode sp_serving_node)
{
    mutex_lock l(mu_);
    if (find_if(sp_serving_nodes_.begin(), sp_serving_nodes_.end(),
                [] (const SptrServingNode& cand){
                    return *cand == *sp_serving_node;})
        == sp_serving_nodes_.end()){
        sp_serving_nodes_.push_back(sp_serving_node);
    }
}


// AddServingNode랑 모양이 다른데 ㅠㅠ
void ServingNodePool::RemoveServingNode(const ServingNode& serving_node)
{
    mutex_lock l(mu_);

    sp_serving_nodes_.remove_if(sp_serving_nodes_.begin(),
                                sp_serving_nodes_.end(),
                                [] (const SptrServingNode& cand){
                                    return *cand == serving_node;});
}


void ServingNodePool::RemoveServingNode(const SptrServingNode& sp_serving_node){
    RemoveServingNode(*sp_serving_node);
}


// selecto 분리
// TODO develope logic with latency or resource evaluation.
SptrServingNode ServingNodePool::GetServingNodeCandidate()
{
    //lock is required. iter can be chaned because of reallocation.
    mutex_lock l(mu_);
    return serving_node_selector_->select(sp_serving_nodes_);
}

} //namespace tensorflow
} //namespace serving
