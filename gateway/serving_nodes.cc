#include <algorithm>
#include "gateway/serving_nodes.h"

void SptrServingNodes::AddServingNode(SptrServingNode sp_serving_node){
    if (find_if(sp_serving_nodes_.begin(), sp_serving_nodes_.end(),
                [&sp_serving_node] (const SptrServingNode& cand){
                    return *cand == *sp_serving_node;})
        == sp_serving_nodes_.end()){
        sp_serving_nodes_.push_back(sp_serving_node);
    }
}
void SptrServingNodes::RemoveServingNode(const ServingNode& serving_node){
    sp_serving_nodes_.remove_if(sp_serving_nodes_.begin(),
                                sp_serving_nodes_.end(),
                                [&serving_node] (const SptrServingNode& cand){
                                    return *cand == serving_node;});
}
void SptrServingNodes::RemoveServingNode(const SptrServingNode& sp_serving_node){
    RemoveServingNode(*sp_serving_node);
}

void SptrServingNodes::IsEmpty(){
    return sp_serving_nodes_.empty();
}
