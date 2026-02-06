#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen.
brain  Brain;


//Devices
controller Controller1 = controller(primary);


// Drive Train //
// Ernie //
// motor Fleft = motor(PORT19, ratio6_1, true);
// motor FBleft = motor(PORT20, ratio6_1, false);
// motor Sleft = motor(PORT18, ratio6_1, true);
// motor BFleft = motor(PORT17, ratio6_1, false);
// motor Bleft = motor(PORT16, ratio6_1, true);

// motor Fright = motor(PORT14, ratio6_1, false);
// motor FBright = motor(PORT13, ratio6_1, true);
// motor Sright = motor(PORT15, ratio6_1, false);
// motor BFright = motor(PORT12, ratio6_1, true);
// motor Bright = motor(PORT11, ratio6_1, false);
// -- //


// Bert //
motor Fleft = motor(PORT19, ratio6_1, true);
motor FBleft = motor(PORT20, ratio6_1, false);
motor Sleft = motor(PORT18, ratio6_1, true);
motor BFleft = motor(PORT16, ratio6_1, false);
motor Bleft = motor(PORT17, ratio6_1, true);

motor Fright = motor(PORT11, ratio6_1, false);
motor FBright = motor(PORT12, ratio6_1, true);
motor Sright = motor(PORT15, ratio6_1, false);
motor BFright = motor(PORT14, ratio6_1, true);
motor Bright = motor(PORT13, ratio6_1, false);
// -- //



/////////////////// X-Drive ///////////////////////////////
motor FTleft = motor(PORT4, ratio6_1, false);
motor BTleft = motor(PORT4, ratio6_1, false);
motor BBleft = motor(PORT4, ratio6_1, true);
motor FTright = motor(PORT4, ratio6_1, true);
motor BTright = motor(PORT4, ratio6_1, true);
motor BBright = motor(PORT4, ratio6_1, false);

motor_group LeftFrontDrive (FTleft,  FBleft);
motor_group LeftBackDrive  (BTleft,  BBleft);
motor_group RightFrontDrive(FTright, FBright);
motor_group RightBackDrive (BTright, BBright);
/////////////////// ------ ///////////////////////////////

/// Intake Motors //
motor BottomIntake = motor(PORT3, ratio6_1, false);
motor MiddleIntake = motor(PORT1, ratio6_1, true);
motor TopIntake = motor(PORT2, ratio6_1, true);



digital_out MatchLoader = digital_out(Brain.ThreeWirePort.G);
digital_out Wing = digital_out(Brain.ThreeWirePort.H);
digital_out Flap = digital_out(Brain.ThreeWirePort.E);
digital_out Intake = digital_out(Brain.ThreeWirePort.F);

inertial Inertial = inertial(PORT10);
rotation Sideways = rotation(PORT9, false);
rotation ForwardTrack = rotation(PORT8, true);

distance Distance = distance(PORT7);
optical Optical = optical(PORT4);



//Add your devices below, and don't forget to do the same in robot-config.h:


void vexcodeInit( void ) {
  // nothing to initialize
}