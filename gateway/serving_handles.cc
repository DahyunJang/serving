#include <algorithm>
#include "gateway/serving_handles.h"

void ServingHandles::AddServingNodeToServingHandle(const ModelId& model_id,
                                                   SptrServingNode sp_serving_node){

    SptrServingHandle sp_serving_handle = GetServingHandle(model_id);
    if (sp_serving_handle == nullptr){
        sp_serving_handle = std::make_shared<ServingHandle>(model_id);
        sp_serving_handles.push_back(sp_serving_handle);
    }

    sp_serving_handle->AddServingNode(sp_serving_node);
}

void ServingHandles::RemoveServingNodeToServingHandle(const ModelId& model_id,
                                                      const SptrServingNode& sp_serving_node){
    SptrServingHandle sp_serving_handle = GetServingHandle(model_id);
    if (sp_serving_handle != nullptr){
        //remove here
        sp_serving_handle->RemoveServingNode(sp_serving_node);
        if (sp_serving_handle->IsEmpty()){
            // 맞나.?
            std::remove(sp_serving_handles.begin(), sp_serving_handles.end(), sp_serving_handle);
        }
    }

    sp_serving_handle->AddServingNode(sp_serving_node);
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
