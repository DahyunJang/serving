#include "gateway/serving_handle.h"


ServingHandle::ServingHandle(const ModelId& model_id,
              ServingNodeSelectorType selector_type)
    :model_id_(model_id){
    selector_ = ServingNodeSelectorFactory::CreateByType(selector_type);
}


void ServingHandle::AddServingNode(SptrServingNode sp_serving_node){
    sp_serving_nodes_.AddServingNode(sp_serving_node);
}

SptrServingNode ServingHandle::GetServingNode(){
    return selector_.Select(sp_serving_nodes_);
}
