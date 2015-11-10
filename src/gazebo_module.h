/*
* File: gazebo_module.h
* Author: m79lol, iskinmike
*
*/
#ifndef GAZEBO_MODULE_H
#define GAZEBO_MODULE_H

class GazeboFunctionModule : public FunctionModule {
	FunctionData **gazebo_functions;
	
public:
	//ModuleInfo *mi;
	colorPrintfModuleVA_t *colorPrintf_p;
	//colorPrintf_t *colorPrintf;
	GazeboFunctionModule();

	//init
	const char *getUID();
	//const struct ModuleInfo& getModuleInfo();
	void prepare(colorPrintfModule_t *colorPrintf_p,
               colorPrintfModuleVA_t *colorPrintfVA_p);

	//compiler only
	FunctionData **getFunctions(unsigned int *count_functions);
	void *writePC(unsigned int *buffer_length);

	//intepreter - program & lib
	void readPC(void *buffer, unsigned int buffer_length);

	//intepreter - program
	FunctionResult* executeFunction(system_value function_index, void **args);
	int startProgram(int uniq_index);
	int endProgram(int uniq_index);

	//destructor
	void destroy();
	~GazeboFunctionModule() {};
};






#endif /* GAZEBO_MODULE_H */