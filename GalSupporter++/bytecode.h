#pragma once

/*---------------------------------------------
* GS_CommandBasic
* ---------------------------------------------
* Virtual BaseClass of all bytecode commands.
* 
*----------------------------------------------*/
class GS_CommandBasic
{
public:
	virtual int execute() = 0;
};

