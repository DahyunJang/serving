#include "gateway/serving_node_pool.h"


//흠 거의 단일 스레드 워크인데 뮤텍스가 필요할까?
namespace tensorflow {
namesapce serving{
    ServingNodePool::ServingNodePool(std::unique_ptr<ServingNodeSelector> selector)
        :serving_node_selector_(std::move(selector)){}

    ServingNodePool::ServingNodePool(){
        ServingNodePool(serving_node_selector_factory
                        .Create(ServingNodeSelectorType::RR));
    }


void ServingNodePool::AddServingNode(const SP_ServingNode& sp_serving_node)
{
    mutex_lock l(mu_);
    if (find_if(sp_serving_nodes_.begin(), sp_serving_nodes_.end(),
                [] (const SP_ServingNode& cand){
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
                                [] (const SP_SErvingNode& cand){
                                    return *cand == serving_node;});
}


void ServingNodePool::RemoveServingNode(const SP_ServingNode& sp_serving_node){
    RemoveServingNode(*sp_serving_node);
}


// selecto 분리
// TODO develope logic with latency or resource evaluation.
SP_ServingNode ServingNodePool::GetServingNodeCandidate()
{
    //lock is required. iter can be chaned because of reallocation.
    mutex_lock l(mu_);
    return serving_node_selector_->select(sp_serving_nodes_);
}

} //namespace tensorflow
} //namespace serving
