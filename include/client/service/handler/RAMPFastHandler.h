#pragma once

#include <boost/range/adaptor/map.hpp>
#include "TrxHandler.h"
#include "Item.h"


class RAMPFastHandler : public TrxHandler {

public:
    RAMPFastHandler(int id);
    ~RAMPFastHandler() = default;

    void put_all();
    void get_all();
};
