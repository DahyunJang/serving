#ifndef SERVING_GATEWAY_SERVING_HANDLE_H_
#define SERVING_GATEWAY_SERVING_HANDLE_H_


#include <map>

#include "gateway/model_id.h"
#include "gateway/serving_node.h"
#include "gateway/serving_node_selector.h"


namespace tensorflow {
namesapce serving{

/*
  fast read dynamic ptr 못씀.
  EM 구조에서 같은 스레드에서 update, read 같이 불릴 수 있음.
*/


/* ServingHandles holds model-servingnode information*/
/* K: model id, V1:vectors of serving nodes , V2: selector */
class ServingHandles {
public:

    void AddServingHandles(const ModelId& model_id,
                           const SP_ServingNode& sp_serving_node){
        handles_map.insert(
            std::pair<ModelId, SP_ServingNode>(model_id, sp_serving_node));
    }

    /*
       이거 파라미터 타입을 레퍼런스로 받아야 할까??
       그리고 -> 쓰는게 이상한데 .. using은 그냥 매크로로 이해해도 되는거지?
     */
    void AddServingHandles(const SP_ServingNode& sp_serving_node){
        cosnt std::vector<ModelId> model_ids = sp_serving_node->GetModelIds();
        for (ModelId& model_id : model_ids){
            AddServingHandles(model_id, sp_serving_node);
        }
    }


    /* logic? */
    SP_ServingNode& GetServingNode (const ModelId& model_id,
                                    ServingNodeSelector& selector){

        return selector().select;
    }

private:
    std::multi_map<ModelId, SP_ServingNode> handles_map;
}


// serving_node_pool가지고 불러야 함..
class ServingHandlesManager {
public:
    /* this is applied to new_ one. needs manual update to swap old one and new */
    void AddServingHandles(const SP_ServingNode& sp_serving_node){
        mutex l(mu_new_);
        /* 차이만 반영해야 하는데.. */
        /* ㄱ드 */
        new_serving_handles->AddServingHandles(sp_serving_node);
    }

    /* get from current */
    SP_ServingNode& GetServingNode (const ModelId& model_id){
        mutex_lock l(mu_current_);
        current_serving_handles->GetServingNode(model_id);
    }


    void Update(){
        mutex l(mu_new_);
        mutex_lock l(mu_current_);
        current_serving_handles = std::move(new_serving_handles);
    }

private:
    //TODO change mu_currnet_ to rw lock ?
    mutex mu_current_;
    mutex mu_new_;
    shared_ptr<ServingHandles> new_serving_handles;
    shared_ptr<ServingHandles> current_serving_handles;
}


} //namespace serving
} //namespace tensorflow

#endif //SERVING_GATEWAY_SERVING_HANDLE_H_
