#include "gateway/request_manager.h"

namespace tensorflow {
namesapce serving{

Status RequestManager::AddSN(const string& ip_port){
    /* TODO create error handling */
    sn_pool_.CreateSN();
    AddHandleOfSN(ip_port);
    return Status::OK();
}

Status RequestManager::RemoveSN(const string& ip_port){
    RemoveHandleOfSN(ip_port);
    sn_pool_.DestroySN(ip_port);
    return Status::OK();
}

/* update only the modified result is changed (status::OK()) */
void RequestManager::UpdateHandles(){
    handles_.Update();
}

/* TODO 기존에 로드되지 않은 하나의 SN에 모델을 로드함. */
Status RequestManager::LoadModel(const Model& model, SptrSN sn = nullptr){
    if (sn == nullptr)
        sn = GetSNCandToLoadModel(model);

    if (sn != nullptr){
        Status status = sn->LoadModel(model);
        handles_.AddHandle(model, sn);
        return status;
    }

    return Status::OK(); // TODO change it to NOT OK
}

Status RequestManager::UnloadModel(const Model& model, SptrSn sn = nullptr){
    handles_.RemoveHandle(model, sn);
    return Status::OK();
}

/* TODO not implemeted */
/* unload model and load model on another SN */
void RequestManager::MigrateHandle(const Model& model, SptrSN from){
    LoadModel(model);
    UnloadModel(model, from);
}

//TODO with context
void RequestManager::Predict(Model& model){
    cosnt SptrSN sn = handles_.GetSN(model);
    sn->Predict();
}

Status RequestManager::AddHandleOfSN(const string& ip_port){
    const SptrSN sn = sn_pool_.GetSN(ip_port);

    /* nullptr 취급 주의 */
    if (sn != nullptr){
        const std::vector<Model> models = sn->GetModels();
        for (const Model& model: models){
            handles_.AddHandle(model, sn);
        }
    }
    return Status::OK();
}

Status RequestManager::RemoveHandleOfSN(const string& ip_port){
    const SptrSN sn = sn_pool_.GetSN(ip_port);

    /* nullptr 취급 주의 */
    if (sn != nullptr){
        const std::vector<Model> models = sn->GetModels();
        for (const Model& model: models){
            handles_.RemoveHandle(model, sn);
        }
    }

    return Status::OK();
}



} //serving
} //tensorflow
