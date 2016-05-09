#pragma once

#include <random>
#include <algorithm>
#include "Config.h"
#include "Transaction.h"


class TrxQueue {

private:
    ConcurrentQueue<Transaction*> queue;

public:
    TrxQueue() = default;

    void gen_test_trxs();
    bool pop(Transaction*& trx);
};
