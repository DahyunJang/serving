#include <gtest/gtest.h>

#include "tensorflow_serving/gateway/sn_pool.h"


namespace tensorflow{
namespace serving{

TEST(SnPool, CreateSN) {
    SNPool sn_pool;

    const string ip_port1 = "192.168.4.19:8080";
    const string ip_port2 = "192.168.4.19:8081";
    const string ip_port3 = "192.168.4.19:8082";

    sn_pool.CreateSN(ip_port1);
    LOG(INFO) << sn_pool.DebugString();

    sn_pool.CreateSN(ip_port1);
    LOG(INFO) << sn_pool.DebugString();

    sn_pool.CreateSN(ip_port2);
    LOG(INFO) << sn_pool.DebugString();

    EXPECT_NE(sn_pool.GetSN(ip_port1), nullptr);
    EXPECT_NE(sn_pool.GetSN(ip_port2), nullptr);
    EXPECT_EQ(sn_pool.GetSN(ip_port3), nullptr);
}

TEST(SnPool, CreateSNAndGetSN) {
    SNPool sn_pool;

    const string ip_port1 = "192.168.4.19:8080";
    const string ip_port2 = "192.168.4.19:8081";
    const string ip_port3 = "192.168.4.19:8082";

    SptrSN sn;

    sn = sn_pool.CreateSN(ip_port1);
    LOG(INFO) << sn->DebugString();
    LOG(INFO) << sn_pool.DebugString();
    EXPECT_EQ(sn_pool.GetSN(ip_port1), sn);
    EXPECT_EQ(sn_pool.GetSN(ip_port2), nullptr);


    sn = sn_pool.CreateSN(ip_port2);
    LOG(INFO) << sn->DebugString();
    EXPECT_EQ(sn_pool.GetSN(ip_port2), sn);
    LOG(INFO) << sn_pool.DebugString();
}


// 아직 안됨
TEST(SnPool, simpleDestroy1) {
    SNPool sn_pool;

    const string ip_port1 = "192.168.4.19:8080";
    const string ip_port2 = "192.168.4.19:8081";
    const string ip_port3 = "192.168.4.19:8082";

    SptrSN sp_sn1;

    sp_sn1 = sn_pool.CreateSN(ip_port1);
    sp_sn1 = sn_pool.CreateSN(ip_port2);
    sp_sn1 = sn_pool.CreateSN(ip_port3);

    sn_pool.DestroySN(ip_port1);
    sn_pool.DestroySN(ip_port2);
    EXPECT_EQ(sn_pool.GetSN(ip_port1), nullptr);
    sn_pool.DestroySN(ip_port3);

    sn_pool.DebugString();
}

}
}
