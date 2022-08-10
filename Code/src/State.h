#include "Tools.h"

class State
{
  public:
	State(int pipeFd);
	~State();    
	void setCities();
	void createUnnamedPipes();
	void fillUnnamedPipes();
	void createNamedPipes();
	void forkCities();
	void runCity(int i);
	void getCitiesData();    
	void sendContentsToW();

	string getCityData(int i);

  private:
	int cityCount;
	
	string command;
	string fifoPath;
	string resultStr;
	string directory;	

	vector <pid_t> cities;	
	vector <string> result;    
	vector <string> citiesName;

	vector < vector <int>> unnamedPipes;
};
