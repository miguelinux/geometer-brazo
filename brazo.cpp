#include "Aria.h"

/** @example armExample.cpp Example program showing use of the P2 arm accessory.
 *
 *  Demonstrates how to connect with the Pioneer 2 controller
 *  and set up the P2Arm class, including the ARMpac handler.
 *  It simply queries and prints the status of the arm, moving it to a position, then exits.
 *
*/

ArRobot robot;
ArP2Arm arm;
P2ArmJoint *joint;

void brazo(int *pos)
{
  // Move the arm joints to specific positions
  printf("Moving Arm...\n");

  for (int i=1; i<=ArP2Arm::NumJoints; i++)
  {
    joint = arm.getJoint(i);
    arm.moveToTicks(i, joint->myCenter + pos[i]);
  }

  // Wait for arm to achieve new position, printing joint positions and M for
  // moving, NM for not moving.
  arm.requestStatus(ArP2Arm::StatusContinuous);
  ArUtil::sleep(300); // wait a moment for arm status packet update with joints moving
  bool moving = true;
  while (moving)
  {
      moving = false;
      for (int i=1; i<=ArP2Arm::NumJoints; i++)
      {
        if (arm.getMoving(i))
        {
	        moving = true;
	      }
        else
        {
        }
	    }
  }
}

void saludo()
{
  //                0   1    2   3  4  5  6
  int position[] = {0, -60, -90,  0, 0, 0, 0};

  brazo(position);
  //ArUtil::sleep(1000);

  position[3] = -30;
  brazo(position);
  //ArUtil::sleep(800);

  position[3] =  50;
  brazo(position);
  //ArUtil::sleep(800);

  position[3] = -30;
  brazo(position);
  //ArUtil::sleep(800);

  position[3] =  50;
  brazo(position);
  //ArUtil::sleep(800);

  position[3] = -30;
  brazo(position);
  //ArUtil::sleep(800);

  position[3] =  50;
  brazo(position);

  ArUtil::sleep(1000);
  position[2] = 60;
  position[3] =  0;
  brazo(position);
}

void saluda()
{
  /*
   * Saluda
   */
  int position[] = {0, -90, -20, 0, 0, 0, 0};
  brazo(position);
  ArUtil::sleep(2000);

  position[2] =  0;
  brazo(position);
  ArUtil::sleep(300);

  position[2] = -20;
  brazo(position);
  ArUtil::sleep(300);

  position[2] =  0;
  brazo(position);
  ArUtil::sleep(300);

  position[2] = -20;
  brazo(position);
  ArUtil::sleep(1000);

  position[2] = 60;
  brazo(position);
  ArUtil::sleep(2000);
}

int main(int argc, char **argv)
{
  Aria::init();
  ArSimpleConnector con(&argc, argv);


  if(!Aria::parseArgs()) 
  {
    Aria::logOptions();
    Aria::shutdown();
    return 1;
  }

  ArLog::log(ArLog::Normal, "armExample: Connecting to the robot.");
  if(!con.connectRobot(&robot))
  {
    ArLog::log(ArLog::Terse, "armExample: Could not connect to the robot. Exiting.");
    Aria::shutdown();
    return 1;
  }
  robot.runAsync(true);

  // turn off sonar
  robot.comInt(28, 0);

  // Set up and initialize the arm
  arm.setRobot(&robot);
  if (arm.init() != ArP2Arm::SUCCESS)
  {
    ArLog::log(ArLog::Terse, "armExample: Error initializing the P2 Arm!");
    return 1;
  }

  // Print out some of the settings
  printf("Current joint info:\nJoint   Vel  Home  Center\n");
  for (int i=1; i<=ArP2Arm::NumJoints; i++)
  {
    joint = arm.getJoint(i);
    printf("  %2i:  %5i %5i   %5i\n", i, joint->myVel, joint->myHome, joint->myCenter);
  }
  printf("\n");

  // Put the arm to work
  printf("Powering on  (takes a couple seconds to stabilize)\n");
  arm.powerOn();

  // Request one status packet and print out the arm's status
  printf("Current arm status:\n");
  arm.requestStatus(ArP2Arm::StatusSingle);
  ArUtil::sleep(200);  // Give time to get the packet
  printf("Arm Status: ");
  if (arm.getStatus() & ArP2Arm::ArmGood)
    printf("Good=1 ");
  else
    printf("Good=0 ");
  if (arm.getStatus() & ArP2Arm::ArmInited)
    printf("Inited=1 ");
  else
    printf("Inited=0 ");
  if (arm.getStatus() & ArP2Arm::ArmPower)
    printf("Power=1 ");
  else
    printf("Power=0 ");
  if (arm.getStatus() & ArP2Arm::ArmHoming)
    printf("Homing=1 ");
  else
    printf("Homing=0 ");
  printf("\n\n");


  /* 0 Nada
   * 1 Izquerda/Derecha
   * 2 Arriba/Abajo
   * 3 Codo
   */
  /*

  */
  int opcion;

  do {

    printf("0: Salir\n");
    printf("1: Saludo (HOLA)\n");
    printf("2: Saluda (Mucho gusto)\n");
    scanf("%d",&opcion);
    switch(opcion) {
      case 1:
	      saludo();
	      break;
      case 2:
	      saluda();
	      break;
    }

  } while(opcion!=0);
  



  // Return arm to park, wait, and disconnect. (Though the arm will automatically park
  // on client disconnect)
  printf("Parking arm.\n");
  arm.park();

  // Wait 5 seconds or until arm shuts off
  for(int i = 5; (i > 0) && (arm.getStatus() & ArP2Arm::ArmPower); i--)
  {
    ArUtil::sleep(1000);
  }

  // Disconnect from robot, etc., and exit.
  printf("Shutting down ARIA and exiting.\n");
  Aria::shutdown();

  return(0);
}

