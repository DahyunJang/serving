#include "gateway/serving_handle.h"


ServingHandle::ServingHandle(const ModelId& model_id)
    :model_id_(model_id){
    selector_ = ServingNodeSelectorFactory::CreateByType(
        model_id.GetSelectorType());
}


void ServingHandle::AddServingNode(SptrServingNode sp_serving_node){
    sp_serving_nodes_.AddServingNode(sp_serving_node);
}

void ServingHandle::RemoveServingNode(const SptrServingNode& sp_serving_node){
    sp_serving_nodes_.RemoveServingNode(sp_serving_node);
}


SptrServingNode ServingHandle::GetServingNode(){
    return selector_.Select(sp_serving_nodes_);
}


bool ServingHandle::IsEmpty(){
    return sp_serving_nodes_.IsEmpty();
}
n
