#pragma once
//Вывод ошибок

#include "ofMain.h"

#define kuAssertWarning( val, message ) \
{ if (!(val)) { cout << "WARNING: " << message << endl; ofSystemAlertDialog( message ); __debugbreak(); } }

#define kuAssertCritical( val, message ) \
{ if (!(val)) { cout << "CRITICAL ERROR: " << message << endl; ofSystemAlertDialog( message ); __debugbreak(); OF_EXIT_APP(0); } }

#define kuExit( message ) \
{ cout << "EXITING: " << message << endl; ofSystemAlertDialog( message ); OF_EXIT_APP(0); }
