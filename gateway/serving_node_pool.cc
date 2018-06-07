#include "gateway/serving_node_pool.h"


namespace tensorflow {
namesapce serving{
ServingNodePool::ServingNodePool(ServingNodeSelectorType selector_type){
    selector_ = ServingNodeSelectorFactory::CreateByType(selector_type);
}


ServingNodePool::ServingNodePool(){
    ServingNodePool(serving_node_selector_factory
                    .Create(ServingNodeSelectorType::RR));
}


void ServingNodePool::AddServingNode(SptrServingNode sp_serving_node)
{
    mutex_lock l(mu_);
    sp_serving_nodes_.AddServingNode(sp_serving_node);
}


void ServingNodePool::RemoveServingNode(const ServingNode& serving_node)
{
    mutex_lock l(mu_);
    sp_serving_nodes_.RemvoeServingNode(serving_node);
}


void ServingNodePool::RemoveServingNode(const SptrServingNode& sp_serving_node){
    mutex_lock l(mu_);
    sp_serving_nodes_.RemoveServingNode(sp_serving_node);
}

SptrServingNode ServingNodePool::GetServingNodeCandidate()
{
    //lock is required. iter can be chaned because of reallocation.
    mutex_lock l(mu_);
    return selector_.select(sp_serving_nodes_);
}

} //namespace tensorflow
} //namespace serving
