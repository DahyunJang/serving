#ifndef SERVING_GATEWAY_SERVING_HANDLE_H_
#define SERVING_GATEWAY_SERVING_HANDLE_H_


#include <vector>

#include "gateway/model_id.h"
#include "gateway/serving_node.h"
#include "gateway/serving_nodes.h"
#include "gateway/serving_node_selector.h"
#include "gateway/serving_node_selector_factory.h"

namespace tensorflow {
namesapce serving{

class ServingHandle{
    /* serving handel should be lock free */
    /* lockign mechanism is up on ServingHandlesmanager*/
    /* Rather than modify it, you should make new for lock-free. */
    /* TODO WANRING 모니터 만들 때도 모디파이 없이 새로 만들 것  */
public:
    ServingHandle(const ModelId& model_id,
                  UptrServingNodeSelector selector
                  = ServingNodeSelectorFactory::Create())
        :model_id_(model_id), selector_(selector){}

    friend bool operator==(const ServingHandle &a, const ServingHandle& b);
    friend bool operator!=(const ServingHandle &a, const ServingHandle& b);
    friend bool operator<(const ServingHandle &a, const ServingHandle& b);

    /* 모델 아이디에 대해 IsServing같은 체크가 없으니 사용시 유의할 것*/
    void AddServingNode(SptrServingNode sp_serving_node){
        sp_serving_nodes_.AddServingNode(sp_serving_node);
    }

    SptrServingNode GetServingNode(){
        return selector_.Select(sp_serving_nodes_);
    }

private:
    const ModelId model_id_;
    UptrServingNodeSelector selector_;
    SptrServingNodes sp_serving_nodes_;
}


bool operator==(const ServingHandle& a, const ServingHandle& b){
    return (a.model_id_ == b.model_id_);
}

bool operator!=(const ServingHandle& a, const ServingHandle& b){
    return !(operator==(a,b));
}

bool operator<(const ServingHandle& a, const ServingHandle& b){
    return (a.modle_id_ < b.model_id_);
}



/* ServingHandles holds model-servingnode information*/
/* K: model id, V1:vectors of serving nodes , V2: selector */
class ServingHandles {
public:

    void AddServingHandle(const ModelId& model_id,
                          SptrServingNode sp_serving_node){
        //1. find ServingHandle by model_id
        //2. add sp_serving_node to handle.
    }

    void AddServingHandles(SptrServingNode sp_serving_node){
        const std::vector<ModelId> model_ids = sp_serving_node->Getstd::vector<ModelId>();
        for (ModelId& model_id : model_ids){
            AddServingHandles(model_id, sp_serving_node);
        }
    }


    /* logic? */
    SptrServingNode GetServingNode (const ModelId& model_id){

        return serving_handle.Select(serving_handle);
    }

private:
    bool FindServingHandle(const ModelId& model_id, ){

    }

    std::vector<ServingHandle> handles_map;
}




} //namespace serving
} //namespace tensorflow

#endif //SERVING_GATEWAY_SERVING_HANDLE_H_
