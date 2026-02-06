#include "vex.h"




void default_constants(){
  // Each constant set is in the form of (maxVoltage, kP, kI, kD, startI).
  chassis.set_drive_constants(10, 1.5, 0, 10, 0);
  chassis.set_heading_constants(6, .4, 0, 1.35, 0);
  chassis.set_turn_constants(12, .4, .03, 3.75, 15);
  chassis.set_swing_constants(12, .3, .001, 2, 15);

  // Each exit condition set is in the form of (settle_error, settle_time, timeout).
  chassis.set_drive_exit_conditions(1.5, 10, 5000);
  chassis.set_turn_exit_conditions(1, 300, 3000);
  chassis.set_swing_exit_conditions(1, 300, 3000);
}

void empty_constants(){
  // Each constant set is in the form of (maxVoltage, kP, kI, kD, startI).
  chassis.set_drive_constants(10, 1.5, 0, 10, 0);
  chassis.set_heading_constants(6, .4, 0, 1.35, 0);
  chassis.set_turn_constants(12, .4, .03, 4.4, 15);
  chassis.set_swing_constants(12, .3, .001, 2, 15);

  // Each exit condition set is in the form of (settle_error, settle_time, timeout).
  chassis.set_drive_exit_conditions(1.5, 10, 5000);
  chassis.set_turn_exit_conditions(1, 300, 3000);
  chassis.set_swing_exit_conditions(1, 300, 3000);
}

void full_constants(){
  // Each constant set is in the form of (maxVoltage, kP, kI, kD, startI).
  chassis.set_drive_constants(10, 1.5, 0, 10, 0);
  chassis.set_heading_constants(6, .4, 0, 1.35, 0);
  chassis.set_turn_constants(12, .4, .03, 4.4, 15);
  chassis.set_swing_constants(12, .3, .001, 2, 15);

  // Each exit condition set is in the form of (settle_error, settle_time, timeout).
  chassis.set_drive_exit_conditions(1.5, 10, 5000);
  chassis.set_turn_exit_conditions(1, 300, 3000);
  chassis.set_swing_exit_conditions(1, 300, 3000);
}



void odom_constants(){
  default_constants();
  chassis.heading_max_voltage = 10;
  chassis.drive_max_voltage = 2;
  chassis.drive_settle_error = 3;
  chassis.boomerang_lead = .5;
  chassis.drive_min_voltage = 0;

  // std::vector<Waypoint> path = {
  // {30.0f, 20.0f,   0.0f},
  // {45.0f, 40.0f,  45.0f, test},
  // {60.0f, 60.0f,  90.0f},
  // {70.0f, 70.0f,  90.0f} 
  // };

  // chassis.drive_with_pursuit_path(path,/*setback=*/ 4.0f);
}


/**
 * Doesn't drive the robot, but just prints coordinates to the Brain screen 
 * so you can check if they are accurate to life. Push the robot around and
 * see if the coordinates increase like you'd expect.
 */
bool odomT = true;
void odom_test(){
  //chassis.set_coordinates(0, 0, 0);
  while(odomT == true){
    Brain.Screen.clearScreen();
    Brain.Screen.printAt(5,20, "X: %f", chassis.get_X_position());
    Brain.Screen.printAt(5,40, "Y: %f", chassis.get_Y_position());
    Brain.Screen.printAt(5,60, "Heading: %f", chassis.get_absolute_heading());
    Brain.Screen.printAt(5,80, "ForwardTracker: %f", chassis.get_ForwardTracker_position());
    Brain.Screen.printAt(5,100, "SidewaysTracker: %f", chassis.get_SidewaysTracker_position());
    task::sleep(20);
  }
}




void IntakeSlow()
{
  // BottomRoller.spin(fwd, 600, rpm);
  // BigRoller.spin(fwd, 600, rpm);
  // TopRoller.spin(fwd, 20, rpm); 
  // LCIntake.spin(fwd, 200, rpm);
  // RCIntake.spin(fwd, 200, rpm);
}

void ScoreLow()
{
  // BottomRoller.spin(reverse, 600, rpm);
  // BigRoller.spin(reverse, 600, rpm);
  // TopRoller.spin(reverse, 600, rpm); 
  // RCIntake.spin(reverse, 300, rpm);
  // LCIntake.spin(reverse, 300, rpm);
}

void ScoreLong()
{
  // BottomRoller.spin(fwd, 600, rpm);
  // BigRoller.spin(fwd, 600, rpm);
  // TopRoller.spin(fwd, 600, rpm); 
  // LCIntake.spin(fwd, 600, rpm);
  // RCIntake.spin(fwd, 600, rpm);
}

void ScoreMid()
{
  // BottomRoller.spin(fwd, 200, rpm);
  // BigRoller.spin(fwd, 200, rpm);
  // TopRoller.spin(fwd, 50, rpm); 
  // LCIntake.spin(fwd, 600, rpm);
  // RCIntake.spin(fwd, 600, rpm);
}

void IntakeStop()
{
  // BottomRoller.stop(coast);
  // BigRoller.stop(coast);
  // TopRoller.stop(coast);
  // LCIntake.stop(coast);
  // RCIntake.stop(coast);
}

void FunnelIn()
{
  //Funnel.set(false);
}

void LiftDown()
{
  // RightLift.set(false);
  // LeftLift.set(false);
}

void FlapDown()
{
  Flap.set(false);
}

void ChangeUpOut()
{
  // RightP.set(true);
  // LeftP.set(true);
}

void ScoreCount(int Balls)
{
  int Count = 0;
  ScoreLong();

  const int TIMEOUT_MS = 2000;  // how long to wait before giving up

  while (Count < Balls)
  {
    // ------- Wait for sensor to go BELOW 20 -------
    int startTime = vex::timer::system();
    while (Distance.value() >= 20)
    {
      if (vex::timer::system() - startTime > TIMEOUT_MS) {
        IntakeStop();
        return;   // exit the whole function on timeout
      }
    }

    // ------- Wait for sensor to go BACK ABOVE 20 -------
    startTime = vex::timer::system();
    while (Distance.value() <= 20)
    {
      if (vex::timer::system() - startTime > TIMEOUT_MS) {
        IntakeStop();
        return;   // exit on timeout
      }
    }

    Count += 1;
  }

  IntakeStop();
}



void Skills()
 {
 // Start Odom thread
 thread cord = thread(odom_test);
 //Set Odom Constants
 //and Drive Constance
  odom_constants();
  chassis.set_coordinates(0, 0, 0);
  chassis.drive_max_voltage = 2;
  chassis.heading_max_voltage = 2;
  chassis.turn_max_voltage = 5;
  //new variables
  chassis.drive_settle_error = .5;
  chassis.boomerang_lead = 0.5;
  chassis.drive_min_voltage = 0;
  //Set settle times
  // chassis.drive_settle_time = 0;
  // chassis.turn_settle_time = 0;
  // chassis.swing_settle_time = 1;
  chassis.set_drive_exit_conditions(2, 0, 5000);
  //chassis.set_turn_exit_conditions(3, 0, 3000);
  //chassis.set_turn_exit_conditions(4, 0, 5000);
  // chassis.holonomic_drive_to_pose(5.94, 44.39, 180);
  // chassis.drive_stop(hold);

  std::vector<Waypoint> path1 = {
    {0, 23, 0},
    {21, 49, 45}   // go forward 24 (inches or whatever units your odom uses)
  };

  chassis.drive_with_pursuit_path(path1, 0);
  chassis.drive_stop(hold);


  
 
  


  wait(1000000000000000000, msec);
  
  

}


void ZoeySkills()
 {
 // Start Odom thread
 thread cord = thread(odom_test);
 //Set Odom Constants
 //and Drive Constance
  odom_constants();
  chassis.set_coordinates(0, -2, 0);
  chassis.drive_max_voltage = 10;
  chassis.heading_max_voltage = 5;
  chassis.turn_max_voltage = 5;
  //new variables
  chassis.drive_settle_error = .5;
  chassis.boomerang_lead = 0.5;
  chassis.drive_min_voltage = 0;
  //Set settle times
  // chassis.drive_settle_time = 0;
  // chassis.turn_settle_time = 0;
  // chassis.swing_settle_time = 1;
  chassis.set_drive_exit_conditions(2, 0, 5000);
  //chassis.set_turn_exit_conditions(3, 0, 3000);
  //chassis.set_turn_exit_conditions(4, 0, 5000);
  // chassis.holonomic_drive_to_pose(5.94, 44.39, 180);
  // chassis.drive_stop(hold);


  

  

  wait(1000000000000000000, msec);
  
  

}

