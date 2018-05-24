#ifndef SERVING_GATEWAY_SERVING_NODE_H_
#define SERVING_GATEWAY_SERVING_NODE_H_

#include <string>
#include <vector>

#include "grpc++/create_channel.h"
#include "grpc++/security/credentials.h"
#include "tensorflow/core/lib/core/errors.h"
#include "tensorflow/core/lib/core/status.h"
#include "tensorflow/core/paltform/mutex.h"
#include "tensorflow/core/paltform/thread_annotations.h"


using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using tensorflow::serving::FilePredictRequest;
using tensorflow::serving::FilePredictResponse;
using tensorflow::serving::PredictionService;

namespace tensorflow {
namesapce serving{

class ModelId{
public:
    ModelId(string& name, int64 version):
    name_(name), version_(version){}

    ModelId(const ModelSpec& model_spec):
    name_(model_spec.name()), version_(model_spec.version().value()){}


    /* vector find 에서 사용되는거 맞지? */
    bool operator==(const ModelId& rhs){
        return ((this.name_ == rhs.name_)
                && (this.version_ == rhs.version_));
    }

private:
    const string name_;            /* model_name */
    const int64 version_;          /* model_version */
};



/// ServingNode holds prediction service interface
/// it caches loaded models on serving_node
class ServingNode {
public:

    /* 중간에 뻑나면 어떻게 처리하지? */
    /* server_port is [server ip]:[server] */
    ServingNode(const string server_port)
        :server_port_(server_port){
        stub_(PredictionService::NewStub(
                  grpc::CreateChannel(server_port,
                                      grpc::InsecureChannelCredentials())));
    }

    bool operator==(const ServingNode& rhs){
        return (this.server_port_ == rhs.server_port_)
    }

    Status FilePredict(const FilePredictRequest &request,
                       FilePredictResponse *response){
        const ModelSpec& model_spec = request.model_spec();

        if (IsServingModel(model_spec)){
            return  FilePredict_(request, response);
        }
        else{
            return tensorflow::errors::NotFound("Could not find the version ",
                                                model_spec.version().value(),
                                                "of model ",
                                                model_spec.name());
        }
    }

    // get loaded models info
    Status RestoreModelIds(){
        Status status;

        GetModelStatusRequest request;
        GetModelStatusResponse response;

        status = GetModelStatus_(request, &response);
        if () {
            for (){
                ModelVersionStatus* version_status
                    = response.get_model_version_status();// TODO 함수이름뭐져 =_=?
                //newly list up models and replace old one.
            }

        }
    }

    Status LoadModel(ModelSpec& model_spec){
        if (!IsServingModel(model_spec)){
            Status status;

            ReloadConfigRequest request;
            ReloadConfigResponse response;
            //TODO build request
            status = HandleReloadConfigRequest_(request, &response);
            //TODO response 처리 ?
        }
        else { // already servable
            return Status::OK();
        }
    }
    Status UnloadModel(ModelSpec& model_spec){
        if (IsServingModel(model_spec)){
            Status status;

            ReloadConfigRequest request;
            ReloadConfigResponse response;
            //TODO build request
            status = HandleReloadConfigRequest_(request, &response);
            //TODO response 처리 ?
        }
        else {
            return Status::OK();
        }
    }

private:
    const string server_port_;
    // When ServinNode destructed, stub_ will removed automatically.
    std::unique_ptr<PredictionService::Stub> stub_;

    //just forwrard request
    Status FilePredict_(const FilePredictRequest &request,
                              FilePredictResponse *response){
        ClientContext context;
        return stub_->FilePredict(&context, request, response);
    }

    Status GetModelStatus_(const GetModelStatusRequest& request,
                                 GetModelStatusResponse *response){
        ClientContext context;
        return stub_->GetModelStatus(&context, reqeust, response);
    }

    Status HandleReloadConfigRequest_(const ReloadConfigRequest& request,
                                            ReloadConfigResponse *response){
        ClientContext context;
        return stub_->HandleReloadConfigRequest(&context, reqeust, response);
    }


    void AddModelId(const ModelSpec& model_spec)
        LOCKS_EXCLUDED(mu_)
    {
        mutex_lock l(mu_);
        ModelId model_id(model_spec);
        model_ids_.push_back(std::move(model_id));
    }

    void RemoveModelId(const ModelSpec& model_spec)
        LOCKS_EXCLUDED(mu_)
    {
        mutex_lock l(mu_);
        ModelId model_id(model_spec);
        model_ids_.remove(model_ids_.begin(), model_ids_.end(), model_id);
    }

    bool IsServingModel(ModelSpec& model_spec)
        LOCKS_EXCLUDED(mu_)
    {
        mutex_lock l(mu_);
        ModelId model_id(model_spec);
        model_ids_.find(model_ids_.begin(), model_ids_.end(), model_id);
    }

    //This is kind a cache of model stauts in serving node.
    mutex mu_;
    std::vector<ModelId> model_ids_ GUARDED_BY(mu_);
}






} //serving
} // tensorflow

#endif SERVING_GATEWAY_SERVING_NODE_H_
