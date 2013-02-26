#include <iostream>

#include "framework/Log.hpp"
#include "framework/StringUtil.hpp"

using namespace FrameWork;
using namespace std;


int main(int argc, char* argv[])
{

	TRACE("The value o  more iterations left " , 99 , "hihihi" );
	DEBUG("The value o  more iterations left " , 99 , "hihihi" );
	INFO("The value o  more iterations left " , 99 , "hihihi" );
	WARN("The value o  more iterations left " , 99 , "hihihi" );
	ERROR("The value o  more iterations left " , 99 , "hihihi" );
	FAULT("The value o  more iterations left " , 99 , "hihihi" );

	DEBUG(StringUtil::ToPaddedString(1234, 4));
	
	return 0;
}