#include "Tools.h"

class LoadBalancer
{
  public:
	LoadBalancer(string commmandsFile, string _dirname);
	~LoadBalancer();
	
	void readCommands(string commmandsFile);
	void createUnnamedPipes();
	void fillUnnamedPipes();
	void createNamedPipes();
	void forkWorkers();
	void runWorker(int i);
	void getWorkersData();
	void showResult();
	
	string getWorkerData(int i);   

  private:
	int processCount;
	string dirName;

	vector <pid_t> workers;
	vector <string> answer;
	vector <string> workerBackPipes;

	vector < vector <int>> unnamedPipes;
	vector < vector <string>> commands;
};