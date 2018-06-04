#include "serving_node_selector.h"


ServingNodeSelectorRR::ServingNodeSelectorRR()
    :RR_counter_(0){}

/* 내부적으로 락을 잡지 않으니 락은 이 밖에서 잘 잡자 */
virtual SP_ServingNode
ServingNodeSelectorRR::select(const SP_ServingNodeVector& sp_serving_nodes){

    // if the vector is emtpy..
    if(sp_serving_nodes.empty()){
        return nullptr;
    }

    SP_ServingNode sp_serving_node_cand = sp_serving_nodes[RR_counter_];

    RR_counter_++;
    if (RR_counter_ == sp_serving_nodes_.size()){
        RR_counter_ = 0;
    }

    return sp_serving_node_cand;
}
