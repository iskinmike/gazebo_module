/*
* File: gazebo_functions.cpp
* Author: iskinmike
*
*/
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

#include "ros_headers.h"
#include "gazebo_headers.h"
#include "gazebo_functions.h"


// MACROS
#define STATIC_POSITION_OFFSET 8

#define CHECK_OTHER_MODELS_POSITIONS 																				\
	for (auto i = gl_map_of_models_positions.begin(); i != gl_map_of_models_positions.end(); ++i)					\
	{ 																												\
		ros::NodeHandle node_to_get_model_data; 																	\
	  	ros::ServiceClient service_to_get_model_data = 																\
	  	  node_to_get_model_data.serviceClient<gazebo_msgs::GetModelState>("/gazebo/get_model_state"); 				\
	  	gazebo_msgs::GetModelState etalon_model_data;																\
	  	etalon_model_data.request.model_name = i->first;		 													\
		if (service_to_get_model_data.call(etalon_model_data)){ 													\
			ModelPosition *actual_model_position  																	\
  				= new ModelPosition(model_name, etalon_model_data.response.pose, etalon_model_data.response.twist); \
  			if (!actual_model_position->isEqualToEtalon(i->second)){ 												\
  				actual_model_position->printActualData(); 															\
  				i->second->printActualData();																		\
  				delete actual_model_position; 																		\
  				return 5; 																							\
 			} 																										\
		} 																											\
	};

#define TELEPORTATION(_x,_y,_z,_angle) 									\
	ros::NodeHandle node_to_set_model_coords; 							\
  	ros::ServiceClient service_to_move_model 							\
  		= node_to_set_model_coords.serviceClient<gazebo_msgs::SetModelState>("/gazebo/set_model_state"); \
  	gazebo_msgs::SetModelState teleport_model_data; 					\
  																		\
  	teleport_model_data.request.model_state.model_name = model_name; 	\
  	teleport_model_data.request.model_state.twist = zero_twist; 		\
  																		\
  	geometry_msgs::Pose model_position;  								\
	model_position.position.x = _x; 									\
	model_position.position.y = _y; 									\
	model_position.position.z = _z; 									\
																		\
	model_position.orientation.x = 0; 									\
	model_position.orientation.y = 0; 									\
	model_position.orientation.z = 0; 									\
	model_position.orientation.w = _angle; 								\
																		\
  	teleport_model_data.request.model_state.pose = model_position; 		\
  	if (!service_to_move_model.call(teleport_model_data)){ 				\
  		return 1; 														\
  	}

double formatDoubleToFourNumbers(double _data){
		char buffer[10];
		sprintf(buffer,"%0.2f", _data);
		return atof(buffer);
	}

class ModelPosition{
public:
	geometry_msgs::Pose pose;
	geometry_msgs::Twist twist;
	std::string model_name;

	double formatDoubleNumber(double _data){
		char buffer[10];
		sprintf(buffer,"%0.2f", _data);
		return atof(buffer);
	}

	#define COMPARE_TWO_NUMERIC_THINGS(first,second) \
		if (first != second) { \
			printf("%f %f\n",first, second ); \
			return false; }

	#define WRITE_FORMAT_DATA 										  \
		pose.position.x    = formatDoubleNumber(_pose.position.x);    \
		pose.position.y    = formatDoubleNumber(_pose.position.y);    \
		pose.position.z    = formatDoubleNumber(_pose.position.z);    \
		pose.orientation.x = formatDoubleNumber(_pose.orientation.x); \
		pose.orientation.y = formatDoubleNumber(_pose.orientation.y); \
		pose.orientation.z = formatDoubleNumber(_pose.orientation.z); \
		pose.orientation.w = formatDoubleNumber(_pose.orientation.w); \
		twist.linear.x     = formatDoubleNumber(_twist.linear.x);     \
		twist.linear.y     = formatDoubleNumber(_twist.linear.y);     \
		twist.linear.z     = formatDoubleNumber(_twist.linear.z);     \
		twist.angular.x    = formatDoubleNumber(_twist.angular.x);    \
		twist.angular.y    = formatDoubleNumber(_twist.angular.y);    \
		twist.angular.z    = formatDoubleNumber(_twist.angular.z);    	

	ModelPosition(std::string _name, geometry_msgs::Pose _pose, geometry_msgs::Twist _twist){
		model_name = _name;
		WRITE_FORMAT_DATA;
	};
	ModelPosition(){};
	~ModelPosition(){};
	// Сравнение с предыдущим значением
	// Вопрос - как правильно это организовать?
	bool isEqualToEtalon(ModelPosition *_etalon){
		//if (model_name.compare(_etalon->model_name) !=0) {return false;}

		COMPARE_TWO_NUMERIC_THINGS(_etalon->pose.position.x, pose.position.x);
		COMPARE_TWO_NUMERIC_THINGS(_etalon->pose.position.y, pose.position.y);
		COMPARE_TWO_NUMERIC_THINGS(_etalon->pose.position.z, pose.position.z);

		COMPARE_TWO_NUMERIC_THINGS(_etalon->pose.orientation.x, pose.orientation.x);
		COMPARE_TWO_NUMERIC_THINGS(_etalon->pose.orientation.y, pose.orientation.y);
		COMPARE_TWO_NUMERIC_THINGS(_etalon->pose.orientation.z, pose.orientation.z);
		COMPARE_TWO_NUMERIC_THINGS(_etalon->pose.orientation.w, pose.orientation.w);
		
		//COMPARE_TWO_NUMERIC_THINGS(_etalon->twist.linear.x, twist.linear.x);
		//COMPARE_TWO_NUMERIC_THINGS(_etalon->twist.linear.y, twist.linear.y);
		//COMPARE_TWO_NUMERIC_THINGS(_etalon->twist.linear.z, twist.linear.z);

		//COMPARE_TWO_NUMERIC_THINGS(_etalon->twist.angular.x, twist.angular.x);
		//COMPARE_TWO_NUMERIC_THINGS(_etalon->twist.angular.y, twist.angular.y);
		//COMPARE_TWO_NUMERIC_THINGS(_etalon->twist.angular.z, twist.angular.z);

		return true;
	};
	void assignNewData(ModelPosition *new_model){
		model_name = new_model->model_name;
		pose       = new_model->pose;
		twist      = new_model->twist;	
	};
	void assignNewData(std::string _name, geometry_msgs::Pose _pose, geometry_msgs::Twist _twist){
		model_name = _name;
		WRITE_FORMAT_DATA;	
	};
	void printData(){
		printf("%s%s\n","model:", model_name.c_str() );
		printf("%s\n","pose:" );
		printf("%s\n","  position: " );
		printf("    x = %0.2f \n ",pose.position.x);
		printf("    y = %0.2f \n ",pose.position.y);
		printf("    z = %0.2f \n ",pose.position.z);
		printf("%s\n","  orientation: " );
		printf("    x = %0.2f \n ",pose.orientation.x);
		printf("    y = %0.2f \n ",pose.orientation.y);
		printf("    z = %0.2f \n ",pose.orientation.z);
		printf("    w = %0.2f \n ",pose.orientation.w);
		printf("%s\n","twist" );
		printf("%s\n","  linear: " );
		printf("    x = %0.2f \n ",twist.linear.x);
		printf("    y = %0.2f \n ",twist.linear.y);
		printf("    z = %0.2f \n ",twist.linear.z);
		printf("%s\n","  angular: " );
		printf("    x = %0.2f \n ",twist.angular.x);
		printf("    y = %0.2f \n ",twist.angular.y);
		printf("    z = %0.2f \n ",twist.angular.z);
	};
	void printActualData(){
		ros::NodeHandle node_to_get_model_data; 																	\
	  	ros::ServiceClient service_to_get_model_data = 																\
	  	  node_to_get_model_data.serviceClient<gazebo_msgs::GetModelState>("/gazebo/get_model_state"); 				\
	  	gazebo_msgs::GetModelState etalon_model_data;																\
	  	etalon_model_data.request.model_name = model_name;
	  	if (service_to_get_model_data.call(etalon_model_data)){ 
	  		printf("%s%s\n","model:", model_name.c_str() );
			printf("%s\n","pose:" );
			printf("%s\n","  position: " );
			printf("    x = %0.2f \n ",etalon_model_data.response.pose.position.x);
			printf("    y = %0.2f \n ",etalon_model_data.response.pose.position.y);
			printf("    z = %0.2f \n ",etalon_model_data.response.pose.position.z);
			printf("%s\n","  orientation: " );
			printf("    x = %0.2f \n ",etalon_model_data.response.pose.orientation.x);
			printf("    y = %0.2f \n ",etalon_model_data.response.pose.orientation.y);
			printf("    z = %0.2f \n ",etalon_model_data.response.pose.orientation.z);
			printf("    w = %0.2f \n ",etalon_model_data.response.pose.orientation.w);
	  	}
	  	else {}

	}
};

// GLOBAL VARIABLES
std::map<std::string, ModelPosition*> gl_map_of_models_positions;
#define WALL_WIDTH 2

int createWall(std::string wall_name, double dx, double dy, double dz, double x, double y, double z){
	// fill SDF

	std::string sdf_wall("<sdf version=\"1.5\">");
{
	sdf_wall.append("<model name=\"");
	sdf_wall.append(wall_name);
	sdf_wall.append("\">");
	sdf_wall.append("  <pose>0 0 0 0 0 0</pose>");
	sdf_wall.append("  <static>false</static>");
	sdf_wall.append("    <link name=\"");
	sdf_wall.append(wall_name);
	sdf_wall.append("\">");
	sdf_wall.append("      <collision name=\"collision\">");
	sdf_wall.append("        <geometry>");
	sdf_wall.append("          <box>");
	sdf_wall.append("            <size>");
	sdf_wall.append(std::to_string(dy));
	sdf_wall.append(" ");
	sdf_wall.append(std::to_string(dx));
	sdf_wall.append(" ");
	sdf_wall.append(std::to_string(dz));
	sdf_wall.append("</size>");
	sdf_wall.append("          </box>");
	sdf_wall.append("        </geometry>");
	sdf_wall.append("      </collision>");
/*
	sdf_wall.append("      <visual name=\"visual\">");
	sdf_wall.append("        <geometry>");
	sdf_wall.append("          <box>");
	sdf_wall.append("            <size>");
	sdf_wall.append(std::to_string(dy));
	sdf_wall.append(" ");
	sdf_wall.append(std::to_string(dx));
	sdf_wall.append(" ");
	sdf_wall.append(std::to_string(dz));
	sdf_wall.append("</size>");
	sdf_wall.append("          </box>");
	sdf_wall.append("        </geometry>");
	sdf_wall.append("      </visual>");
*/
	sdf_wall.append("    </link>");
	sdf_wall.append("  </model>");
	sdf_wall.append("</sdf>");
}
	// create Model
  ros::NodeHandle node_to_spawn_model;
  ros::ServiceClient service_to_spawn_model = node_to_spawn_model.serviceClient<gazebo_msgs::SpawnModel>("/gazebo/spawn_sdf_model");
  gazebo_msgs::SpawnModel new_model_data;

  geometry_msgs::Pose wall_position; 
  wall_position.position.x = x;
  wall_position.position.y = y;
  wall_position.position.z = z;

  wall_position.orientation.x = 0;
  wall_position.orientation.y = 0;
  wall_position.orientation.z = 0;
  wall_position.orientation.w = 1.0;

  new_model_data.request.model_xml = sdf_wall;

  new_model_data.request.model_name = wall_name;
  new_model_data.request.initial_pose = wall_position;
  if (service_to_spawn_model.call(new_model_data)){ 
  	geometry_msgs::Twist twist;
	twist.linear.x = 0.0;
	twist.linear.y = 0.0;
	twist.linear.z = 0.0;
	twist.angular.x = 0.0;
	twist.angular.y = 0.0;
	twist.angular.z = 0.0;
	ModelPosition *etalon_wall_position = new ModelPosition(wall_name, wall_position, twist);
	gl_map_of_models_positions[wall_name] = etalon_wall_position;
	return 0;
  }
  else{ 
  	printf("%s %s\n","Problem With World Creation: ", wall_name.c_str() );
    return 1;
  }
};

int createWorld(double wide_x, double wide_y, double wide_z){
	int rez[5] = {0,0,0,0,0};
	rez[0] = createWall("Wall_N", wide_x + WALL_WIDTH*2, WALL_WIDTH, wide_z, +wide_y/2 + WALL_WIDTH/2, 0, wide_z/2); // North Wall  
	rez[1] = createWall("Wall_S", wide_x + WALL_WIDTH*2, WALL_WIDTH, wide_z, -wide_y/2 - WALL_WIDTH/2, 0, wide_z/2); // South Wall
	rez[2] = createWall("Wall_E", WALL_WIDTH, 			     wide_y, wide_z, 0, +wide_x/2 + WALL_WIDTH/2, wide_z/2); // East Wall
	rez[3] = createWall("Wall_W", WALL_WIDTH, 			     wide_y, wide_z, 0, -wide_x/2 - WALL_WIDTH/2, wide_z/2); // West Wall
	rez[4] = createWall("Roof",   wide_x + WALL_WIDTH*2, wide_y + WALL_WIDTH*2, WALL_WIDTH,     0, 0, wide_z + WALL_WIDTH/2); // Roof

	for (int i = 0; i < 5; ++i)
	{
		if (rez[i]) {return 1;}
	}
	return 0;
};


void deleteAllModels(std::map<std::string, ModelPosition*> *map_of_models){
  ros::NodeHandle node_to_delete_model;
  ros::ServiceClient service_to_delete_model = node_to_delete_model.serviceClient<gazebo_msgs::DeleteModel>("/gazebo/delete_model");
  gazebo_msgs::DeleteModel deletion_information;

  for (auto i = map_of_models->begin(); i != map_of_models->end(); ++i)
  {
    deletion_information.request.model_name = i->first;
   	if (service_to_delete_model.call(deletion_information))
   	 {
   	 	delete i->second;
   	 }
  }
  map_of_models->clear();
}; 
void destroyWorld(){
	deleteAllModels(&gl_map_of_models_positions); 
};

void changeColor(int object_id, std::string color);
void changeStatus(int object_id, int hold);
													 
#define GET_COORDS(coordinate) 																		 \
  ros::NodeHandle node_to_get_model_coords; 														 \
  ros::ServiceClient service_to_get_model_coords 													 \
  	= node_to_get_model_coords.serviceClient<gazebo_msgs::GetModelState>("/gazebo/get_model_state"); \
  gazebo_msgs::GetModelState model_data; 															 \
 																									 \
  model_data.request.model_name = model_name; 														 \
 																									 \
  if (!service_to_get_model_coords.call(model_data)) 												 \
  { 																								 \
  	return 1; 																						 \
  } 																								 \
  return formatDoubleToFourNumbers(model_data.response.pose.position.coordinate);
/////////////

double getX(std::string model_name){
	GET_COORDS(x);
};
double getY(std::string model_name){
	GET_COORDS(y);
};
double getZ(std::string model_name){
	GET_COORDS(z);
};
double getAngle(std::string model_name){
  ros::NodeHandle node_to_get_model_coords;
  ros::ServiceClient service_to_get_model_coords
  	= node_to_get_model_coords.serviceClient<gazebo_msgs::GetModelState>("/gazebo/get_model_state");
  gazebo_msgs::GetModelState model_data;

  model_data.request.model_name = model_name;

  if (!service_to_get_model_coords.call(model_data))
  { return 1;}
  return formatDoubleToFourNumbers(model_data.response.pose.orientation.w);
};

void testPtintFunction(){
	printf("%s\n","This is data fom gazebo_functions.cpp" );
};

void initRosSystem(){
	// proxy args for ros::init()
	int proxy_argc=1;
	char* proxy_argv[] = {"proxy"};
	ros::init(proxy_argc, proxy_argv, "rcml_gazebo_module");
};

int createModel(std::string model_name, 
				double pos_x,    double pos_y,    double pos_z,
				double orient_x, double orient_y, double orient_z,
				double orient_w, double is_static,
				std::string path){
/// Сначала сделаем такую штуку
	// Пусть у нас будет создан узел, РосНод к кторому будем постоянно подключаться чтобы не плоожить нового.
	//Каждый раз когда функция вызвается будем создавать топик и подключаться к соответствующей службе
	// Теперь.
	// Мы начинаем разбирать что е будет делать криэйтМодел()
	// 
	// Создаем службу к которой будем подключаться с топиком по созданию моделей.
	// Записываем в структуру что будем отправлять у нас 8 параметров, которые не надо особо преобразовывать, просто правильно записать. 
	// И их будем отправлять.
	//
	// Так вот дальше мы смотрим путь до файла.
	// По идее если мы просто собираем нашу модель, то нам нужно создать строковую переменную в которой сформируем ХМЛ код модели
	// Создали модель и в сетку столкновений вставляем имя модели с .stl а в  визуализацию файл с .dae
	// Ну и теперь все это собрали и запихиваем
	// и отправляем через srvice.call()
	//// Try To Spawn MODELS
  // string model_name                 # name of the model to be spawn
  // string model_xml                  # this should be an urdf or gazebo xml
  // string robot_namespace            # spawn robot and all ROS interfaces under this namespace
  // geometry_msgs/Pose initial_pose   # only applied to canonical body
  // string reference_frame            # initial_pose is defined relative to the frame of this model/body
                                    // # if left empty or "world", then gazebo world frame is used
                                    // # if non-existent model/body is specified, an error is returned
                                    // #   and the model is not spawned
  // ---
  // bool success                      # return true if spawn successful
// string status_message             # comments if available

  printf("%s\n","CreateModel Called" );

  ros::NodeHandle node_to_spawn_model;
  ros::ServiceClient service_to_spawn_model = node_to_spawn_model.serviceClient<gazebo_msgs::SpawnModel>("/gazebo/spawn_sdf_model");
  gazebo_msgs::SpawnModel new_model_data;

  geometry_msgs::Pose model_position; 
  model_position.position.x = pos_x;
  model_position.position.y = pos_y;
  model_position.position.z = pos_z;

  model_position.orientation.x = orient_x;
  model_position.orientation.y = orient_y;
  model_position.orientation.z = orient_z;
  model_position.orientation.w = 1.0;

///////////////////////////////////////// FILE CONNECT
  ///// Connect To Files
  std::string xml_string("");

  FILE* pointer_to_file;
  pointer_to_file = fopen(path.c_str(), "r+");
  if (pointer_to_file != NULL) {
  	// Чтение из файла
    while(!feof(pointer_to_file)){
      char _mystring [100];
      fgets (_mystring , 100 , pointer_to_file);
      xml_string.append(_mystring);
    }
    fclose(pointer_to_file);
  } else {
    fclose(pointer_to_file);
    return 2;
  }
/////////////////////////////////////////
//
  std::string true_str("true");
  std::string false_str("false");

  int start_pos = xml_string.find("<static>") + STATIC_POSITION_OFFSET;
  int end_pos   = xml_string.find("</static>");

if ( start_pos != std::string::npos){
	if (is_static)
	{
		xml_string.replace(start_pos, end_pos - start_pos, true_str );
	}
	else {
		xml_string.replace(start_pos, end_pos - start_pos, false_str );
	}	
}
else {
	std::string static_true_str("<static>true</static>");
	std::string static_false_str("<static>false</static>");
	start_pos = xml_string.find("</model>");
	if (is_static)
	{
		xml_string.insert(start_pos, static_true_str );
	}
	else {
		xml_string.insert(start_pos, static_false_str );
	}	
}
//////////////////////////////////////////////////////
 
  new_model_data.request.model_xml = xml_string;

  new_model_data.request.model_name = model_name;
  new_model_data.request.initial_pose = model_position;
  if (service_to_spawn_model.call(new_model_data))
  { 
	geometry_msgs::Twist twist;
	twist.linear.x = 0.0;
	twist.linear.y = 0.0;
	twist.linear.z = 0.0;
	twist.angular.x = 0.0;
	twist.angular.y = 0.0;
	twist.angular.z = 0.0;
	ModelPosition *etalon_model_position = new ModelPosition(model_name, model_position, twist);

	ros::NodeHandle node_to_get_model_coords;
  	ros::ServiceClient service_to_get_model_coords
  		= node_to_get_model_coords.serviceClient<gazebo_msgs::GetModelState>("/gazebo/get_model_state");
  	gazebo_msgs::GetModelState model_data;
  	model_data.request.model_name = model_name;
  	usleep(50000);
  	int error_with_model_created = service_to_get_model_coords.call(model_data);
  	ModelPosition *actual_model_position;
  	if (error_with_model_created){
  		actual_model_position = new ModelPosition(model_name, model_data.response.pose, model_data.response.twist);
  		if (!etalon_model_position->isEqualToEtalon(actual_model_position))
  		{return 3;}
  	}
  	else{ return 6;}
  	// Test Other Models
  	delete actual_model_position;
	CHECK_OTHER_MODELS_POSITIONS;
	gl_map_of_models_positions[model_name] = etalon_model_position;

    return 0;
  }
  else
  { return 1; }
};

void deleteModel(){

};

int moveModel( std::string model_name,
			   double x, double y, double z, double _speed,
			   double _angle, double _angle_speed ){

	geometry_msgs::Twist zero_twist;
	zero_twist.linear.x = 0.0;
	zero_twist.linear.y = 0.0;
	zero_twist.linear.z = 0.0;
	zero_twist.angular.x = 0.0;
	zero_twist.angular.y = 0.0;
	zero_twist.angular.z = 0.0;

	geometry_msgs::Pose model_position; 
	model_position.position.x = x;
	model_position.position.y = y;
	model_position.position.z = z;

	model_position.orientation.x = 0;
	model_position.orientation.y = 0;
	model_position.orientation.z = 0;
	model_position.orientation.w = _angle;

	if (_speed)
	{
		// recieve zero values
		ros::NodeHandle node_to_get_model_coords;
		ros::ServiceClient service_to_get_model_coords
			= node_to_get_model_coords.serviceClient<gazebo_msgs::GetModelState>("/gazebo/get_model_state");
		gazebo_msgs::GetModelState model_data;
		
		model_data.request.model_name = model_name;
		
		if (!service_to_get_model_coords.call(model_data)) {
		}
		// Calculate move points
		int _freq = 5;
		double x0 = model_data.response.pose.position.x;
		double y0 = model_data.response.pose.position.y;
		double z0 = model_data.response.pose.position.z;
		double angle0 = model_data.response.pose.orientation.w;

		double new_x = x0;
		double new_y = y0;
		double new_z = z0;
		double new_angle = angle0;

		double delta_x = x-x0;
		double delta_y = y-y0;
		double delta_z = z-z0;
		double delta_angle = _angle - angle0;

		int dt = sqrt(pow(delta_x,2) + pow(delta_y,2) + pow(delta_z,2))/_speed; 
		int n = dt*_freq+1;

		ModelPosition *etalon_model_position = new ModelPosition(model_name, model_position, zero_twist);
		for (int i = 1; i < n-1; ++i)
		{
			new_x = x0 + i*delta_x/n;
			new_y = y0 + i*delta_y/n;
			new_z = z0 + i*delta_z/n;

			new_angle = angle0 + i*delta_angle/n;
			TELEPORTATION(new_x, new_y, new_z, new_angle);
		}
		TELEPORTATION(x, y, z, _angle);
		
		gl_map_of_models_positions[model_name] = etalon_model_position;
		CHECK_OTHER_MODELS_POSITIONS;
	}
	else 
	{
		// Teleportation
		printf("%s\n", "Teleportation");
		ros::NodeHandle node_to_set_model_coords;
	  	ros::ServiceClient service_to_move_model
	  		= node_to_set_model_coords.serviceClient<gazebo_msgs::SetModelState>("/gazebo/set_model_state");
	  	gazebo_msgs::SetModelState model_data;

	  	model_data.request.model_state.model_name = model_name;
	  	model_data.request.model_state.twist = zero_twist;

	  	

	  	ModelPosition *etalon_model_position = new ModelPosition(model_name, model_position, zero_twist);

	  	model_data.request.model_state.pose = model_position;
	  	if (service_to_move_model.call(model_data)){ 
	  		gl_map_of_models_positions[model_name] = etalon_model_position;
	  		CHECK_OTHER_MODELS_POSITIONS;
	  		return 0;
	  	}
	  	return 1;
/*
	  	// model_data.request.pose.position.x = x;
	  	// model_data.request.pose.position.y = y;
	  	// model_data.request.pose.position.z = z;

	  	// model_data.request.pose.orientation.x = 0;
	  	// model_data.request.pose.orientation.y = 0;
	  	// model_data.request.pose.orientation.z = 1;
	  	// model_data.request.pose.orientation.w = _angle;

	    // srv.response.link_state.pose.position.x;
	    // srv.response.link_state.pose.position.y;
	    // srv.response.link_state.pose.position.z;
		/////////
	    // srv.response.link_state.pose.orientation.x;
	    // srv.response.link_state.pose.orientation.y;
	    // srv.response.link_state.pose.orientation.z;
	    // srv.response.link_state.pose.orientation.w;
		///////
	    // srv.response.link_state.twist.linear.x;
	    // srv.response.link_state.twist.linear.y;
	    // srv.response.link_state.twist.linear.z;
		// ///////
	    // srv.response.link_state.twist.angular.x;
	    // srv.response.link_state.twist.angular.y;
	    // srv.response.link_state.twist.angular.z;
*/
	}
	return 0;
};

// 
