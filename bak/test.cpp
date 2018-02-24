//
//  test.cpp
//  awsiot
//
//  Created by Patrick McDonagh on 2/23/18.
//  Copyright © 2018 Henry Pump. All rights reserved.
//

#include "test.hpp"


// Testing CIP wrapper of the libplctag library
void testingCIP()
{
    // uses protocol ab_eip and cpu=Logix with ip=192.168.1.207 located at the backplane (1), in the slot 0, with time out = 3000 for all tag reading
    tcip maxWaterSystem ("ab_eip", "10.20.4.36", "LGX", 1, 0, 3000);
    
    maxWaterSystem.SetDebug (false); // Just needed if want set debug mode to true to see debugging messages. By default debug mode = false, so it is no needed to call SetDebug
    
    int tag[4];
    // Add all tags links
    tag[0] = maxWaterSystem.AddTag ("val_FluidLevel", CIP_DATA_TYPE_REAL, 1);
    tag[1] = maxWaterSystem.AddTag ("val_IntakePressure", CIP_DATA_TYPE_REAL, 1);
    tag[2] = maxWaterSystem.AddTag ("val_IntakeTemperature" , CIP_DATA_TYPE_REAL, 1);
    tag[3] = maxWaterSystem.AddTag ("Energy_Cost", CIP_DATA_TYPE_REAL, 30);
    
//    Write tags values
//    maxWaterSystem.SetTagReal(tag[0], 0, 10.5); maxWaterSystem.WriteTag (tag[0]);
//    maxWaterSystem.SetTagDint(tag[1], 0, 32);   maxWaterSystem.WriteTag (tag[1]);
//    maxWaterSystem.SetTagInt (tag[2], 0, 20);   maxWaterSystem.WriteTag (tag[2]);
//    maxWaterSystem.SetTagSint(tag[3], 0, 14);   maxWaterSystem.WriteTag (tag[3]);
    
//    Set array elements with values 1..8
//    for (int i=0; i< 8 ; i++)
//        maxWaterSystem.SetTagInt (tag[5],i * maxWaterSystem.GetTypeSize(CIP_DATA_TYPE_INT), i+1); // compute offset i * 2
//
//    Update Array
//    maxWaterSystem.WriteTag (tag[5]);
    
//    Another way to write all tags in loop
//    for (int i=0; i< maxWaterSystem.GetTagsCount(); i++)
//        maxWaterSystem.WriteTag (tag[i]);  // In this case because tag[i] has same value that i, the equivalent instruction would be cip.WriteTag (i)
    
    // read all tags
    for (int i=0; i< maxWaterSystem.GetTagsCount() ;i++) {
        maxWaterSystem.ReadTag(tag[i]); // In this case because tag[i] has same value that i, the equivalent instruction would be cip.ReadTag (i)
    };
    
    
    //Show current tag values
    cout << " Tag: " << tag[0] << " Value: " << maxWaterSystem.GetTagReal(tag[0],0) << endl;
    cout << " Tag: " << tag[1] << " Value: " << maxWaterSystem.GetTagReal(tag[1],0) << endl;
    cout << " Tag: " << tag[2] << " Value: " << maxWaterSystem.GetTagReal(tag[2],0) << endl;
//    cout << " Tag: " << tag[3] << " Value: " << maxWaterSystem.GetTagReal(tag[3],0) << endl;
    
    // Show Array elements
    cout << " Tag: " << tag[3] << " Array Values " << endl;
    for (int i=0; i<30; i++)
        cout << i << "  Value " << maxWaterSystem.GetTagReal(tag[3], 2 * i) << endl;
    
    
    // The tags are destroy automatically when the cip object calls the destructor
    // no needed to call cip.DelTagList();
};


int main(int argc, char *argv[])
{
    testingCIP();
    return 0;
}
