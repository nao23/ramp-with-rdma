#pragma once

#include "TrxHandler.h"
#include "Item.h"


class LockBasedHandler : public TrxHandler {

public:
    LockBasedHandler() = default;
    ~LockBasedHandler() = default;
    
    void put_all();
    void get_all();
};
