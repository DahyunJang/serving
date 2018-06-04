#include "serving_node_selector.h"

class ServingNodeSelectorRR : public ServingNodeSelectorBase{
public:
    ServingNodeSelectorRR();

    virtual SP_ServingNode select(
        const SP_ServingNodeVector& sp_serving_nodes);

private:
    int RR_counter_;
}
