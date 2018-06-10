#include GATEWAY_SN_H_
#define GATEWAY_SN_H_

//temporal. change it to log
#include <iostream>

#include "grpc++/create_channel.h"
#include "grpc++/security/credentials.h"

#include "tnesorflow/core/lib/strings/strcat.h"
#include "tensorflow/core/lib/core/errors.h"
#include "tensorflow/core/lib/core/status.h"
#include "tensorflow/core/paltform/mutex.h"
#include "tensorflow/core/paltform/thread_annotations.h"


#include "gateway/model.h"

namespace tensorflow {
namesapce serving{
// SN means "ServingNode"
class SN{
public:
    /* TODO 생성시 장애 발생 대응 */
    SN(const string& ip_port)
        :ip_port_(ip_port){
        stub_(PredictionService::NewStub(
                  grpc::CreateChannel(server_port,
                                      grpc::InsecureChannelCredentials())));
    }

    string DebugString() const {
        return strings::StrCat("{ip_port: ", ip_port_, "}");
    }

    const string& GetIpPort() const{
        return ip_port_;
    }

    /* load 가 실제 SN에서 로드된건 아니고 객체에 할당만 된 것. */
    bool hasModel(const Mdoel& model){
        tf_shared_lock l(mu_);
        return (std::find(models.cbegin(), models.cend(), model)
                != models.cend());
    }

    /* temporal */
    Status LoadModel(const Model& model)
        LOCKS_EXCLUDED(mu_){
        std::cout << "LoadModel " << model.DebugString() << std::endl;
        {
            mutex l(mu_);
            if (std::find(models.cbegin(), models.cend(), model)
                == models.cend())
                models.push_back(model);
        }
        return Status::OK();
    }

    /* temporal */
    Status UnloadModel(const Model& model)
        LOCKS_EXCLUDED(mu_){
        std::cout << "UnLoadModel " << model.DebugString() << std::endl;
        {
            mutex l(mu_);
            remove(models.begin(), models.end(), model);
        }
        return Status::OK();
    }

    /* sn 제거로 인한 모델 제거시/모니터에만 쓰여야 함. */
    /* 락 제어가 힘든 관계로 그냥 벡터를 다 복사한다.
     */
    const std::vector<Model> GetModels() const{
        tf_shared_lock l(mu_);
        return models;
    }

    /* const method !! */
    /* not yet implemeted */
    Status GetModelStatus(const Model& model) const{
        std::cout << "GetModelStatus " << model.DebugString() << std::endl;
    }

    /* const method !! */
    /* not yet implemeted */
    Status Predict(const Model& model) const{
        std::cout << "Predict " << model.DebugString() << std::endl;
    }



private:
    const string ip_port_;
    std::unique_ptr<PredictionService::Stub> stub_;

    mutex mu_;
    std::vector<Model> models_
        GUARDED_BY(mu_);
}

using SptrSN = std::shared_ptr<SN>;


} //serving
} //tensorflow


#endif
