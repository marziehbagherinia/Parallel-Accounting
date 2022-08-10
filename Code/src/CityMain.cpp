#include "City.h"

int main(int argc, char const *argv[])
{
	int unnamedPipeFd = atoi(argv[1]);
    City city(unnamedPipeFd);
    city.setStores();
	city.createUnnamedPipes();
	city.fillUnnamedPipes();
	city.forkStores();
    city.createNamedPipes();
    city.getStoresData();
    city.sendContentsToS();	
}