#include "Tools.h"

class Worker
{
  public:
	Worker(int pipeFd);
	~Worker();

	void setStates();
	void createUnnamedPipes();
	void fillUnnamedPipes();
	void createNamedPipes();
	void forkStates();
	void runState(int i);
	void getStatesData();
	void provideAnswer();
	void sendContentsToLB();
	
	string getStateData(int i);

  private:
	int stateCount;

	string type;
	string answer;
	string command;
	string fifoPath;
	string resultStr;
	string directory;

	vector <int> result;
	vector <pid_t> states;
	vector <string> statesName;
	vector < vector <int>> unnamedPipes;

};
