using namespace vex;

extern brain Brain;

//To set up a motor called LeftFront here, you'd use
//extern motor LeftFront;

extern controller Controller1;
extern motor FTleft;
extern motor FBleft;
extern motor BTleft;
extern motor BBleft;
extern motor FTright;
extern motor FBright;
extern motor BTright;
extern motor BBright;

extern motor Fleft;
extern motor FBleft;
extern motor Sleft;
extern motor BFleft;
extern motor Bleft;
extern motor Fright;
extern motor FBright;
extern motor Sright;
extern motor BFright;
extern motor Bright;

extern inertial Inertial;
extern motor BottomIntake;
extern motor MiddleIntake;
extern motor TopIntake;


extern digital_out MatchLoader;
extern digital_out Wing;
extern digital_out Flap;
extern digital_out Intake;

extern motor_group LeftFrontDrive;
extern motor_group LeftBackDrive;
extern motor_group RightFrontDrive;
extern motor_group RightBackDrive;

extern inertial Inertial;
extern rotation Sideways;
extern rotation ForwardTrack;
extern distance Distance;
extern optical Optical;



void  vexcodeInit( void );