#include "serving_node_selector_factory.h"

static ServingNodeSelector&
ServingNodeSelectorFactory::Create(ServingNodeSelectorType type){
    switch(type) {
    case ServingNodeSelectorType::RR :
        return ServingNodeSelectorRR();
    }
}