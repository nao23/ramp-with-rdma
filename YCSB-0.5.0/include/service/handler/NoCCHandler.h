#pragma once

#include "TrxHandler.h"
#include "Item.h"


class NoCCHandler : public TrxHandler {

public:
    NoCCHandler() = default;
    ~NoCCHandler() = default;

    void put_all();
    void get_all();
};
