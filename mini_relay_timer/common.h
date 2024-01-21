
/**************************************************************************/
/*!
  @file     common.h

  Original library by Pablin where all common resources are declared.

  License: MIT (see LICENSE)
*/
/**************************************************************************/

#ifndef _COMMON_H_
#define _COMMON_H_

#include <Arduino.h>

enum EXEC_STATUSES
{
  NO_COMMAND = -4,
  TOO_FEW_ARGUMENTS = -3,
  COMMAND_SUCCESSFUL = -2,
  BAD_COMMAND = -1,
};

void printErrorMessage(EXEC_STATUSES, String);

#endif // _COMMON_H_