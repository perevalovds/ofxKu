#pragma once
//Вывод ошибок

#include "ofMain.h"

#ifdef _DEBUG
#define KU_DEBUG_BREAK __debugbreak();
#else
#define KU_DEBUG_BREAK {}
#endif

#define kuAssertWarning( val, message ) \
{ if (!(val)) { \
    cout << "WARNING: " << message << endl; \
    ofSystemAlertDialog( message ); \
    KU_DEBUG_BREAK \
    } \
}

#define kuAssertCritical( val, message ) \
{ if (!(val)) { cout << "CRITICAL ERROR: " << message << endl; ofSystemAlertDialog( message ); __debugbreak(); OF_EXIT_APP(0); } }

#define kuExit( message ) \
{ cout << "EXITING: " << message << endl; ofSystemAlertDialog( message ); OF_EXIT_APP(0); }
