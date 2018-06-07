#include <algorithm>
#include "serving_node.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using tensorflow::serving::FilePredictRequest;
using tensorflow::serving::FilePredictResponse;
using tensorflow::serving::PredictionService;

namespace tensorflow {
namesapce serving{


/* server_port is [server ip]:[server] */
ServingNode::ServingNode(const string& server_port,
                         const string& model_base_path = "/tmp/models/")
    :server_port_(server_port),
    model_base_path_(model_base_path){
        stub_(PredictionService::NewStub(
                  grpc::CreateChannel(server_port,
                                  grpc::InsecureChannelCredentials())));
}


void ServingNode::AddModelId(const ModelId& model_id)
{
    mutex_lock l(mu_);
    if (!IsServingModel(model_id)){
        model_ids_.push_back(model_id);
    }
}

void ServingNode::RemoveModelId(const ModelId& model_id)
{
    mutex_lock l(mu_);
    model_ids_.remove(model_ids_.begin(), model_ids_.end(), model_id);
}



Status ServingNode::ReloadModels(){
    // model_ids_ 로 리퀘스트 빌드

    return ;
}


Status ServingNode::UpdateModelStatus(const ModelId& model_id){
    // 해당 모델이 정상적으로 로딩되었는지 확인.
    // 버전 정보는 어떻게 하지? 무시해?
    return;
}

// TODO how to handle error?
Status ServingNode::UpdateModelStatus(){
    // update all
    for_each(auto model_id_iter : model_ids_){
        UpdateModelStatus(*model_id_iter);
    }
}

const std::vector<ModelId>& GetModelIds() const{
    return model_ids_;
}


Status ServingNode::FilePredict(const ModelId& model_id,
                                const string& input_data_file_path,
                                const string& output_data_file_path){

    //ModelId model_id = ; get modelId from
    if (IsServingModel(model_id)){
        Status status;

        FilePredictRequest request;
        FilePredictResponse response;


        // TODO REQUIRED build request and response
        ModelSpec& model_spec = request.model_spec(); //
        model_spec.set;//
            reqeust.set_input_file_path(input_data_file_path);//
            reqeust.set_output_file_path(output_data_file_path);//
        //...

        status = FilePredict_(request, response);

        // TODO REQUIRED handle response?

        return status;
    }
    else{
        return tensorflow::errors::NotFound("Could not find the version ",
                                            model_spec.version().value(),
                                            "of model ",
                                            model_spec.name());
    }
}


// TODO
const Status ServingNode::GetModelStatus_(request, response) const{
    ClientContet context;
    return stub_->(&context, request, response);
}


Status ServingNode::HandleReloadConfigRequest_(const ReloadConfigRequest& request,
                                               ReloadConfigResponse *response){
    ClientContext context;
    return stub_->HandleReloadConfigRequest(&context, reqeust, response);
}

Status ServingNode::FilePredict_(const FilePredictRequest &request,
                    FilePredictResponse *response){
    ClientContext context;
    return stub_->FilePredict(&context, request, response);
}



bool ServingNode::IsServingModel(ModelId& model_id)
{
    mutex_lock l(mu_);

    if (std::find(model_ids_.begin(), model_ids_.end(), model_id)
        == model_ids_.end()){
        return false;
    }

    return true;
}



} //serving
} // tensorflow
