#ifndef SERVING_GATEWAY_SERVING_MANAGER_H_
#define SERVING_GATEWAY_SERVING_MANAGER_H_


#include "tensorflow/core/lib/core/errors.h"
#include "tensorflow/core/lib/core/status.h"

#include "gateway/serving_node.h"
#include "gateway/model_id.h"
#include "gateway/serving_node_pool.h"
#include "gateway/serving_handle.h"


namespace tensorflow {
namesapce serving{


// For now, assume there is only one node for one model
// comm with IaaS master is out of manager scope.
// Manager should specify the version of model if model version is not specified.

class ServingManager {
public:
    /*  */
    Status FilePredict() {//request and response in json form to proto?

    }

    /* 모델 로딩 관리 */
    //method without serving node specification
    Status LoadModel (ModelConfig& model_config){
        ServingNode serving_node = GetServingNode();
        if () {
            //TODO build request message
            //TODO  and forward it.
            serving_node.
        }
    }

    //method without serving node specification
    Status UnloadModel (ModelConfig& model_config){
        //TODO build request message
        //TODO  and forward it.

    }
    /* servign node 관리  */
    Status AddServingNode(const string& server_port, ){
        auto sp_serving_node = make_shared<ServingNode>(server_port, );
        serving_node_pool_.AddServingNode(sp_serving_node);
        UpdateServingHandles();
    }

    Status DeleteServingNode(){
        //0. local var serving node
        //1. remove the node from serving_node_poo
        UpdateServingHandles();
    }


    /* monitoring */

private:
    ServingNodePool serving_node_pool_;
    ServingHandlesManager serving_handles_;

    void UpdateServingHandles(){
        for (const SP_ServingNode& sp_serving_node :serving_node_pool_.?) {
            serving_handles_.AddServingHandles(sp_serving_node);
        }
        serving_handles_.Update();
    }
}



} //namespace serving
} //namespace tensorflow

#endif //SERVING_GATEWAY_SERVING_MANAGER_H_
