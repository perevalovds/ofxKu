#include "kuErrors.h"


//---------------------------------------------------------------------
bool kuAssert(bool v, const string &message) {
	if (!v) { ofSystemAlertDialog(message); return true; }
	return false;
}


//---------------------------------------------------------------------

