#include "gateway/serving_handles_manager.h"
// TODO
// 업데이트시 무브말고 기존거에 카피함.
// model load 성공시 -> AddServingNodeToServingHandle 부름.
// model unload시 -> RemoveServingNodeToServingHandle 부름.
// 모니터링 실패시 -> invalid로 옮겨 놓는다. 추후처리 ...
// 업데이트시 CopyCurrentToNew -
namespace tensorflow {
namesapce serving{

/* this is applied to new_ one. needs manual update to swap old one and new */
void ServingHandlesManager::AddServingNodeToServingHandle
    (const ModelId& model_id, SptrServingNode sp_serving_node)
{
    mutex l(mu_new_);
    sp_new_serving_handles
        ->AddServingNodeToServingHandle(model_id, sp_serving_node);
    // WARNING 여기서 update를 부르면 데드락임...ㅋㅋㅋ
    // addServinghandels 를 필요한만큼 다 부른 후 update를 따로 부를 것!
}

/* this is applied to new_ one. needs manual update to swap old one and new */
void ServingHandlesManager::RemoveServingNodeFromServingHandle
    (const ModelId& model_id, SptrServingNode sp_serving_node)
{
    mutex l(mu_new_);
    sp_new_serving_handles
        ->RemoveServingNodeToServingHandle(model_id, sp_serving_node);
}



/* get from current */
SptrServingNode ServingHandlesManager::GetServingNode(const ModelId& model_id){
    tf_shared_lock lc(mu_current_);
    sp_current_serving_handles->GetServingNode(model_id);
}

//

void ServingHandlesManager::CopyCurrentToNew(){
    // 나중에 이 부분 모니터링 로직 반영해 변경할 것.
    // current를 valid/invalid 로 나누고,
    // valid 한 애들만 옮길 것.
    mutex_lock l(mu_new_);
    tf_shared_lock lc(mu_current_);
    *sp_new_serving_handles += *sp_current_serving_handles;
}

void ServingHandlesManager::Move(){
    mutex_lock l(mu_new_);
    mutex_lock lc(mu_current_); //mu_current_ 에 걸리는 유일한 mutex_lock
    sp_current_serving_handles = std::move(sp_new_serving_handles);
}

void ServingHandlesManager::Update(){
    Move();
    CopyCurrentToNew();
}


} //namespace serving
} //namespace tensorflow

#endif //SERVING_GATEWAY_SERVING_HANDLE_MANAGER_H_
