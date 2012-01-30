#include <string>
#include <iostream>
#include <sstream>

//#include "math.h"

const char* FloatToString(float val, const char* newStr) {

	int temp = (int)val;

	std::stringstream sstr;
	sstr<<val;
	std::string str1 = sstr.str();

	return str1.c_str();
}






/*
		Public Function GetHeading(NX As Single, NY As Single, TargetX As Long, TargetY As Long) As Single
'On Error Resume Next
Dim XDiff As Single
Dim YDiff As Single

'Find the Difference between the 2 coords
XDiff = TargetX - NX
YDiff = NY - TargetY

'Do the math
If Sgn(YDiff) > 0 Then GetHeading = Atn(XDiff / YDiff)
If Sgn(YDiff) < 0 Then GetHeading = Atn(XDiff / YDiff) + (Atn(1) * 4)

'Add twopi is necessary
If GetHeading < 0 Then GetHeading = GetHeading + (Atn(1) * 8)

End Function
*/