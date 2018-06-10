#include GATEWAY_SN_POOL_H_
#define GATEWAY_SN_POOL_H_

#include <vector>

#include "tensorflow/core/lib/core/errors.h"
#include "tensorflow/core/lib/core/status.h"
#include "tensorflow/core/paltform/mutex.h"
#include "tensorflow/core/paltform/thread_annotations.h"

#include "gateway/sn.h"

namespace tensorflow {
namesapce serving{

class SNPool{
public:
    SNPool(){
        get_sn_pos_ = 0;
    }

    void CreateSN(const string& ip_port)
        LOCKS_EXCLUDED(mu_){
        mutex l(mu_);

        if (find_if(sp_sns_.begin(), sp_sns_.end(),
                    [&ip_port] (const SptrSN& sp_cand){
                        return sp_cand->GetIpPort() == ip_port;})
            == sp_sns_.end()){
            SptrSN sp_sn = std::make_shared<SN>(ip_port);
            sp_sns_.push_back(sp_sn);
        }

        /* TODO 중간에 생성 안되는 경우가 있을 수 있음. */
        return;
    }

    void DestorySN(const string& ip_port)
        LOCKS_EXCLUDED(mu_){
        mutex l(mu_);

        sp_sns_.remove_if(sp_sns_.begin(), sp_sns_.end(),
                          [&ip_port] (const SptrSN& sp_cand){
                              return sp_cand->GetIpPort() == ip_port;});

    }

    /* before destroy... */
    const SptrSN GetSN(const string& ip_port) const{
        auto iter = find_if(sp_sns_.begin(), sp_sns_.end(),
                            [&ip_port] (const SptrSN& sp_cand){
                                return sp_cand->GetIpPort() == ip_port;})
        if (iter == sp_sns_.end()){
            return nullptr;
        }

        return *iter;
    }


    /* get SN candidate Except */
    /* 일단은 라운드로빈만 지원함. */
    SptrSN GetSNCandToLoadModel(const Model& model)
        LOCKS_EXCLUDED(mu_){
        mutex l(mu_);

        std::size_t start_pos = get_sn_pos_;
        do{
            SptrSN cand = sp_sns_[get_sn_pos_];

            if (++get_sn_pos_ == sp_sns_.size())
                get_sn_pos_ = 0;

            if (!cand.hasModel(model))
                return cand;

        } while(start_pos != get_sn_pos_);

        return nullptr;
    }

private:
    mutex mu_;
    std::vector<SptrSN> sp_sns_
        GUARDED_BY(mu_);

    std::size_t get_sn_pos_;
}

} //serving
} //tensorflow


#endif
