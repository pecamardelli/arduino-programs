// #include "DCMotor.h"

// DCMotor::DCMotor(/* args */) {
//   pinMode(AIA, OUTPUT);
//   pinMode(AIB, OUTPUT);
// }

// DCMotor::~DCMotor() {}

// void DCMotor::run(byte speed) {
//   analogWrite(AIA, 0);
//   analogWrite(AIB, speed);
// }

// /**************************************************************************/
// /*!
//     @brief  Executes a user command.
//     @param  args Array of Strings representing the command itself and its
//    arguments.
// */
// /**************************************************************************/
// EXEC_STATUSES DCMotor::exec(String args[]) {
//   if (args[0].equals("motor")) {
//     if (args[1].equals("speed")) {
//       run(args[2].toInt());
//       return COMMAND_SUCCESSFUL;
//     } else {
//       return BAD_COMMAND;
//     }
//   }

//   return NO_COMMAND;
// }