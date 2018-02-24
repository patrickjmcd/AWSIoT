
#include "testPJM.hpp"

bool verbose = false;
bool print = false;


const char* fluidLevelPath = "protocol=ab_eip&gateway=10.20.4.36&path=1,0&cpu=LGX&elem_size=4&elem_count=1&name=val_FluidLevel";
const char* intakePressurePath = "protocol=ab_eip&gateway=10.20.4.36&path=1,0&cpu=LGX&elem_size=4&elem_count=1&name=val_IntakePressure";
const char* intakeTemperaturePath = "protocol=ab_eip&gateway=10.20.4.36&path=1,0&cpu=LGX&elem_size=4&elem_count=1&name=val_IntakeTemperature";
const char* energyCostPath = "protocol=ab_eip&gateway=10.20.4.36&path=1,0&cpu=LGX&elem_size=4&elem_count=30&name=Energy_Cost";

// Convert int Number to String
string intToStr (int vnumber)
{
    ostringstream convert;
    convert << vnumber;
    return convert.str();
};

string getCpuPath(plc_struct &plc)
{
    return "protocol=" + plc.protocol + "&gateway=" + plc.ip + "&path=" + intToStr(plc.portType) + "," + intToStr(plc.slot) + "&cpu=" + plc.cpu;
};

int getTypeSize(int type)
{
    int result;
    switch (type)
    {
        case 1: // for BOOL
            result = 1;
            break;
        case 2: // for SINT
            result = 1;
            break;
        case 3: // for INT
            result = 2;
            break;
        case 4: // for DINT
            result = 4;
            break;
        case 5: // for REAL
            result = 4;
            break;
        default:
            result = 0;
    }; // switch end
    return result;
};

string getTagPath(string cpuPath, const plc_tag_struct &tag)
{
    string str = cpuPath + "&elem_count=" + intToStr(tag.elemCount) + "&elem_size=" + intToStr(getTypeSize(tag.tagType)) + "&name=" + tag.tagName;
    if (verbose)
        str += "&debug=1"; // add the debug parameters if debug is active. The debug is set using SetDebug
    return str;
};

float readFloatTag(plc_tag_struct tag){
    float val_float;
    int rc;
    /* everything OK? */
    if(!tag.tagPtr) {
        fprintf(stderr,"ERROR: Could not create tag!\n");
    }
    
    /* let the connect succeed we hope */
    while(plc_tag_status(tag.tagPtr) == PLCTAG_STATUS_PENDING) {
        sleep_ms(100);
    }
    
    if(plc_tag_status(tag.tagPtr) != PLCTAG_STATUS_OK) {
        fprintf(stderr,"Error setting up tag internal state. Error %s\n", plc_tag_decode_error(plc_tag_status(tag.tagPtr)));
    }
    
    /* get the data */
    rc = plc_tag_read(tag.tagPtr, DATA_TIMEOUT);
    
    if(rc != PLCTAG_STATUS_OK) {
        fprintf(stderr,"ERROR: Unable to read the data! Got error code %d: %s\n",rc, plc_tag_decode_error(rc));
    }
    val_float = plc_tag_get_float32(tag.tagPtr, 0);
    if (verbose)fprintf(stderr, "data=%f\n", val_float);
    return val_float;
}

float_vec_t readFloatTagArray(plc_tag_struct tag, int numToRead){
    int rc;
    int i;
    float_vec_t results;
    float val_float;
    results.reserve(numToRead);
    
    /* everything OK? */
    if(!tag.tagPtr) {
        fprintf(stderr,"ERROR: Could not create tag!\n");
        // return 0;
    }
    
    /* let the connect succeed we hope */
    while(plc_tag_status(tag.tagPtr) == PLCTAG_STATUS_PENDING) {
        sleep_ms(100);
    }
    
    if(plc_tag_status(tag.tagPtr) != PLCTAG_STATUS_OK) {
        fprintf(stderr,"Error setting up tag internal state. Error %s\n", plc_tag_decode_error(plc_tag_status(tag.tagPtr)));
        // return 0;
    }
    
    /* get the data */
    rc = plc_tag_read(tag.tagPtr, DATA_TIMEOUT);
    if(rc != PLCTAG_STATUS_OK) {
        fprintf(stderr,"ERROR: Unable to read the data! Got error code %d: %s\n",rc, plc_tag_decode_error(rc));
        // return 0;
    }
    /* print out the data */
    for(i=0; i < numToRead; i++) {
        val_float = plc_tag_get_float32(tag.tagPtr,(i*getTypeSize(tag.tagType)));
        if (verbose) fprintf(stderr,"data[%d]=%f\n",i, val_float);
        results.push_back(val_float);
    }
    return results;
}

int main()
{
    plc_tag fluidLevelTag = PLC_TAG_NULL;
    plc_tag intakePressureTag = PLC_TAG_NULL;
    plc_tag intakeTemperatureTag = PLC_TAG_NULL;
    plc_tag energyCostTag = PLC_TAG_NULL;
    
    
    
    
    /* create the tag */
    fluidLevelTag = plc_tag_create(fluidLevelPath);
    intakePressureTag = plc_tag_create(intakePressurePath);
    intakeTemperatureTag = plc_tag_create(intakeTemperaturePath);
    energyCostTag = plc_tag_create(energyCostPath);
    
    float fluidLevel = readFloatTag(fluidLevelTag);
    float intakePressure = readFloatTag(intakePressureTag);
    float intakeTemperature = readFloatTag(intakeTemperatureTag);
    float_vec_t energyCost = readFloatTagArray(energyCostTag, 30);
    
    printf("--\n");
    printf("Fluid Level = %f\n--\n", fluidLevel);
    printf("Intake Temperature = %f\n--\n", intakeTemperature);
    printf("Intake Pressure = %f\n--\n", intakePressure);
    for(int i = 0; i < energyCost.size(); i++){
        printf("Energy Cost [%d] = %f\n", i, energyCost[i]);
    }
    printf("--\n");
    
    
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
    
    /* we are done */
    plc_tag_destroy(fluidLevelTag);
    plc_tag_destroy(intakePressureTag);
    plc_tag_destroy(intakeTemperatureTag);
    plc_tag_destroy(energyCostTag);
    
    return 0;
}


// TAGS
float val_FluidLevel, val_IntakePressure, val_IntakeTemperature;
float_vec_t energyCost;


// Read Functions

float readLGXFloat(Eip_Session *session, Eip_Connection *connection, char *varName){
	int i;
	if (verbose == true) printf("Reading %s\n",varName);
	float val_float = 1.234;
	LGX_Read *data=ReadLgxData(session, connection, varName, 1);
	if (data!=NULL)
	{
		if (data->Varcount>0)
		{
			if (verbose == true){
				printf("ReadLgxData Ok :\n");
				printf("\telements :%d\n\tDatatype : %d\n\ttotalsize : %d\n\tElement size : %d\n",data->Varcount,data->type,data->totalsize,data->elementsize,data->mask);
			}
			val_float = _GetLGXValueAsFloat(data, 0);
			if (verbose == true) printf("%s = %f\n", varName, val_float);

		} else printf("Error ReadLgxData : %s\n",cip_err_msg);
		free(data); // You have to free the data return by ReadLgxData
	} else {
		printf("Error : ReadLgxData %s (%d:%d)\n",cip_err_msg,cip_errno,cip_ext_errno);
	}
	return val_float;
}


float_vec_t readLGXFloatArray(Eip_Session *session, Eip_Connection *connection, char *varName, int numToRead){
	int i;
	float_vec_t results;
	results.reserve(numToRead);
	if (verbose == true) printf("Reading %s\n",varName);
	float val_float;
	LGX_Read *data=ReadLgxData(session, connection, varName, numToRead);
	if (data!=NULL)
	{
		if (data->Varcount>0)
		{
			if (verbose == true){
				printf("ReadLgxData Ok :\n");
				printf("\telements :%d\n\tDatatype : %d\n\ttotalsize : %d\n\tElement size : %d\n",data->Varcount,data->type,data->totalsize,data->elementsize,data->mask);
			}
			for (i=0;i<data->Varcount;i++)
			{
				val_float=_GetLGXValueAsFloat(data,i);
				results.push_back(val_float);
				if (verbose == true) printf("%s [%d] = %f\n",varName,i,val_float);
			}
		} else printf("Error ReadLgxData : %s\n",cip_err_msg);
		free(data); // You have to free the data return by ReadLgxData
	} else
	{
		printf("Error : ReadLgxData %s (%d:%d)\n",cip_err_msg,cip_errno,cip_ext_errno);
	}
	return results;
}


int readLGXInt(Eip_Session *session, Eip_Connection *connection, char *varName){
	int i, val_int;
	if (verbose == true) printf("Reading %s\n", varName);
	LGX_Read *data = ReadLgxData(session,connection, varName, 1);
	if (data!=NULL)
	{
		if (data->Varcount>0)
		{
			if (verbose == true){
				printf("ReadLgxData Ok :\n");
				printf("\telements :%d\n\tDatatype : %d\n\ttotalsize : %d\n\tElement size : %d\n",data->Varcount,data->type,data->totalsize,data->elementsize,data->mask);
			}
			val_int = _GetLGXValueAsInteger(data,0);
			if (verbose == true) printf("%s = %d\n", varName, val_int);
		} else printf("Error ReadLgxData : %s\n",cip_err_msg);
		free(data); // You have to free the data return by ReadLgxData
	} else
	{
		printf("Error : ReadLgxData %s (%d:%d)\n",cip_err_msg,cip_errno,cip_ext_errno);
	}
	return val_int;
}



int_vec_t readLGXIntArray(Eip_Session *session, Eip_Connection *connection, char *varName, int numToRead){
	int i;
	int_vec_t results;
	results.reserve(numToRead);
	if (verbose == true) printf("Reading %s\n",varName);
	int val_int;
	LGX_Read *data=ReadLgxData(session, connection, varName, numToRead);
	if (data!=NULL)
	{
		if (data->Varcount>0)
		{
			if (verbose == true){
				printf("ReadLgxData Ok :\n");
				printf("\telements :%d\n\tDatatype : %d\n\ttotalsize : %d\n\tElement size : %d\n",data->Varcount,data->type,data->totalsize,data->elementsize,data->mask);
			}
			for (i=0;i<data->Varcount;i++)
			{
				val_int =_GetLGXValueAsInteger(data,i);
				results.push_back(val_int);
				if (verbose == true) printf("%s [%d] = %d\n", varName, i, val_int);
			}
		} else printf("Error ReadLgxData : %s\n", cip_err_msg);
		free(data); // You have to free the data return by ReadLgxData
	} else
	{
		printf("Error : ReadLgxData %s (%d:%d)\n", cip_err_msg, cip_errno, cip_ext_errno);
	}
	return results;
}


int main(int argc,char *argv[])
{
	//cip_debuglevel=LogDebug; // You can uncomment this line to see the data exchange between TuxEip and your Logic controller
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

	char *IP="10.20.4.36\0";
	BYTE path[]={1,0};

	int res;

	if (verbose == true) printf("Starting, Pid = %d\n",getpid());

	/* Opening a session */
	if (verbose == true) printf("entering OpenSession \n");

	Eip_Session *session=OpenSession(IP);
	if (session==NULL)
	{
		printf("Error : OpenSession %s (%d:%d)\n",cip_err_msg,cip_errno,cip_ext_errno);
		return(1);
	}
	if (verbose == true) {
		printf("OpenSession Ok\n");
		printf("entering RegisterSession \n");
	}
	res=RegisterSession(session);

	if (res!=E_Error)
	{
		if (verbose == true) {
			printf("RegisterSession Ok\n");
			printf("entering ConnectPLCOverCNET\n");
		}
		Eip_Connection *connection=ConnectPLCOverCNET(
			session, // session whe have open
			LGX, // plc type
			0x12345678, // Target To Originator connection ID
			0x6789, // Connection Serial Number
			5000, // Request Packet Interval
			path, // Path to the ControlLogix
			sizeof(path) // path size
		);

		if (connection!=NULL)
		{
			if (verbose == true) printf("ConnectPLCOverCNET Ok, \n");

			// float val_FluidLevel = readLGXFloat(session, connection, "val_FluidLevel");
			// float val_IntakeTemperature = readLGXFloat(session, connection, "val_IntakeTemperature");
			// float val_IntakePressure = readLGXFloat(session, connection, "val_IntakePressure");
			// float_vec_t energyCost = readLGXFloatArray(session, connection, "Energy_Cost", 30);

			val_FluidLevel = readLGXFloat(session, connection, "val_FluidLevel");
			val_IntakeTemperature = readLGXFloat(session, connection, "val_IntakeTemperature");
			val_IntakePressure = readLGXFloat(session, connection, "val_IntakePressure");
			energyCost = readLGXFloatArray(session, connection, "Energy_Cost", 30);


			if (verbose == true) printf("entering Forward_Close\n");

			res=Forward_Close(connection);

			if (res!=E_Error)	{
				if (verbose == true) printf("Forward_Close %s\n",cip_err_msg);
			} else {
				printf("Error : Forward_Close %s (%d:%d)\n",cip_err_msg,cip_errno,cip_ext_errno);
			}
		} else
		{
			printf("Error : ConnectPLCOverCNET %s (%d:%d)\n",cip_err_msg,cip_errno,cip_ext_errno);
		}
		UnRegisterSession(session);
		if (verbose == true) printf("UnRegister : %s\n",cip_err_msg);

	}else
	{
		printf("Error : RegisterSession %s (%d:%d)\n",cip_err_msg,cip_errno,cip_ext_errno);
	}

	if (verbose == true) printf("entering CloseSession\n");
	CloseSession(session);


	// THIS IS WHERE PROCESSING SHOULD GO
	if (print == true){
		printf("--\n");
		printf("Fluid Level = %f\n--\n", val_FluidLevel);
		printf("Intake Temperature = %f\n--\n", val_IntakeTemperature);
		printf("Intake Pressure = %f\n--\n", val_IntakePressure);
		for(int i = 0; i < energyCost.size(); i++){
			printf("Energy Cost [%d] = %f\n", i, energyCost[i]);
		}
		printf("--\n");
	}


	return(0);
}
