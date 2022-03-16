#ifndef SICK_GENERIC_LASER_H
#define SICK_GENERIC_LASER_H
/*
 *
 */
#ifndef _MSC_VER
#endif

#include <sick_scan/sick_scan_common_tcp.h>

enum NodeRunState
{
  scanner_init, scanner_run, scanner_finalize
};


bool startGenericLaser(int argc, char **argv, std::string nodeName, rosNodePtr nhPriv, int* exit_code);

int mainGenericLaser(int argc, char **argv, std::string scannerName, rosNodePtr nh);

void rosSignalHandler(int signalRecv);

void setVersionInfo(std::string _versionInfo);

std::string getVersionInfo();

bool parseLaunchfileSetParameter(rosNodePtr nhPriv, int argc, char **argv);

bool stopScannerAndExit(bool force_immediate_shutdown = false);

#endif

