#include "State.h"

int main(int argc, char const *argv[])
{
	int unnamedPipeFd = atoi(argv[1]);
    State state(unnamedPipeFd);
    state.setCities();
	state.createUnnamedPipes();
	state.fillUnnamedPipes();
	state.forkCities();
    state.createNamedPipes();
    state.getCitiesData();
    state.sendContentsToW();    
}