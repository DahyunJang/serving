#include GATEWAY_HANDLES_H_
#define GATEWAY_HANDLES_H_

#include <map>

#include "tensorflow/core/paltform/mutex.h"
#include "tensorflow/core/paltform/thread_annotations.h"

#include "gateway/model.h"
#include "gateway/sn.h"


namespace tensorflow {
namesapce serving{

using HandleMap = std::unordered_multimap<Model, SptrSN, ModelHash>;

class Handles{
public:

    void AddHandle(const Model& model, SptrSN sn)
        LOCKS_EXCLUDED(mu_update_);

    void RemoveHandle(const Model& model, SptrSN sn = nullptr)
        LOCKS_EXCLUDED(mu_update_);

    void Update()
        LOCKS_EXCLUDED(mu_update_), LOCKS_EXCLUDED(mu_read_);

    const SptrSN GetSN(const Model& model);

private:

    /* no wirte to current_hanldes. this is read only. */
    mutex mu_read_;
    HandleMap read_handles_
        GUARDED_BY(mu_read_);

    /* no read to update_handles. this is write only. */
    mutex mu_update_;
    HandleMap update_handles_
        GUARDED_BY(mu_update_);
}

} //serving
} //tensorflow


#endif
