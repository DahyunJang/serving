#include GATEWAY_REQUEST_MANAGER_H_
#define GATEWAY_REQUEST_MANAGER_H_


#include "tnesorflow/core/lib/strings/strcat.h"
#include "tensorflow/core/lib/core/errors.h"
#include "tensorflow/core/lib/core/status.h"
#include "tensorflow/core/paltform/mutex.h"
#include "tensorflow/core/paltform/thread_annotations.h"



#include "gateway/sn_pool.h"
#include "gateway/handle_manager.h"

namespace tensorflow {
namesapce serving{


/* 음 적당한 이름이 없넹 */
/* 서버 풀에서 받아와서 로딩도 하고, 핸들 매니징도함 */
class RequestManager{
public:

    Status AddSN(const string& ip_port){
        /* TODO create error handling */
        sn_pool_.CreateSN();
        AddHandleOfSN(ip_port);
        return Status::OK();
    }

    Status RemoveSN(const string& ip_port){
        RemoveHandleOfSN(ip_port);
        sn_pool_.DestroySN(ip_port);
        return Status::OK();
    }

    /* update only the modified result is changed (status::OK()) */
    void UpdateHandles(){
        handles_.Update();
    }

    /* TODO 기존에 로드되지 않은 하나의 SN에 모델을 로드함. */
    Status LoadModel(const Model& model, SptrSN sn = nullptr){
        if (sn == nullptr)
            sn = GetSNCandToLoadModel(model);

        if (sn != nullptr){
            Status status = sn->LoadModel(model);
            handles_.AddHandle(model, sn);
            return status;
        }

        return Status::OK(); // TODO change it to NOT OK
    }

    Status UnloadModel(const Model& model, SptrSn sn = nullptr){
        handles_.RemoveHandle(model, sn);
        return Status::OK();
    }

    /* TODO not implemeted */
    /* unload model and load model on another SN */
    void MigrateHandle(const Model& model, SptrSN from){
        LoadModel(model);
        UnloadModel(model, from);
    }

    //TODO with context
    void Predict(Model& model){
        cosnt SptrSN sn = handles_.GetSN(model);
        sn->Predict();
    }

private:
    Status AddHandleOfSN(const string& ip_port){
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

    Status RemoveHandleOfSN(const string& ip_port){
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


    SNPool sn_pool_;
    Handles handles_;
}


} //serving
} //tensorflow


#endif
