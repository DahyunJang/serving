#include "gateway/serving_handles_manager.h"
// TODO
// 업데이트시 무브말고 기존거에 카피함.
// model load 성공시 -> AddServingNodeToServingHandle 부름.
// model unload시 -> RemoveServingNodeToServingHandle 부름.
// 모니터링 실패시 -> invalid로 옮겨 놓는다. 추후처리 ...

namespace tensorflow {
namesapce serving{

/* this is applied to new_ one. needs manual update to swap old one and new */
void ServingHandlesManager::AddServingNodeToServingHandle
    (const ModelId& model_id, SptrServingNode sp_serving_node)
{
    mutex l(mu_new_);
    new_serving_handles
        ->AddServingNodeToServingHandle(model_id, sp_serving_node);
    // WARNING 여기서 update를 부르면 데드락임...ㅋㅋㅋ
    // addServinghandels 를 필요한만큼 다 부른 후 update를 따로 부를 것!
}

/* this is applied to new_ one. needs manual update to swap old one and new */
void ServingHandlesManager::AddInvalidServingNodeToServingHandle
    (const ModelId& model_id, SptrServingNode sp_serving_node)
{
    mutex l(mu_invalid_);
    invalid_serving_handles
        ->AddServingNodeToServingHandle(model_id, sp_serving_node);
}


/* get from current */
SptrServingNode ServingHandlesManager::GetServingNode (const ModelId& model_id){
    tf_shared_lock l(mu_current_);
    current_serving_handles->GetServingNode(model_id);
}

//

void ServingHandlesManager::Update(){
    mutex_lock l(mu_new_);
    mutex_lock l(mu_current_);
    // TODO
    CopyCurrentToNew();
    current_serving_handles = std::move(new_serving_handles);
}

} //namespace serving
} //namespace tensorflow

#endif //SERVING_GATEWAY_SERVING_HANDLE_MANAGER_H_
