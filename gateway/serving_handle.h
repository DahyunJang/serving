#ifndef SERVING_GATEWAY_SERVING_HANDLE_H_
#define SERVING_GATEWAY_SERVING_HANDLE_H_


#include <map>

#include "gateway/model_id.h"
#include "gateway/serving_node.h"
#include "gateway/serving_node_selector.h"
#include "gateway/"

namespace tensorflow {
namesapce serving{

struct ServingHandle{
public:
    ModelId model_id;
    std::unique_ptr<ServingNodeSelector> selector;
    SptrServingNodeVector sp_serving_nodes;
}



/* ServingHandles holds model-servingnode information*/
/* K: model id, V1:vectors of serving nodes , V2: selector */
class ServingHandles {
public:

    void AddServingHandles(const ModelId& model_id,
                           const SptrServingNode& sp_serving_node){
        handles_map.insert(
            std::pair<ModelId, SptrServingNode>(model_id, sp_serving_node));
    }

    /*
       이거 파라미터 타입을 레퍼런스로 받아야 할까??
       그리고 -> 쓰는게 이상한데 .. using은 그냥 매크로로 이해해도 되는거지?
     */
    void AddServingHandles(const SptrServingNode& sp_serving_node){
        cosnt std::vector<ModelId> model_ids = sp_serving_node->GetModelIds();
        for (ModelId& model_id : model_ids){
            AddServingHandles(model_id, sp_serving_node);
        }
    }


    /* logic? */
    SptrServingNode GetServingNode (const ModelId& model_id,
                                    ServingNodeSelector& selector){

        return selector().select;
    }

private:
    std::multi_map<ModelId, SptrServingNode> handles_map;
}




} //namespace serving
} //namespace tensorflow

#endif //SERVING_GATEWAY_SERVING_HANDLE_H_
