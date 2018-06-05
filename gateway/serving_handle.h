#ifndef SERVING_GATEWAY_SERVING_HANDLE_H_
#define SERVING_GATEWAY_SERVING_HANDLE_H_


#include <vector>

#include "gateway/model_id.h"
#include "gateway/serving_node.h"
#include "gateway/serving_node_selector.h"
#include "gateway/serving_node_selector_factory.h"

namespace tensorflow {
namesapce serving{

class ServingHandle{
public:
    ServingHandle(const ModelId& model_id,
                  UptrServingNodeSelector selector
                  = ServingNodeSelectorFactory::Create(),
                  SptrServingNodes sp_serving_nodes = nullptr)
        :model_id_(model_id), selector_(selector),
        sp_serving_nodes_(sp_serving_nodes){}

    friend bool operator==(const ServingHandle &a, const ServingHandle& b);
    friend bool operator!=(const ServingHandle &a, const ServingHandle& b);
    friend bool operator<(const ServingHandle &a, const ServingHandle& b);

    SptrServingNode Select(){
        return selector_.Select(sp_serving_nodes_);
    }

private:
    ModelId model_id_;
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
        const ModelIds model_ids = sp_serving_node->GetModelIds();
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
