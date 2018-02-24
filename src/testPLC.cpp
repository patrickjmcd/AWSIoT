
#include "testPLC.hpp"

bool verbose = false;
bool print = false;

int main(int argc, char *argv[])
{
    int opt;
    while ((opt = getopt(argc, argv, "vp")) != -1)
        switch (opt) {
            case 'v':
                verbose = true;break;
            case 'p':
                print = true;break;
            default:
                fprintf(stderr, "Usage: %s [-v] [-p]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    
    PLC maxWaterSystemPlc = PLC("ab_eip", "10.20.4.36", "compactlogix", 1, 0, DATA_TIMEOUT);
    
    PLC pioneerFracSkidPlc = PLC("ab_eip", "10.20.4.40", "micro800", 1, 0, DATA_TIMEOUT);
    
    Tag fluidLevelTag = Tag("val_FluidLevel", CIP_DATA_TYPE_REAL, 1, maxWaterSystemPlc.getCpuPath(), verbose);
    Tag intakePressureTag = Tag("val_IntakePressure", CIP_DATA_TYPE_REAL, 1, maxWaterSystemPlc.getCpuPath(), verbose);
    Tag intakeTemperatureTag = Tag("val_IntakeTemperature", CIP_DATA_TYPE_REAL, 1, maxWaterSystemPlc.getCpuPath(), verbose);
    Tag alarmDowntimeSecTag = Tag("cfg_AlarmDowntimeSec", CIP_DATA_TYPE_DINT, 1, maxWaterSystemPlc.getCpuPath(), verbose);
    Tag rpFlowmeterTag = Tag("rp_Flowmeter", CIP_DATA_TYPE_BOOL, 1, maxWaterSystemPlc.getCpuPath(), verbose);
    Tag energyCostTag = Tag("Energy_Cost", CIP_DATA_TYPE_REAL, 30, maxWaterSystemPlc.getCpuPath(), verbose);
    
    Tag flowTotalizerBTag = Tag("val_FlowTotalizerB", CIP_DATA_TYPE_REAL, 1, pioneerFracSkidPlc.getCpuPath(), verbose);
    
    
    float fluidLevel = Tag::readFloatTag(fluidLevelTag);
    float intakePressure = Tag::readFloatTag(intakePressureTag);
    float intakeTemperature = Tag::readFloatTag(intakeTemperatureTag);
    int alarmDowntimeSec = Tag::readIntTag(alarmDowntimeSecTag);
    int rpFlowmeter = Tag::readIntTag(rpFlowmeterTag);
    float_vec_t energyCost = Tag::readFloatTagArray(energyCostTag);
    float flowTotalizerB = Tag::readFloatTag(flowTotalizerBTag);
    
    
    if (print == true){
        printf("--\nMAX WATER SYSTEM\n--\n");
        printf("--\n");
        printf("Fluid Level = %f\n--\n", fluidLevel);
        printf("Intake Temperature = %f\n--\n", intakeTemperature);
        printf("Intake Pressure = %f\n--\n", intakePressure);
        printf("Alarm Downtime Sec = %d\n--\n", alarmDowntimeSec);
        printf("Run Permissive: Flowmeter = %d\n--\n", rpFlowmeter);
        for(int i = 0; i < energyCost.size(); i++){
            printf("Energy Cost [%d] = %f\n", i, energyCost[i]);
        }
        printf("--\nPIONEER FRAC SKID\n--\n");
        printf("Flow Totalizer B = %f\n--\n", flowTotalizerB);
        printf("--\n");
    }
    
    
    // /* now test a write */
    // for(i=0; i < ELEM_COUNT; i++) {
    //     int32_t val = plc_tag_get_int32(tag,(i*ELEM_SIZE));
    //
    //     val = val+1;
    //
    //     fprintf(stderr,"Setting element %d to %d\n",i,val);
    //
    //     plc_tag_set_int32(tag,(i*ELEM_SIZE),val);
    // }
    //
    // rc = plc_tag_write(tag, DATA_TIMEOUT);
    //
    // if(rc != PLCTAG_STATUS_OK) {
    //     fprintf(stderr,"ERROR: Unable to read the data! Got error code %d: %s\n",rc, plc_tag_decode_error(rc));
    //     return 0;
    // }
    //
    //
    // /* get the data again*/
    // rc = plc_tag_read(tag, DATA_TIMEOUT);
    //
    // if(rc != PLCTAG_STATUS_OK) {
    //     fprintf(stderr,"ERROR: Unable to read the data! Got error code %d: %s\n",rc, plc_tag_decode_error(rc));
    //     return 0;
    // }
    //
    // /* print out the data */
    // for(i=0; i < ELEM_COUNT; i++) {
    //     fprintf(stderr,"data[%d]=%d\n",i,plc_tag_get_int32(tag,(i*ELEM_SIZE)));
    // }
    
    
    return 0;
}

