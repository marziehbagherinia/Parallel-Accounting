#include "Tools.h"

class Store
{
  public:
	Store(int pipeFd);
	~Store();

	void readInstance();
	void find_price();	
	void sendContentsToC();

	bool is_valid_date(vector <int> date);

  private:
	int id;

	string command;
	string fifoPath;	
	string directory;
	string start_date;
	string end_date;

	vector <int> result; 
	vector <int> price;
	vector <int> product_id;
	
	vector < vector <int>> dates;
	vector < vector <int>> unnamedPipes;
};
