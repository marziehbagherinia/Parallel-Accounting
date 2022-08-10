#include "Worker.h"

int main(int argc, char const *argv[])
{
	int unnamedPipeFd = atoi(argv[1]);
    Worker worker(unnamedPipeFd);
    worker.setStates();    
	worker.createUnnamedPipes();
	worker.fillUnnamedPipes();
	worker.forkStates();
    worker.createNamedPipes();
    worker.getStatesData();
    worker.provideAnswer();  
    worker.sendContentsToLB();
}
