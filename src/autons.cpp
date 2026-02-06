#include "vex.h"
#include <atomic>

std::atomic<bool> intakeDone(false);
std::atomic<bool> intakeCancel(false);



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




void DriveOn(int RPM)
{
  Fleft.spin(fwd, RPM, rpm); 
  FBleft.spin(fwd, RPM, rpm);
  Sleft.spin(fwd, RPM, rpm);
  BFleft.spin(fwd, RPM, rpm);
  Bleft.spin(fwd, RPM, rpm);

  Fright.spin(fwd, RPM, rpm);
  FBright.spin(fwd, RPM, rpm);
  Sright.spin(fwd, RPM, rpm);
  BFright.spin(fwd, RPM, rpm);
  Bright.spin(fwd, RPM, rpm);
}

void DriveOnRev(int RPM)
{
  Fleft.spin(reverse, RPM, rpm); 
  FBleft.spin(reverse, RPM, rpm);
  Sleft.spin(reverse, RPM, rpm);
  BFleft.spin(reverse, RPM, rpm);
  Bleft.spin(reverse, RPM, rpm);

  Fright.spin(reverse, RPM, rpm);
  FBright.spin(reverse, RPM, rpm);
  Sright.spin(reverse, RPM, rpm);
  BFright.spin(reverse, RPM, rpm);
  Bright.spin(reverse, RPM, rpm);
}


void IntakeBalls()
{
  BottomIntake.spin(fwd, 600, rpm);
  MiddleIntake.spin(fwd, 600, rpm);
  TopIntake.setMaxTorque(20, pct);
  TopIntake.spin(fwd, 600, rpm);
  Flap.set(true);
  Intake.set(false);
}

void IntakeStop()
{
  BottomIntake.stop(coast);
  MiddleIntake.stop(brake);
  TopIntake.stop(brake);
}

void ScoreLong()
{
  BottomIntake.spin(fwd, 600, rpm);
  MiddleIntake.spin(fwd, 600, rpm);
  TopIntake.setMaxTorque(100, pct);
  TopIntake.spin(fwd, 600, rpm);
  Flap.set(false);
}

void ScoreMid()
{
  BottomIntake.spin(fwd, 300, rpm);
  MiddleIntake.spin(fwd, 300, rpm);
  TopIntake.setMaxTorque(100, pct);
  TopIntake.spin(reverse, 300, rpm);
}

void ScoreMidFast()
{
  BottomIntake.spin(fwd, 600, rpm);
  MiddleIntake.spin(fwd, 600, rpm);
  TopIntake.setMaxTorque(100, pct);
  TopIntake.spin(reverse, 600, rpm);
}



// "Real object" filter: must be detected AND close enough to be a ball, not a wall.
bool SeesRealObjectClose() {
  double d = Distance.objectDistance(mm);
  if (d < 0) return false;  // no object

  // Tune these to your robot so it doesn't arm on a wall:
  const double close_min = 20; // mm
  const double close_max = 90; // mm
  return (d >= close_min && d <= close_max);
}

bool DistanceAboveForTime(double threshold, int time_ms) {
  int elapsed = 0;
  const int step = 10;

  while (elapsed < time_ms) {
    if (intakeCancel) return false;

    double d = Distance.objectDistance(mm);

    // -1 (no object) should NOT count as "above"
    if (d < 0 || d <= threshold) return false;

    wait(step, msec);
    elapsed += step;
  }
  return true;
}


int IntakeMatchloadThread() {
  intakeDone = false;

  const int GLOBAL_TIMEOUT_MS = 3000;  // safety timeout
  int totalElapsed = 0;

  // Start intake
  BottomIntake.spin(fwd, 600, rpm);
  MiddleIntake.spin(fwd, 600, rpm);
  TopIntake.setMaxTorque(20, pct);
  TopIntake.spin(fwd, 600, rpm);
  Flap.set(true);
  Intake.set(false);

  auto seesRealObjectClose = []() -> bool {
    double d = Distance.objectDistance(mm);
    if (d < 0) return false;
    return (d >= 20 && d <= 90); // ball window
  };

  auto aboveForTime = []() -> bool {
    int elapsed = 0;
    const int step = 10;
    while (elapsed < 1000) {
      double d = Distance.objectDistance(mm);
      if (d < 0 || d <= 110) return false;
      wait(step, msec);
      elapsed += step;
    }
    return true;
  };

  // Phase 1: wait for first real object OR timeout
  while (!seesRealObjectClose() && totalElapsed < GLOBAL_TIMEOUT_MS) {
    wait(20, msec);
    totalElapsed += 20;
  }

  wait(60, msec); // debounce

  // Phase 2: wait for exit condition OR timeout
  while (totalElapsed < GLOBAL_TIMEOUT_MS) {
    if (aboveForTime()) break;
    wait(20, msec);
    totalElapsed += 20;
  }

  // Stop intake (always)
  BottomIntake.stop();
  MiddleIntake.stop();
  TopIntake.stop();

  // GUARANTEED signal
  intakeDone = true;
  return 0;
}




void StopIntakeOnBlue() {
  const int TIMEOUT_MS = 4000;
  int startTime = vex::timer::system();

  Optical.setLightPower(100, pct);
  Optical.setLight(vex::ledState::on);

  ScoreLong();

  while (true) {

    // ----- TIMEOUT -----
    if (vex::timer::system() - startTime > TIMEOUT_MS) {
      IntakeStop();
      break;   // ← exits loop on timeout
    }

    // ----- SEE BLUE -----
    if (Optical.hue() > 190 && Optical.hue() < 270) {
      IntakeStop();
      // BottomIntake.spin(reverse, 600, rpm);
      // MiddleIntake.spin(reverse, 600, rpm);

      break;   // ← exits loop on blue
    }

    wait(10, msec);
  }

  // Function exits naturally after loop
}


void FlapDown()
{
  Flap.set(false);
}


void ScoreCount(int Balls)
{
  int Count = 0;
  ScoreLong();

  const int TIMEOUT_MS = 4000;  // how long to wait before giving up

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

void BertAuton()
 {
 // Start Odom thread
 thread cord = thread(odom_test);
 //Set Odom Constants
 //and Drive Constance
  odom_constants();
  chassis.set_coordinates(-16.639, -52.255, 180);
  chassis.drive_max_voltage = 5;
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
  chassis.set_turn_exit_conditions(1, 0, 3000);
// chassis.set_turn_exit_conditions(4, 0, 5000);
  // chassis.holonomic_drive_to_pose(5.94, 44.39, 180);
  // chassis.drive_stop(hold);

std::vector<Waypoint> path1 = { 
{ -16.640f, -43.577f, 0.0f },
{ -16.640f, -40.500f, 0.0f },
{ -16.640f, -37.500f, 0.0f },
{ -16.640f, -34.500f, 0.0f },
{ -16.640f, -31.500f, 0.0f },
{ -16.640f, -28.500f, 0.0f },
{ -16.640f, -25.500f, 0.0f },

// Start curving ~4" earlier (gentle lead-in)
{ -16.635f, -23.300f, 0.0f },
{ -16.625f, -22.200f, 0.0f },
{ -16.610f, -22.200f, 0.0f },
{ -16.590f, -21.200f, 0.0f },
{ -16.565f, -20.200f, 0.0f },

// Arc section
{ -16.620f, -20.000f, 0.0f },
{ -16.560f, -18.300f, 0.0f },
{ -16.400f, -16.700f, 0.0f },
{ -16.120f, -15.200f, 0.0f },
// { -15.700f, -16.800f, 0.0f },
{ -15.120f, -12.500f, 0.0f },
{ -14.400f, -11.300f, 0.0f },

// { -12.600f, -11.650f, 0.0f },

// // Smooth curve into final target (Bezier-like)
// { -11.900f, -10.300f, 0.0f },
// { -11.000f, -8.600f, 0.0f },
// { -10.000f, -6.800f, 0.0f },
// { -9.300f,  -5.300f, 0.0f }
// { -10.56f,  -6.17f, 0.0f },

// Final endpoint
// { -8.684f,  -3.950f, 0.0f }
//{-10.207, -6.597, 0.0f}


};



// Start: 
// Ending:
  chassis.drive_with_pursuit_path(path1, 0); // To MID
  // chassis.drive_distance(-9);
  // chassis.turn_to_point(-46.12, -39.74);
  chassis.set_drive_exit_conditions(2, 0, 200);
  chassis.heading_max_voltage = 2;
  chassis.drive_to_pose(-10.207, -6.597, 227);
  chassis.set_drive_exit_conditions(2, 0, 5000);
  chassis.drive_stop(coast);
  
  // Score top mid
  ScoreMid();



  // Going to Matchloader (turning towards matchloader)
    wait(1000, msec);
  chassis.set_drive_exit_conditions(2, 0, 5000);
  chassis.drive_max_voltage = 6;
  chassis.drive_to_point(-46.12, -39.74);

  MatchLoader.set(true);
  chassis.turn_to_point(-45.12, -56.50);

  thread Intake = thread(IntakeMatchloadThread);
  chassis.drive_max_voltage = 5;
  chassis.set_drive_exit_conditions(2, 0, 1000);
  chassis.drive_to_point(-45.12, -56.50);
  chassis.drive_stop(coast);
  wait(20, msec);
  DriveOn(40);
  waitUntil(intakeDone);
  chassis.heading_max_voltage = 1;


  chassis.drive_to_point(-45.43, -24.66);
  chassis.drive_stop(coast);
  wait(20, msec);
  DriveOnRev(50);
  wait(500, msec);
  StopIntakeOnBlue();
  // wait(1000, msec);

 
  chassis.drive_max_voltage = 6;
  chassis.set_drive_exit_conditions(1, 0, 2000);
  ScoreMidFast();
  chassis.drive_to_point(-44.93, -54.67); // 2nd time to matchload
  thread Intake2 = thread(IntakeMatchloadThread);
  chassis.drive_stop(coast);
  wait(10, msec);
  DriveOn(50);
  chassis.set_drive_exit_conditions(2, 0, 2000);

  waitUntil(intakeDone);
  // wait(1000000, msec);
  chassis.drive_to_point(-46.41, -25.23);
  chassis.drive_stop(coast);
  wait(20, msec);
  DriveOnRev(20);
  //wait(200, msec);
  ScoreLong();
  wait(200, msec);
  MatchLoader.set(false);
  wait(1800, msec);
  chassis.drive_to_point(-46.41, -30.33);
  chassis.drive_to_point(-46.41, -25.23);


  
 
  


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

