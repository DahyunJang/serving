#include "tensorflow_serving/gateway/sn.h"


#include "grpc++/create_channel.h"
#include "grpc++/security/credentials.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;



namespace tensorflow {
namespace serving{
// SN means "ServingNode"

SN::SN(const string& ip_port)
    :ip_port_(ip_port){

    LOG(INFO) << "Construct SN :" << ip_port;

    // TODO error 핸들링 필요
    std::shared_ptr<Channel> channel =
        grpc::CreateChannel(ip_port, grpc::InsecureChannelCredentials());

    stub_ = PredictionService::NewStub(channel);
}

SN::SN(const string& ip_port, bool is_dummy)
    :ip_port_(ip_port){
    LOG(INFO) << "Construct Dummy SN :" << ip_port;
    if (!is_dummy){
        std::shared_ptr<Channel> channel =
            grpc::CreateChannel(ip_port, grpc::InsecureChannelCredentials());

        stub_ = PredictionService::NewStub(channel);
    }
}

SN::~SN(){
    LOG(INFO) << "Destruct SN :" << this->ip_port_;
}


string SN::DebugString() const {
    return strings::StrCat("{ip_port: ", ip_port_, "}");
}

const string& SN::GetIpPort() const{
    return ip_port_;
}

/* load 가 실제 SN에서 로드된건 아니고 객체에 할당만 된 것. */
bool SN::hasModel(const Model& model){
    tf_shared_lock l(mu_);
    return (std::find(models_.cbegin(), models_.cend(), model)
            != models_.cend());
}

/* temporal */
Status SN::LoadModel(const Model& model)
{
    LOG(INFO) << "LoadModel " << model.DebugString();
    {
        mutex_lock l(mu_);
        if (std::find(models_.cbegin(), models_.cend(), model)
            == models_.cend())
            models_.push_back(model);
    }
    return Status::OK();
}

/* temporal */
Status SN::UnloadModel(const Model& model)
{
    LOG(INFO) << "UnLoadModel " << model.DebugString();
    {
        mutex_lock l(mu_);
        models_.remove(model);
    }
    return Status::OK();
}

/* sn 제거로 인한 모델 제거시/모니터에만 쓰여야 함. */
/* 락 제어가 힘든 관계로 벡터를 다 복사한다.
 */
const std::list<Model> SN::GetModels() {
    tf_shared_lock l(mu_);
    return models_;
}

/* const method !! */
/* not yet implemeted */
Status SN::GetModelStatus(const Model& model) const{
    LOG(INFO) << "GetModelStatus " << model.DebugString();
    return Status::OK();
}

/* const method !! */
/* not yet implemeted */
Status SN::Predict(const Model& model) const{
    LOG(INFO) << "Predict " << model.DebugString();
    return Status::OK();
}


} //serving
} //tensorflow
