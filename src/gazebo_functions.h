/*
* File: gazebo_functions.h
* Author: iskinmike
*
*/
#ifndef _STRING_
#include <string>
#endif

int createWorld(double x, double y, double z);
void destroyWorld();
int createModel(std::string model_name, 
				double pos_x, double pos_y, double pos_z,
				double orient_x, double orient_y, double orient_z,
				double orient_w, double is_static,
				std::string path);
void changeColor(int object_id, std::string color);
void changeStatus(int object_id, int hold);
double getX(std::string model_name);
double getY(std::string model_name);
double getZ(std::string model_name);
double getAngle(std::string model_name);
void testPtintFunction();
void initRosSystem();

int moveModel( std::string input1,
			   double x, double y, double z, double _speed,
			   double _angle, double _angle_speed );

class ModelPosition;