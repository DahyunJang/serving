#ifndef SERVING_GATEWAY_SERVING_NODE_H_
#define SERVING_GATEWAY_SERVING_NODE_H_


#include "grpc++/create_channel.h"
#include "grpc++/security/credentials.h"
#include "tensorflow/core/lib/core/errors.h"
#include "tensorflow/core/lib/core/status.h"



using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using tensorflow::serving::FilePredictRequest;
using tensorflow::serving::FilePredictResponse;
using tensorflow::serving::PredictionService;

namespace tensorflow {
namesapce serving{


/// ServingNode holds prediction service interface
/// it caches loaded models on serving_node

class ServingNode {
public:
    /* how to handle stub error ?? */
    /* server_port is [server ip]:[server] */
    ServingNode(const string server_port)
        :server_port_(server_port)
        //TODO initialize handling_model_status_;
    {
        //channel type?
        channel = grpc::CreateChannel(server_port,
                                      grpc::InsecureChannelCredentials());

        // chan
        stub_(PredictionService::NewStub(channel));
    }

    Status FilePredict(const FilePredictRequest &request,
                       FilePredictResponse *response){
        ModelSpec model_spec = request.; ///TODO reference?

        if (IsServingModel(model_spec)){
            return  FilePredict_(request, response);
        }
        else{
            return status_failed;      /* TODO failed */
        }

    }

    Status RestoreLoadedModels(){
        // get loaded models info

        Status status;
        // build request

        status = GetModelStatus_(request, response);
        for (){
            //newly list up models and replace old one.
        }
    }
    Status FlushModels(){
        // flush all loaded models
    }
    Status LoadModel(ModelSpec& model_spec){
        if (status = IsServingModel(model_spec)) {
            return ForwardHandleReload
        }
        else { // already servable
            return // okay status.
        }

    }
    Status UnloadModel(ModelSpec& model_spec){

    }

    bool IsServingModel(ModelSpec& model_spec){
        // check model spec is in hanlding_model_status_ and its loading status
    }

    Status DumpServingModels(){

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

    //This is cache of model stauts in serving node.
    //cache is updated at it loaded/unloaded or unexpected failed.
    std::unique_ptr<vector> model_status_;
    void UpdateModelStatus_(ModelSpec& model_spec, Status status){

    }
}






} //serving
} // tensorflow

#endif SERVING_GATEWAY_SERVING_NODE_H_
