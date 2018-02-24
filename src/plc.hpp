//
//  plc.hpp
//  awsiot
//
//  Created by Patrick McDonagh on 2/23/18.
//  Copyright © 2018 Henry Pump. All rights reserved.
//

#ifndef plc_hpp
#define plc_hpp

#include <stdio.h>
#include <iostream>
#include <sstream>
#include "utils.h"

using namespace std;


class PLC {
    string protocol;        // protocol type: ab_eip, ab_cip
    string ip;              // IP address: 192.168.1.10
    string cpu;             // AB CPU model: plc,plc5,slc,slc500,micrologix,mlgx,compactlogix,clgx,lgx,controllogix,contrologix,flexlogix,flgx
    int    portType;        // Communication Port Type: 1- Backplane, 2- Control Net/Ethernet, DH+ Channel A, DH+ Channel B, 3- Serial
    int    slot;            // Slot number where cpu is installed
    long   timeout;         // Time out for reading/writing tags
public:
    PLC(string protocol, string ip, string cpu, int portType, int slot, int timeout);
    string getCpuPath();
};



#endif /* plc_hpp */
