#include "gateway/sn.h"

namespace tensorflow {
namespace serving{
// SN means "ServingNode"

SN::SN(const string& ip_port)
    :ip_port_(ip_port){
    stub_(PredictionService::NewStub(
              grpc::CreateChannel(server_port,
                                  grpc::InsecureChannelCredentials())));
}

string SN::DebugString() const {
    return strings::StrCat("{ip_port: ", ip_port_, "}");
}

const string& SN::GetIpPort() const{
    return ip_port_;
}

/* load 가 실제 SN에서 로드된건 아니고 객체에 할당만 된 것. */
bool SN::hasModel(const Mdoel& model){
    tf_shared_lock l(mu_);
    return (std::find(models.cbegin(), models.cend(), model)
            != models.cend());
}

/* temporal */
Status SN::LoadModel(const Model& model)
{
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
Status SN::UnloadModel(const Model& model)
{
    std::cout << "UnLoadModel " << model.DebugString() << std::endl;
    {
        mutex l(mu_);
        remove(models.begin(), models.end(), model);
    }
    return Status::OK();
}

/* sn 제거로 인한 모델 제거시/모니터에만 쓰여야 함. */
/* 락 제어가 힘든 관계로 벡터를 다 복사한다.
 */
const std::vector<Model> SN::GetModels() const{
    tf_shared_lock l(mu_);
    return models;
}

/* const method !! */
/* not yet implemeted */
Status SN::GetModelStatus(const Model& model) const{
    std::cout << "GetModelStatus " << model.DebugString() << std::endl;
}

/* const method !! */
/* not yet implemeted */
Status SN::Predict(const Model& model) const{
    std::cout << "Predict " << model.DebugString() << std::endl;
}


} //serving
} //tensorflow
