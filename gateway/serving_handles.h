#ifndef SERVING_GATEWAY_SERVING_HANDLE_H_
#define SERVING_GATEWAY_SERVING_HANDLE_H_

#include <vector>
#include "gateway/serving_handle.h"

/* ServingHandles holds model-servingnode information*/
/* K: model id, V1:vectors of serving nodes , V2: selector */
class ServingHandles {
public:

    void AddServingHandle(const ModelId& model_id,
                          SptrServingNode sp_serving_node){
        //TODO 1. find ServingHandle by model_id
        if (){ // 2. if not found add one ServingHandle

        }
        //3. add sp_serving_node to handle.
        serving_handle
    }

    void AddServingHandles(SptrServingNode sp_serving_node){
        const std::vector<ModelId> model_ids
            = sp_serving_node->GetModelIds();
        for (ModelId& model_id : model_ids){
            //TODO 1. find serving_handle by  model_id,
            sp_serving_handle->AddServingHandles(sp_serving_node);
        }
    }


    /* logic? */
    SptrServingNode GetServingNode (const ModelId& model_id){
        //TODO 1. find Servinghandle by model_id
        return sp_serving_handle->GetServingNode();
    }

private:
    std::vector<ServingHandle> handles_map;
}


/* TODO ServingHandle 도 shared ptr로 변경할 것! */


#endif //SERVING_GATEWAY_SERVING_HANDLE_H_
