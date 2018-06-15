#include "tensorflow_serving/gateway/request_manager.h"

namespace tensorflow {
namespace serving{

bool RequestManager::AddSN(const string& ip_port){
    /* TODO create error handling */
    sn_pool_.CreateSN(ip_port);
    return AddHandleOfSN(ip_port);
}

bool RequestManager::RemoveSN(const string& ip_port){
    bool ret = RemoveHandleOfSN(ip_port);
    sn_pool_.DestroySN(ip_port);
    return ret;
}

/* update only the modified result is changed (status::OK()) */
void RequestManager::UpdateHandles(){
    handles_.UpdateHandle();
}

/* TODO 기존에 로드되지 않은 하나의 SN에 모델을 로드함. */
bool RequestManager::LoadModel(const Model& model, const string& ip_port){
    bool ret = false;

    const SptrSN sn = sn_pool_.GetSN(ip_port);

    if (sn != nullptr){
        Status status = sn->LoadModel(model);
        // TODO
        // handle status
        //if status is okay, add handle
        ret = handles_.AddHandle(model.GetModelName(), sn);
    }
    return ret;
}

bool RequestManager::UnloadModel(const Model& model, const string& ip_port){
    bool ret = false;

    const SptrSN sn = sn_pool_.GetSN(ip_port);

    if (sn != nullptr){
        Status status = sn->UnloadModel(model);
        // TODO handles status
        // if status okay then remove handle
        ret =  handles_.RemoveHandle(model.GetModelName(), sn);
    }
    return ret;
}


//TODO with context
bool RequestManager::Predict(const string& model_name){
    bool ret = false;

    const SptrSN sn = handles_.GetSN(model_name);

    if (sn != nullptr){
        sn->Predict(model_name);
        ret = true;
    }

    return ret;
}

bool RequestManager::AddHandleOfSN(const string& ip_port){
    bool ret = false;
    const SptrSN sn = sn_pool_.GetSN(ip_port);

    if (sn != nullptr){
        const std::list<Model> models = sn->GetModels();
        for (const Model& model: models){
            handles_.AddHandle(model.GetModelName(), sn);
        }
        ret = true;
    }
    return ret;
}

bool RequestManager::RemoveHandleOfSN(const string& ip_port){
    bool ret = false;
    const SptrSN sn = sn_pool_.GetSN(ip_port);
    if (sn != nullptr){
        const std::list<Model> models = sn->GetModels();
        for (const Model& model: models){
            handles_.RemoveHandle(model.GetModelName(), sn);
        }
        ret = true;
    }
    return ret;
}



} //serving
} //tensorflow
