//
//  tag.hpp
//  awsiot
//
//  Created by Patrick McDonagh on 2/23/18.
//  Copyright © 2018 Henry Pump. All rights reserved.
//

#ifndef tag_hpp
#define tag_hpp

#include <stdio.h>
#include <iostream>
#include <sstream>
#include "utils.h"
#include <libplctag.h>
#include <chrono>
#include <cmath>

using namespace std;


class Tag{
    string tagName;         // tag name
    int  tagType;           // Tag type. As defined on CIP_DATA_TYPE_XXXX
    int  elemCount;         // elements count: 1- single, n-array
    string tagPath;         // computed tag path
    string plcPath;         // computed PLC path
    plc_tag tagPtr;         // PLC Tag pointer. If null means wasn't assigned
    
    chrono::time_point<chrono::system_clock> lastTimePushed; // the last time the value was pushed
    float changeThreshold;      // Amount the value must change by to be pushed
    int guaranteeSeconds;       // Maximum amount of time that must elapse before the next push
    bool writeEnabled;          // writing to the tag is enabled
    
    float lastFloatValueSent;   // The last floating point value pushed
    float floatValue;           // current floating point value
    float_vec_t lastFloatArraySent; // the last floating point array pushed
    float_vec_t floatArrayValue;    // the current floating point array value
    int lastIntValueSent;       // the last integer value pushed
    int intValue;               // current integer value
    int_vec_t lastIntArraySent; // last integer array pushed
    int_vec_t intArrayValue;    // current integer array value
    
    float readFloat();
    float_vec_t readFloatArray();
    int readInt();
    int_vec_t readIntArray();
	bool forceSend;
    
    
    
public:
    bool verbose;
    Tag(
        string tagName_,
        int tagType_,
        int elemCount_,
        string plcPath_,
        float changeThreshold_,
        int guaranteeSeconds_,
        bool writeEnabled_,
        bool verbose_);
    
    ~Tag();
    string getTagPath(string cpuPath);
//    int getTagType();
//    int getElemCount();
//    plc_tag getTagPtr();
    int read();
	bool checkForSend();
	int send();
	float getFloatValue();
	float_vec_t getFloatArrayValue();
	int getIntValue();
	int_vec_t getIntArrayValue();
	
    
    
};


#endif /* tag_hpp */
