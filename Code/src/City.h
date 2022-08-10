#include "Tools.h"

class City
{
  public:
	City(int pipeFd);
	~City();

	void setStores();
	void createUnnamedPipes();
	void fillUnnamedPipes();
	void createNamedPipes();
	void forkStores();
	void runStore(int i);
	void getStoresData();
	void sendContentsToS();

	string getStoreData(int i);

  private:
	int storeCount;  	
	string command;
	string fifoPath;
	string resultStr;
	string directory;

	vector <pid_t> stores;
	vector <string> result;
	vector <string> storesName;

	vector < vector <int>> unnamedPipes;
};
