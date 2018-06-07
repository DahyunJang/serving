#include <algorithm>
#include "gateway/serving_handles.h"

/* 기존에 model_id가 같은 ServingHandle이 없어야 함. */
void ServingHandles::AddServingHandle(const ModelId& model_id,
                                      ServingNodeSelectorType selector_type){
    SptrServingHandle sp_serving_handle = GetServingHandle(model_id);
    if (sp_serving_handle == nullptr){
        // 2. if not found add one ServingHandle
        sp_serving_handle = std::make_shared<ServingHandle>(model_id,
                                                            selector_type);
        sp_serving_handles.push_back(sp_serving_handle);
    }
}

void ServingHandles::AddServingNodeToServingHandle(const ModelId& model_id,
                                                   SptrServingNode sp_serving_node){
    SptrServingHandle sp_serving_handle = GetServingHandle(model_id);
    if (sp_serving_handle == nullptr){
        /* 음.. ServingHandle 넣어도 되나? */
        sp_serving_handle = std::make_shared<ServingHandle>(model_id);
        sp_serving_handles.push_back(sp_serving_handle);
    }
    sp_serving_handle->AddServingNode(sp_serving_node);
}

void ServingHandles::AddServingHandleWithServingNode(SptrServingNode sp_serving_node){
    const std::vector<ModelId> model_ids
        = sp_serving_node->GetModelIds();
    for (ModelId& model_id : model_ids){
        AddServingHandles(model_id, sp_serving_node);
    }
}


SptrServingNode ServingHandles::GetServingNode(const ModelId& model_id){
    SptrServingHandle sp_serving_handle = GetServingHandle(model_id);
    if (sp_serving_handle == nullptr){
        return nullptr;
    }
    return sp_serving_handle->GetServingNode();
}

SptrServingHandle ServingHandles::GetServingHandle(const ModelId& model_id){
    SptrServingHandle sp_serving_handle;
    ServingHandle sp_serving_handle_cmp(model_id);
    std::vector<SptrServingHandle>::iterator iter;
    iter = std::find_if(sp_serving_handles.begin(), sp_serving_handles.end(),
                        [&sp_serving_handle_cmp] (const SptrServingHandle& cand){
                            return *cand == sp_serving_handle_cmp});
    if (iter != sp_serving_handles.end()){
        sp_serving_handle = *iter;
    }

    return sp_serving_handle;
}
