cmake_minimum_required(VERSION 2.8.3)
project(gazebo_rcml_module)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
)

## System dependencies are found with CMake's conventions
# find_package(Boost REQUIRED COMPONENTS system)


## Uncomment this if the package has a setup.py. This macro ensures
## modules and global scripts declared therein get installed
## See http://ros.org/doc/api/catkin/html/user_guide/setup_dot_py.html
# catkin_python_setup()

################################################
## Declare ROS messages, services and actions ##
################################################
## Generate messages in the 'msg' folder
# add_message_files(
#   FILES
#   Message1.msg
#   Message2.msg
# )

## Generate services in the 'srv' folder
# add_service_files(
#   FILES
#   Service1.srv
#   Service2.srv
# )

## Generate actions in the 'action' folder
# add_action_files(
#   FILES
#   Action1.action
#   Action2.action
# )

## Generate added messages and services with any dependencies listed here
# generate_messages(
#   DEPENDENCIES
#   std_msgs
# )

###################################
## catkin specific configuration ##
###################################
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES gazebo_rcml_module
#  CATKIN_DEPENDS roscpp std_msgs
#  DEPENDS system_lib
  CATKIN_DEPENDS 
  roscpp 
  std_msgs 
  #geometry_msgs 
  #message_runtime
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
# include_directories(include)
include_directories(
  ${catkin_INCLUDE_DIRS}
)

## Declare a C++ library
# add_library(gazebo_rcml_module
#   src/${PROJECT_NAME}/gazebo_rcml_module.cpp
# )

## Add cmake target dependencies of the library
## as an example, code may need to be generated before libraries
## either from message generation or dynamic reconfigure
# add_dependencies(gazebo_rcml_module ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

## Declare a C++ executable
# add_executable(gazebo_rcml_module_node src/gazebo_rcml_module_node.cpp)

## Add cmake target dependencies of the executable
## same as for the library above
# add_dependencies(gazebo_rcml_module_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

## Specify libraries to link a library or executable target against
# target_link_libraries(gazebo_rcml_module_node
#   ${catkin_LIBRARIES}
# )

#############
## Install ##
#############
# Add g++ parameters
ADD_DEFINITIONS("-Wall -std=c++11")

# Set path to include files
set(PATH_TO_HEADERS  /home/mike/workspace/repo/module_headers_master/ )

include_directories(include ${catkin_INCLUDE_DIRS} ${PATH_TO_HEADERS})

set(PROJECT gazebo_rcml_module)

set(HEADERS module.h function_module.h gazebo_functions.h gazebo_module.h)

set(SOURCE_EXE gazebo_module.cpp gazebo_functions.cpp )
#set(SOURCE_LIB sendCommandsLib.cpp )

#add_library( sendCommandsLib STATIC ${SOURCE_LIB}  )
set(SOURCE_LIB src/gazebo_functions.cpp )

#add_library( GazeboFunctionsLib STATIC ${SOURCE_LIB}  )

add_library(gazebo_module SHARED src/gazebo_module.cpp src/gazebo_functions.cpp)

target_link_libraries(gazebo_module ${catkin_LIBRARIES} )

