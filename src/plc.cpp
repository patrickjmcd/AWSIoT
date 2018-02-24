//
//  plc.cpp
//  awsiot
//
//  Created by Patrick McDonagh on 2/23/18.
//  Copyright © 2018 Henry Pump. All rights reserved.
//

#include "plc.hpp"

PLC::PLC(string protocol_, string ip_, string cpu_, int portType_, int slot_, int timeout_){
    protocol = protocol_;
    ip = ip_;
    cpu = cpu_;
    portType = portType_;
    slot = slot_;
    timeout = timeout_;
}

string PLC::getCpuPath(){
    if (cpu.compare("micro800") == 0){
        // the plc type is micro800
        return "protocol=" + protocol + "&gateway=" + ip + "&cpu=" + cpu;
    } else {
        return "protocol=" + protocol + "&gateway=" + ip + "&path=" + intToStr(portType) + "," + intToStr(slot) + "&cpu=" + cpu;
    }
}



