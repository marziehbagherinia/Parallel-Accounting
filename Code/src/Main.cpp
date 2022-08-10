#include "LoadBalancer.h"

int main(int argc, char const *argv[])
{
	string commandsFile = argv[1];
	string assetsDirectory = argv[2];

	LoadBalancer loadBalancer(commandsFile, assetsDirectory);
	loadBalancer.createUnnamedPipes();
	loadBalancer.fillUnnamedPipes();
	//dsfk
	loadBalancer.forkWorkers();
    loadBalancer.createNamedPipes();
    loadBalancer.getWorkersData();
    loadBalancer.showResult();
    
	return 0;
}
