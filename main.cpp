#include "eosSystemCore.h"

int main()
{
	eos::sys::Core* core = eos::sys::Core::Create();
	return core->MainLoop();
}