
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
    
    Tag fluidLevelTag = Tag("val_FluidLevel", CIP_DATA_TYPE_REAL, 1, maxWaterSystemPlc.getCpuPath(), 5.0, 600, false, verbose);
    Tag intakePressureTag = Tag("val_IntakePressure", CIP_DATA_TYPE_REAL, 1, maxWaterSystemPlc.getCpuPath(), 5.0, 600, false, verbose);
    Tag intakeTemperatureTag = Tag("val_IntakeTemperature", CIP_DATA_TYPE_REAL, 1, maxWaterSystemPlc.getCpuPath(), 5.0, 600, false, verbose);
    Tag alarmDowntimeSecTag = Tag("cfg_AlarmDowntimeSec", CIP_DATA_TYPE_DINT, 1, maxWaterSystemPlc.getCpuPath(), 5.0, 600, false, verbose);
    Tag rpFlowmeterTag = Tag("rp_Flowmeter", CIP_DATA_TYPE_BOOL, 1, maxWaterSystemPlc.getCpuPath(), 5.0, 600, false, verbose);
    Tag energyCostTag = Tag("Energy_Cost", CIP_DATA_TYPE_REAL, 30, maxWaterSystemPlc.getCpuPath(), 5.0, 600, false, verbose);
    Tag downholeStatusTag = Tag("Downhole_Sensor_Status", CIP_DATA_TYPE_STRING, 1, maxWaterSystemPlc.getCpuPath(), 0.0, 600, false, verbose);
    Tag distanceToIntakeTag = Tag("cfg_DHSensorDistToIntake", CIP_DATA_TYPE_REAL, 1, maxWaterSystemPlc.getCpuPath(), 0.1, 600, true, verbose);
    
    Tag flowTotalizerBTag = Tag("val_FlowTotalizerB", CIP_DATA_TYPE_REAL, 1, pioneerFracSkidPlc.getCpuPath(), 5.0, 600, false, verbose);
    
    float fluidLevel, intakePressure, intakeTemperature, distanceToIntake;
    int alarmDowntimeSec, rpFlowmeter;
    float_vec_t energyCost;
    string downholeStatus;
    float flowTotalizerB;
    
    int loops = 0;
    while (loops < 5){
        if (fluidLevelTag.checkForSend()) fluidLevelTag.send();
        if (intakePressureTag.checkForSend()) intakePressureTag.send();
        if (intakeTemperatureTag.checkForSend()) intakeTemperatureTag.send();
        if (alarmDowntimeSecTag.checkForSend()) alarmDowntimeSecTag.send();
        if (rpFlowmeterTag.checkForSend()) rpFlowmeterTag.send();
        if (energyCostTag.checkForSend()) energyCostTag.send();
        if (downholeStatusTag.checkForSend()) downholeStatusTag.send();
        if (distanceToIntakeTag.checkForSend()) distanceToIntakeTag.send();
        
        if (flowTotalizerBTag.checkForSend()) flowTotalizerBTag.send();
        
        fluidLevel = fluidLevelTag.getFloatValue();
        intakePressure = intakePressureTag.getFloatValue();
        intakeTemperature = intakeTemperatureTag.getFloatValue();
        
        alarmDowntimeSec = alarmDowntimeSecTag.getIntValue();
        rpFlowmeter = rpFlowmeterTag.getIntValue();
        downholeStatus = downholeStatusTag.getStringValue();
        distanceToIntake = distanceToIntakeTag.getFloatValue();
        
        energyCost = energyCostTag.getFloatArrayValue();
        
        flowTotalizerB = flowTotalizerBTag.getFloatValue();
        
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
            printf("DH Status = %s\n--\n", downholeStatus.c_str());
            printf("Distance to Intake = %f\n--\n", distanceToIntake);
            printf("--\nPIONEER FRAC SKID\n--\n");
            printf("Flow Totalizer B = %f\n--\n", flowTotalizerB);
            printf("--\n");
        }
        
        srand (static_cast <unsigned> (time(0)));
        float randFloat = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/100.0));
//        alarmDowntimeSecTag.write(600);
        distanceToIntakeTag.write(randFloat);
        energyCostTag.writeArrayMember((float (100.0 + loops)), 20);
        
        loops++;
        
    }
    
    return 0;
}

