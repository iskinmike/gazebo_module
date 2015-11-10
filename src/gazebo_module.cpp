/*
* File: gazebo_module.cpp
* Author: m79lol, iskinmike
*
*/

#include <stdio.h>

#include "module.h"
#include "function_module.h"

#include "gazebo_module.h"
#include "gazebo_functions.h"

// GLOBAL DATA
#define UID "Gazebo_function_module_v107"
#define COUNT_GAZEBO_FUNCTIONS 9
#define BUILD_NUMBER 1
//////

#define ADD_GAZEBO_1S8F1S_FUNCTION(FUNCTION_NAME) \
Params = new FunctionData::ParamTypes[10]; \
Params[0] = FunctionData::STRING; \
Params[1] = FunctionData::FLOAT; \
Params[2] = FunctionData::FLOAT; \
Params[3] = FunctionData::FLOAT; \
Params[4] = FunctionData::FLOAT; \
Params[5] = FunctionData::FLOAT; \
Params[6] = FunctionData::FLOAT; \
Params[7] = FunctionData::FLOAT; \
Params[8] = FunctionData::FLOAT; \
Params[9] = FunctionData::STRING; \
gazebo_functions[function_id] = new FunctionData(function_id + 1,10, Params, FUNCTION_NAME); \
function_id++;
//////

#define ADD_GAZEBO_3F_FUNCTION(FUNCTION_NAME) \
Params = new FunctionData::ParamTypes[3]; \
Params[0] = FunctionData::FLOAT; \
Params[1] = FunctionData::FLOAT; \
Params[2] = FunctionData::FLOAT; \
gazebo_functions[function_id] = new FunctionData(function_id + 1,3, Params, FUNCTION_NAME); \
function_id++;
//////

#define ADD_GAZEBO_1S6F_FUNCTION(FUNCTION_NAME) \
Params = new FunctionData::ParamTypes[7]; \
Params[0] = FunctionData::STRING; \
Params[1] = FunctionData::FLOAT;  \
Params[2] = FunctionData::FLOAT;  \
Params[3] = FunctionData::FLOAT;  \
Params[4] = FunctionData::FLOAT;  \
Params[5] = FunctionData::FLOAT;  \
Params[6] = FunctionData::FLOAT;  \
gazebo_functions[function_id] = new FunctionData(function_id + 1,7, Params, FUNCTION_NAME); \
function_id++;
//////

#define ADD_GAZEBO_0_FUNCTION(FUNCTION_NAME) \
gazebo_functions[function_id] = new FunctionData(function_id + 1, 0, NULL, FUNCTION_NAME); \
function_id++;
//////

#define ADD_GAZEBO_1S1F_FUNCTION(FUNCTION_NAME) \
Params = new FunctionData::ParamTypes[2]; \
Params[0] = FunctionData::STRING; \
Params[1] = FunctionData::FLOAT; \
gazebo_functions[function_id] = new FunctionData(function_id + 1, 2, Params, FUNCTION_NAME); \
function_id++;
//////

#define ADD_GAZEBO_1S_FUNCTION(FUNCTION_NAME) \
Params = new FunctionData::ParamTypes[1]; \
Params[0] = FunctionData::STRING; \
gazebo_functions[function_id] = new FunctionData(function_id + 1, 1, Params, FUNCTION_NAME); \
function_id++;
//////

#define DEFINE_ALL_FUNCTIONS 				\
ADD_GAZEBO_0_FUNCTION("testFunction"); 		\
ADD_GAZEBO_1S8F1S_FUNCTION("createModel");  \
ADD_GAZEBO_1S_FUNCTION("getX"); 			\
ADD_GAZEBO_1S_FUNCTION("getY"); 			\
ADD_GAZEBO_1S_FUNCTION("getZ"); 			\
ADD_GAZEBO_1S_FUNCTION("getAngle"); 		\
ADD_GAZEBO_3F_FUNCTION("createWorld"); 		\
ADD_GAZEBO_0_FUNCTION("destroyWorld"); 		\
ADD_GAZEBO_1S6F_FUNCTION("moveModel");
//////


GazeboFunctionModule::GazeboFunctionModule() {
/*
  mi = new ModuleInfo;
  mi->uid = UID;
  mi->mode = ModuleInfo::Modes::PROD;
  mi->version = BUILD_NUMBER;
  mi->digest = NULL;
*/
	gazebo_functions = new FunctionData*[COUNT_GAZEBO_FUNCTIONS];
	system_value function_id = 0;

	FunctionData::ParamTypes *Params;
	function_id=0;

	DEFINE_ALL_FUNCTIONS
	// init ROS Node for module
	initRosSystem();
};

FunctionResult *GazeboFunctionModule::executeFunction(system_value function_index,
                                                   void **args) {
  if ((function_index < 1) || (function_index > (int)COUNT_GAZEBO_FUNCTIONS)) {
    return NULL;
  }

  try {
    variable_value rez = 0;
    switch (function_index) {
      case 1: {
      	printf("%s\n", "test_function_called" );
      	testPtintFunction();
        break;
      }
      case 2: {
      	//printf("%s\n", "createModel_called" );
      	std::string input1((const char *)args[0]);
        variable_value *input2 = (variable_value *)args[1];
        variable_value *input3 = (variable_value *)args[2];
        variable_value *input4 = (variable_value *)args[3];
        variable_value *input5 = (variable_value *)args[4];
        variable_value *input6 = (variable_value *)args[5];
        variable_value *input7 = (variable_value *)args[6];
        variable_value *input8 = (variable_value *)args[7];
        variable_value *input9 = (variable_value *)args[8];
        std::string input10((const char *)args[9]);
        //printf("%s\n", "createModel_called" );
        rez = createModel(          input1, (double)*input2, (double)*input3,
            			   (double)*input4, (double)*input5, (double)*input6,
            			   (double)*input7, (double)*input8, (double)*input9,
            			    input10);
        //printf("%s\n", "createModel_called" );
      	if (rez) {
      		throw std::exception();
      	}
        break;
      }
      case 3:{ 
      	std::string input1((const char *)args[0]);
      	rez = getX(input1);
      	break;
      }
      case 4:{ 
		std::string input1((const char *)args[0]);
      	rez = getY(input1);
      	break;
      }
      case 5:{ 
		std::string input1((const char *)args[0]);
      	rez = getZ(input1);
      	break;
      }
      case 6:{ 
		std::string input1((const char *)args[0]);
      	rez = getAngle(input1);
      	break;
      }
      case 7: {
      	variable_value *input1 = (variable_value *)args[0];
      	variable_value *input2 = (variable_value *)args[1];
        variable_value *input3 = (variable_value *)args[2];
        rez = createWorld((double) *input1, (double) *input2, (double) *input3);
      	if (rez) {
      		throw std::exception();
      	}
        break;
      }
      case 8: {
      	printf("%s\n", "destroyWorld_called" );
      	destroyWorld();
        break;
      }
      case 9: {
      	std::string input1((const char *)args[0]); // name
        variable_value *input2 = (variable_value *)args[1]; // x
        variable_value *input3 = (variable_value *)args[2]; // y
        variable_value *input4 = (variable_value *)args[3]; // z
        variable_value *input5 = (variable_value *)args[4]; // speed
        variable_value *input6 = (variable_value *)args[5]; // angle
        variable_value *input7 = (variable_value *)args[6]; // angle speed
        rez = moveModel(  input1,
        				   (double)*input2, (double)*input3,
            			   (double)*input4, (double)*input5, 
            			   (double)*input6, (double)*input7);
      	//if (rez) {
      	//	throw std::exception();
      	//}
        break;
      }
    };
    return new FunctionResult(1, rez); //return new FunctionResult(FunctionResult::Types::VALUE, rez);
  } catch (...) {
    return new FunctionResult(0); //return new FunctionResult(FunctionResult::Types::EXCEPTION);
  };
};

void GazeboFunctionModule::prepare(colorPrintfModule_t *colorPrintf_p,
               					colorPrintfModuleVA_t *colorPrintfVA_p){
	//colorPrintf = colorPrintf_p;
	this->colorPrintf_p = colorPrintfVA_p;
}
FunctionData** GazeboFunctionModule::getFunctions(unsigned int *count_functions) {
	*count_functions = COUNT_GAZEBO_FUNCTIONS;
	return gazebo_functions;
};


const char *GazeboFunctionModule::getUID() { return "gazebo_module"; };
/*
const struct ModuleInfo &GazeboFunctionModule::getModuleInfo() { 
	return *mi; 
}
*/
void *GazeboFunctionModule::writePC(unsigned int *buffer_length) {
	*buffer_length = 0;
	return NULL;
}
void GazeboFunctionModule::readPC(void *buffer, unsigned int buffer_length) {
}
int GazeboFunctionModule::startProgram(int uniq_index) {
	return 0;
}
int GazeboFunctionModule::endProgram(int uniq_index) {
	return 0;
}
void GazeboFunctionModule::destroy() {
	for (unsigned int j = 0; j < COUNT_GAZEBO_FUNCTIONS; ++j) {
		if (gazebo_functions[j]->count_params) {
			delete[] gazebo_functions[j]->params;
		}
		delete gazebo_functions[j];
	}
	delete[] gazebo_functions;
	delete this;
};

PREFIX_FUNC_DLL FunctionModule* getFunctionModuleObject() {
	return new GazeboFunctionModule();
};

