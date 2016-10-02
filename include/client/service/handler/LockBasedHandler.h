#pragma once

#include "TrxHandler.h"
#include "Item.h"


class LockBasedHandler : public TrxHandler {

public:
    LockBasedHandler(int id);
    ~LockBasedHandler() = default;
    
    void put_all();
    void get_all();
};
