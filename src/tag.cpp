//
//  tag.cpp
//  awsiot
//
//  Created by Patrick McDonagh on 2/23/18.
//  Copyright © 2018 Henry Pump. All rights reserved.
//

#include "tag.hpp"


float Tag::readFloat(){
    float val_float;
    int rc;
    /* everything OK? */
    if(!tagPtr) {
        fprintf(stderr,"ERROR: Could not create tag!\n");
    }
    
    /* let the connect succeed we hope */
    while(plc_tag_status(tagPtr) == PLCTAG_STATUS_PENDING) {
        sleep_ms(100);
    }
    
    if(plc_tag_status(tagPtr) != PLCTAG_STATUS_OK) {
        fprintf(stderr,"Error setting up tag internal state. Error %s\n", plc_tag_decode_error(plc_tag_status(tagPtr)));
    }
    
    /* get the data */
    rc = plc_tag_read(tagPtr, DATA_TIMEOUT);
    
    if(rc != PLCTAG_STATUS_OK) {
        fprintf(stderr,"ERROR: Unable to read the data! Got error code %d: %s\n",rc, plc_tag_decode_error(rc));
    }
    val_float = plc_tag_get_float32(tagPtr, 0);
    if (verbose)fprintf(stderr, "%s=%f\n", tagName.c_str(), val_float);
    return val_float;
}

float_vec_t Tag::readFloatArray(){
    int rc;
    int i;
    float_vec_t results;
    float val_float;
    results.reserve(elemCount);
    
    /* everything OK? */
    if(!tagPtr) {
        fprintf(stderr,"ERROR: Could not create tag!\n");
        // return 0;
    }
    
    /* let the connect succeed we hope */
    while(plc_tag_status(tagPtr) == PLCTAG_STATUS_PENDING) {
        sleep_ms(100);
    }
    
    if(plc_tag_status(tagPtr) != PLCTAG_STATUS_OK) {
        fprintf(stderr,"Error setting up tag internal state. Error %s\n", plc_tag_decode_error(plc_tag_status(tagPtr)));
        // return 0;
    }
    
    /* get the data */
    rc = plc_tag_read(tagPtr, DATA_TIMEOUT);
    if(rc != PLCTAG_STATUS_OK) {
        fprintf(stderr,"ERROR: Unable to read the data! Got error code %d: %s\n",rc, plc_tag_decode_error(rc));
        // return 0;
    }
    /* print out the data */
    for(i=0; i < elemCount; i++) {
        val_float = plc_tag_get_float32(tagPtr,(i*getTypeSize(tagType)));
        if (verbose) fprintf(stderr,"%s[%d]=%f\n", tagName.c_str(),i, val_float);
        results.push_back(val_float);
    }
    return results;
}


int Tag::readInt(){
    int val_int;
    int rc;
    
    int tag_size = getTypeSize(tagType);
    /* everything OK? */
    if(!tagPtr) {
        fprintf(stderr,"ERROR: Could not create tag!\n");
    }
    
    /* let the connect succeed we hope */
    while(plc_tag_status(tagPtr) == PLCTAG_STATUS_PENDING) {
        sleep_ms(100);
    }
    
    if(plc_tag_status(tagPtr) != PLCTAG_STATUS_OK) {
        fprintf(stderr,"Error setting up tag internal state. Error %s\n", plc_tag_decode_error(plc_tag_status(tagPtr)));
    }
    
    /* get the data */
    rc = plc_tag_read(tagPtr, DATA_TIMEOUT);
    
    if(rc != PLCTAG_STATUS_OK) {
        fprintf(stderr,"ERROR: Unable to read the data! Got error code %d: %s\n",rc, plc_tag_decode_error(rc));
    }
    if (tag_size == 1){
        if (tagType % 2 == 0){
            val_int = plc_tag_get_int8(tagPtr, 0);
        } else {
            val_int = plc_tag_get_uint8(tagPtr, 0);
        }
    } else if (tag_size == 2){
        if (tagType % 2 == 0){
            val_int = plc_tag_get_int16(tagPtr, 0);
        } else {
            val_int = plc_tag_get_uint16(tagPtr, 0);
        }
    } else if (tag_size == 4){
        if (tagType % 2 == 0){
            val_int = plc_tag_get_int32(tagPtr, 0);
        } else {
            val_int = plc_tag_get_uint32(tagPtr, 0);
        }
    } else {
        val_int = plc_tag_get_int32(tagPtr, 0);
    }
    if (verbose)fprintf(stderr, "%s=%d\n", tagName.c_str(), val_int);
    return val_int;
}

int_vec_t Tag::readIntArray(){
    int rc;
    int i;
    int_vec_t results;
    int tag_size = getTypeSize(tagType);
    int val_int;
    results.reserve(elemCount);
    
    /* everything OK? */
    if(!tagPtr) {
        fprintf(stderr,"ERROR: Could not create tag!\n");
        // return 0;
    }
    
    /* let the connect succeed we hope */
    while(plc_tag_status(tagPtr) == PLCTAG_STATUS_PENDING) {
        sleep_ms(100);
    }
    
    if(plc_tag_status(tagPtr) != PLCTAG_STATUS_OK) {
        fprintf(stderr,"Error setting up tag internal state. Error %s\n", plc_tag_decode_error(plc_tag_status(tagPtr)));
        // return 0;
    }
    
    /* get the data */
    rc = plc_tag_read(tagPtr, DATA_TIMEOUT);
    if(rc != PLCTAG_STATUS_OK) {
        fprintf(stderr,"ERROR: Unable to read the data! Got error code %d: %s\n",rc, plc_tag_decode_error(rc));
        // return 0;
    }
    /* print out the data */
    for(i=0; i < elemCount; i++) {
        if (tag_size == 1){
            if (tagType % 2 == 0){
                val_int = plc_tag_get_int8(tagPtr, 0);
            } else {
                val_int = plc_tag_get_uint8(tagPtr, 0);
            }
        } else if (tag_size == 2){
            if (tagType % 2 == 0){
                val_int = plc_tag_get_int16(tagPtr, 0);
            } else {
                val_int = plc_tag_get_uint16(tagPtr, 0);
            }
        } else if (tag_size == 4){
            if (tagType % 2 == 0){
                val_int = plc_tag_get_int32(tagPtr, 0);
            } else {
                val_int = plc_tag_get_uint32(tagPtr, 0);
            }
        } else {
            val_int = plc_tag_get_int32(tagPtr, 0);
        }
        if (verbose) fprintf(stderr,"%s[%d]=%d\n", tagName.c_str(), i, val_int);
        results.push_back(val_int);
    }
    return results;
}


Tag::Tag(string tagName_, int tagType_, int elemCount_, string plcPath_, float changeThreshold_, int guaranteeSeconds_, bool writeEnabled_, bool verbose_){
    tagName = tagName_;
    tagType = tagType_;
    elemCount = elemCount_;
    plcPath = plcPath_;
    changeThreshold = changeThreshold_;
    guaranteeSeconds = guaranteeSeconds_;
    writeEnabled = writeEnabled_;
    verbose = verbose_;
	forceSend = true;
	lastTimePushed = chrono::system_clock::now();
    
    string tagPath = getTagPath(plcPath);
    tagPtr = plc_tag_create(tagPath.c_str());
    
    if (elemCount > 1){
        if (tagType == CIP_DATA_TYPE_REAL){
            floatArrayValue.reserve(elemCount);
            lastFloatArraySent.reserve(elemCount);
        } else {
            intArrayValue.reserve(elemCount);
            lastIntArraySent.reserve(elemCount);
        }
    }
}

Tag::~Tag(){
    plc_tag_destroy(tagPtr);

}

int Tag::read(){
    if (tagType == CIP_DATA_TYPE_REAL){
        if (elemCount > 1){
            floatArrayValue = readFloatArray();
        } else {
            floatValue = readFloat();
        }
    } else {
        if (elemCount > 1){
            intArrayValue = readIntArray();
        } else {
            intValue = readInt();
        }
    }
    return 0;
}

bool Tag::checkForSend(){
	read();
	
	if (forceSend){
		if (verbose) fprintf(stderr,"%s - checkForSend: True for forceSend\n", tagName.c_str());
		forceSend = false;
		return true;
	}
	
	chrono::time_point<chrono::system_clock> now = chrono::system_clock::now();
	chrono::duration<float> diff = now - lastTimePushed;
	if (diff.count() > guaranteeSeconds){
		if (verbose) fprintf(stderr,"%s - checkForSend: True for age: %f sec\n", tagName.c_str(), diff.count());
		return true;
	}
	
	if (tagType == CIP_DATA_TYPE_REAL){
		if (elemCount > 1){
			bool foundSendNeeded = false;
			for (int i = 0; i < elemCount; i++){
				if (abs(floatArrayValue[i] - lastFloatArraySent[i]) > changeThreshold){
					foundSendNeeded = true;
				}
			}
			if (foundSendNeeded){
				if (verbose) fprintf(stderr,"%s - checkForSend: True for value change in Array\n", tagName.c_str());
				return true;
			}
		} else {
			if (abs(floatValue - lastFloatValueSent) > changeThreshold){
				if (verbose) fprintf(stderr,"%s - checkForSend: True for value change in %f --> %f\n", tagName.c_str(), lastFloatValueSent, floatValue);
				return true;
			}
		}
	} else {
		if (elemCount > 1){
			bool foundSendNeeded = false;
			for (int i = 0; i < elemCount; i++){
				if (abs(intArrayValue[i] - lastIntArraySent[i]) > changeThreshold){
					foundSendNeeded = true;
				}
			}
			if (foundSendNeeded){
				if (verbose) fprintf(stderr,"%s - checkForSend: True for value change in Array\n", tagName.c_str());
				return true;
			}
		} else {
			if (abs(intValue - lastIntValueSent) > changeThreshold){
				if (verbose) fprintf(stderr,"%s - checkForSend: True for value change in %d --> %d\n", tagName.c_str(), lastIntValueSent, intValue);
				return true;
			}
		}
	}
	return false;
}

int Tag::send(){
	// SEND MECHANISM HERE
	
	// Update current values
	lastTimePushed = chrono::system_clock::now();
	
	if (tagType == CIP_DATA_TYPE_REAL){
		if (elemCount > 1){
			lastFloatArraySent = floatArrayValue;
		} else {
			lastFloatValueSent = floatValue;
		}
	} else {
		if (elemCount > 1) {
			lastIntArraySent = intArrayValue;
		} else {
			lastIntValueSent = intValue;
		}
	}
	return 0;
}

string Tag::getTagPath(string cpuPath)
{
    string str = cpuPath + "&elem_count=" + intToStr(elemCount) + "&elem_size=" + intToStr(getTypeSize(tagType)) + "&name=" + tagName;
//    if (verbose) str += "&debug=3"; // add the debug parameters if debug is active. The debug is set using SetDebug
    return str;
};

float Tag::getFloatValue(){
	return floatValue;
}

float_vec_t Tag::getFloatArrayValue(){
	return floatArrayValue;
}

int Tag::getIntValue(){
	return intValue;
}

int_vec_t Tag::getIntArrayValue(){
	return intArrayValue;
}
