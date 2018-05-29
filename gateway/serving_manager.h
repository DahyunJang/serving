#ifndef SERVING_GATEWAY_SERVING_MANAGER_H_
#define SERVING_GATEWAY_SERVING_MANAGER_H_


#include "tensorflow/core/lib/core/errors.h"
#include "tensorflow/core/lib/core/status.h"

#include "gateway/serving_node.h"
// protocol buffer?

namespace tensorflow {
namesapce serving{


// For now, assume there is only one node for one model
// comm with IaaS master is out of manager scope.
// Manager should specify the version of model if model version is not specified.

class ServingManager {
public:

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

    Status FilePredict() {//request and response in json form to proto?

    }

    Status AddServingNode(){
        //1. add the node related pairs in serving_mapper
        //2. add the node from serving_node_pool
    }


    Status DeleteServingNode(){
        //1. remove the node related pairs in serving_mapper
        //2. remove the node from serving_node_pool

    }

private:
    ServingNodePool serving_node_pool_;
    ServingMapper serving_mapper_;
}



} //namespace serving
} //namespace tensorflow

#endif SERVING_GATEWAY_SERVING_MANAGER_H_
