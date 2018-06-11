#ifndef GATEWAY_SN_POOL_H_
#define GATEWAY_SN_POOL_H_

#include <vector>

#include "tensorflow/core/lib/core/errors.h"
#include "tensorflow/core/lib/core/status.h"
#include "tensorflow/core/platform/mutex.h"
#include "tensorflow/core/platform/thread_annotations.h"

#include "gateway/sn.h"

namespace tensorflow {
namespace serving{

class SNPool{
public:
    SNPool();

    /* Create and add SptrSN to sp_sns_ */
    void CreateSN(const string& ip_port)
        LOCKS_EXCLUDED(mu_);

    /* Remove SptrSN from sp_sns_ and destroy by its nauture of shared ptr*/
    void DestroySN(const string& ip_port)
        LOCKS_EXCLUDED(mu_);

    /* before destroy... */
    const SptrSN GetSN(const string& ip_port) const;

    /* get SN candidate Except */
    /* 일단은 라운드로빈만 지원함. */
    SptrSN GetSNCandToLoadModel(const Model& model)
        LOCKS_EXCLUDED(mu_);

private:
    mutex mu_;
    std::vector<SptrSN> sp_sns_
        GUARDED_BY(mu_);

    std::size_t get_sn_pos_;
};

} //serving
} //tensorflow


#endif
