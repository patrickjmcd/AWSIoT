//
//  tag.cpp
//  awsiot
//
//  Created by Patrick McDonagh on 2/23/18.
//  Copyright © 2018 Henry Pump. All rights reserved.
//

#include "tag.hpp"

float Tag::readFloatTag(Tag tag){
    float val_float;
    int rc;
    /* everything OK? */
    if(!tag.getTagPtr()) {
        fprintf(stderr,"ERROR: Could not create tag!\n");
    }
    
    /* let the connect succeed we hope */
    while(plc_tag_status(tag.getTagPtr()) == PLCTAG_STATUS_PENDING) {
        sleep_ms(100);
    }
    
    if(plc_tag_status(tag.getTagPtr()) != PLCTAG_STATUS_OK) {
        fprintf(stderr,"Error setting up tag internal state. Error %s\n", plc_tag_decode_error(plc_tag_status(tag.getTagPtr())));
    }
    
    /* get the data */
    rc = plc_tag_read(tag.getTagPtr(), DATA_TIMEOUT);
    
    if(rc != PLCTAG_STATUS_OK) {
        fprintf(stderr,"ERROR: Unable to read the data! Got error code %d: %s\n",rc, plc_tag_decode_error(rc));
    }
    val_float = plc_tag_get_float32(tag.getTagPtr(), 0);
    if (tag.verbose)fprintf(stderr, "data=%f\n", val_float);
    return val_float;
}

float_vec_t Tag::readFloatTagArray(Tag tag){
    int rc;
    int i;
    float_vec_t results;
    float val_float;
    results.reserve(tag.getElemCount());
    
    /* everything OK? */
    if(!tag.getTagPtr()) {
        fprintf(stderr,"ERROR: Could not create tag!\n");
        // return 0;
    }
    
    /* let the connect succeed we hope */
    while(plc_tag_status(tag.getTagPtr()) == PLCTAG_STATUS_PENDING) {
        sleep_ms(100);
    }
    
    if(plc_tag_status(tag.getTagPtr()) != PLCTAG_STATUS_OK) {
        fprintf(stderr,"Error setting up tag internal state. Error %s\n", plc_tag_decode_error(plc_tag_status(tag.getTagPtr())));
        // return 0;
    }
    
    /* get the data */
    rc = plc_tag_read(tag.getTagPtr(), DATA_TIMEOUT);
    if(rc != PLCTAG_STATUS_OK) {
        fprintf(stderr,"ERROR: Unable to read the data! Got error code %d: %s\n",rc, plc_tag_decode_error(rc));
        // return 0;
    }
    /* print out the data */
    for(i=0; i < tag.getElemCount(); i++) {
        val_float = plc_tag_get_float32(tag.getTagPtr(),(i*getTypeSize(tag.getTagType())));
        if (tag.verbose) fprintf(stderr,"data[%d]=%f\n",i, val_float);
        results.push_back(val_float);
    }
    return results;
}


int Tag::readIntTag(Tag tag){
    int val_int;
    int rc;
    
    int tag_size = getTypeSize(tag.getTagType());
    /* everything OK? */
    if(!tag.getTagPtr()) {
        fprintf(stderr,"ERROR: Could not create tag!\n");
    }
    
    /* let the connect succeed we hope */
    while(plc_tag_status(tag.getTagPtr()) == PLCTAG_STATUS_PENDING) {
        sleep_ms(100);
    }
    
    if(plc_tag_status(tag.getTagPtr()) != PLCTAG_STATUS_OK) {
        fprintf(stderr,"Error setting up tag internal state. Error %s\n", plc_tag_decode_error(plc_tag_status(tag.getTagPtr())));
    }
    
    /* get the data */
    rc = plc_tag_read(tag.getTagPtr(), DATA_TIMEOUT);
    
    if(rc != PLCTAG_STATUS_OK) {
        fprintf(stderr,"ERROR: Unable to read the data! Got error code %d: %s\n",rc, plc_tag_decode_error(rc));
    }
    if (tag_size == 1){
        if (tag.getTagType() % 2 == 0){
            val_int = plc_tag_get_int8(tag.getTagPtr(), 0);
        } else {
            val_int = plc_tag_get_uint8(tag.getTagPtr(), 0);
        }
    } else if (tag_size == 2){
        if (tag.getTagType() % 2 == 0){
            val_int = plc_tag_get_int16(tag.getTagPtr(), 0);
        } else {
            val_int = plc_tag_get_uint16(tag.getTagPtr(), 0);
        }
    } else if (tag_size == 4){
        if (tag.getTagType() % 2 == 0){
            val_int = plc_tag_get_int32(tag.getTagPtr(), 0);
        } else {
            val_int = plc_tag_get_uint32(tag.getTagPtr(), 0);
        }
    } else {
        val_int = plc_tag_get_int32(tag.getTagPtr(), 0);
    }
    if (tag.verbose)fprintf(stderr, "data=%d\n", val_int);
    return val_int;
}

int_vec_t Tag::readIntTagArray(Tag tag){
    int rc;
    int i;
    int_vec_t results;
    int tag_size = getTypeSize(tag.getTagType());
    int val_int;
    results.reserve(tag.getElemCount());
    
    /* everything OK? */
    if(!tag.getTagPtr()) {
        fprintf(stderr,"ERROR: Could not create tag!\n");
        // return 0;
    }
    
    /* let the connect succeed we hope */
    while(plc_tag_status(tag.getTagPtr()) == PLCTAG_STATUS_PENDING) {
        sleep_ms(100);
    }
    
    if(plc_tag_status(tag.getTagPtr()) != PLCTAG_STATUS_OK) {
        fprintf(stderr,"Error setting up tag internal state. Error %s\n", plc_tag_decode_error(plc_tag_status(tag.getTagPtr())));
        // return 0;
    }
    
    /* get the data */
    rc = plc_tag_read(tag.getTagPtr(), DATA_TIMEOUT);
    if(rc != PLCTAG_STATUS_OK) {
        fprintf(stderr,"ERROR: Unable to read the data! Got error code %d: %s\n",rc, plc_tag_decode_error(rc));
        // return 0;
    }
    /* print out the data */
    for(i=0; i < tag.getElemCount(); i++) {
        if (tag_size == 1){
            if (tag.getTagType() % 2 == 0){
                val_int = plc_tag_get_int8(tag.getTagPtr(), 0);
            } else {
                val_int = plc_tag_get_uint8(tag.getTagPtr(), 0);
            }
        } else if (tag_size == 2){
            if (tag.getTagType() % 2 == 0){
                val_int = plc_tag_get_int16(tag.getTagPtr(), 0);
            } else {
                val_int = plc_tag_get_uint16(tag.getTagPtr(), 0);
            }
        } else if (tag_size == 4){
            if (tag.getTagType() % 2 == 0){
                val_int = plc_tag_get_int32(tag.getTagPtr(), 0);
            } else {
                val_int = plc_tag_get_uint32(tag.getTagPtr(), 0);
            }
        } else {
            val_int = plc_tag_get_int32(tag.getTagPtr(), 0);
        }
        if (tag.verbose) fprintf(stderr,"data[%d]=%d\n",i, val_int);
        results.push_back(val_int);
    }
    return results;
}

Tag::Tag(string tagName_, int tagType_, int elemCount_, string plcPath_, bool verbose_){
    tagName = tagName_;
    tagType = tagType_;
    elemCount = elemCount_;
    plcPath = plcPath_;
    verbose = verbose_;
    
    string tagPath = getTagPath(plcPath, verbose);
    tagPtr = plc_tag_create(tagPath.c_str());
}

Tag::~Tag(){
    plc_tag_destroy(tagPtr);

}

string Tag::getTagPath(string cpuPath, bool verbose)
{
    string str = cpuPath + "&elem_count=" + intToStr(elemCount) + "&elem_size=" + intToStr(getTypeSize(tagType)) + "&name=" + tagName;
    if (verbose) str += "&debug=3"; // add the debug parameters if debug is active. The debug is set using SetDebug
    return str;
};

plc_tag Tag::getTagPtr(){
    return tagPtr;
}

int Tag::getTagType(){
    return tagType;
}

int Tag::getElemCount(){
    return elemCount;
}
