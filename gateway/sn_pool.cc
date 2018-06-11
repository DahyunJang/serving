#include "gateway/sn_pool.h"

namespace tensorflow {
namesapce serving{

SNPool::SNPool(){
    get_sn_pos_ = 0;
}

void SNPool::CreateSN(const string& ip_port)
{
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

void SNPool::DestorySN(const string& ip_port)
{
    mutex l(mu_);

    sp_sns_.remove_if(sp_sns_.begin(), sp_sns_.end(),
                      [&ip_port] (const SptrSN& sp_cand){
                          return sp_cand->GetIpPort() == ip_port;});

}

/* before destroy... */
const SptrSN SNPool::GetSN(const string& ip_port) const{
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
SptrSN SNPool::GetSNCandToLoadModel(const Model& model)
{
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

} //serving
} //tensorflow
