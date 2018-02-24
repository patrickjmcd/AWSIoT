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

using namespace std;


class Tag{
    string tagName;         // tag name
    int  tagType;           // Tag type. As defined on CIP_DATA_TYPE_XXXX
    int  elemCount;         // elements count: 1- single, n-array
    string tagPath;         // computed tag path
    string plcPath;         // computed PLC path
    plc_tag tagPtr;         // PLC Tag pointer. If null means wasn't assigned
    
public:
    bool verbose;
    Tag(string tagName_, int tagType_, int elemCount_, string plcPath_, bool verbose_);
    ~Tag();
    string getTagPath(string cpuPath, bool verbose);
    int getTagType();
    int getElemCount();
    plc_tag getTagPtr();
    static int readIntTag(Tag tag);
    static int_vec_t readIntTagArray(Tag tag);
    static float readFloatTag(Tag tag);
    static float_vec_t readFloatTagArray(Tag tag);
    
    
};


#endif /* tag_hpp */
