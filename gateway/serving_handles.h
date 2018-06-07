#ifndef SERVING_GATEWAY_SERVING_HANDLE_H_
#define SERVING_GATEWAY_SERVING_HANDLE_H_

#include <vector>
#include "gateway/serving_handle.h"

using SptrServingHandle = std::shared_ptr<ServingHandle>;

/* ServingHandles holds model-servingnode information*/
/* K: model id, V1:vectors of serving nodes , V2: selector */
class ServingHandles {
public:

    /* 기존에 model_id가 같은 ServingHandle이 없어야 함. */
    void AddServingHandle(const ModelId& model_id,
                          ServingNodeSelectorType selector_type
                          = ServingNodeSelectorType::RR){
        SptrServingHandle sp_serving_handle = GetServingHandle(model_id);
        if (sp_serving_handle == nullptr){
            // 2. if not found add one ServingHandle
            sp_serving_handle = std::make_shared<ServingHandle>(model_id,
                                                                selector_type);
            sp_serving_handles.push_back(sp_serving_handle);
        }
    }

    void AddServingNodeToServingHandle(const ModelId& model_id,
                                       SptrServingNode sp_serving_node){
        SptrServingHandle sp_serving_handle = GetServingHandle(model_id);
        if (sp_serving_handle == nullptr){
            /* 음.. ServingHandle 넣어도 되나? */
            sp_serving_handle = std::make_shared<ServingHandle>(model_id);
            sp_serving_handles.push_back(sp_serving_handle);
        }
        sp_serving_handle->AddServingNode(sp_serving_node);
    }

    void AddServingHandleWithServingNode(SptrServingNode sp_serving_node){
        const std::vector<ModelId> model_ids
            = sp_serving_node->GetModelIds();
        for (ModelId& model_id : model_ids){
            AddServingHandles(model_id, sp_serving_node);
        }
    }


    SptrServingNode GetServingNode (const ModelId& model_id){
        SptrServingHandle sp_serving_handle = GetServingHandle(model_id);
        if (sp_serving_handle == nullptr){
            return nullptr;
        }
        return sp_serving_handle->GetServingNode();
    }

private:
    std::vector<SptrServingHandle> sp_serving_handles;

    SptrServingHandel GetServingHandle(const ModelId& model_id){

    }
}

/* QQKQKQKQKQ */
/* TODO ServingHandle 도 shared ptr로 변경할 것! */


#endif //SERVING_GATEWAY_SERVING_HANDLE_H_
