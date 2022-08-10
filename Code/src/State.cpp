#include "State.h"

State::State(int pipeFd)
{
	char buff[MAX_BUFF];
	read(pipeFd, buff, MAX_BUFF);
	close(pipeFd);

	string helper = buff;
	
	vector <string> v = Tools::splitSpace(helper);
	if (v.size() >= 5)
	{
		directory = v[0];
		command = v[1];
		command += SPACE;
		command += v[2];
		command += SPACE;
		command += v[3];
		command += SPACE;
		command += v[4];

	}

	while (!isalpha(directory[directory.size() - 1]))
	{
		directory.erase(directory.begin() + directory.size() - 1);
	}

	cityCount = 0;
	fifoPath = Tools::getNamedFifoName(to_string(getpid()));
}

void State::setCities()
{
	DIR *dir;
	struct dirent *ent;

	if ((dir = opendir(directory.c_str())) != NULL) 
	{
		while ((ent = readdir(dir)) != NULL) 
		{
			string fileName;
			fileName = ent -> d_name;
			if (!Tools::doesContainDot(fileName))
			{
				citiesName.push_back(fileName);
				cityCount ++ ;
			}
		}
		closedir (dir);
	} 
	else 
	{
		cerr << "could not open directory" << endl;
	}
}

void State::createUnnamedPipes()
{
	for(int i = 0; i < cityCount; i++)
	{
		int fd[2];
		if(pipe(fd) < 0)
		{
			cerr << "City's Pipe construction failed " << i << endl ;
			return;
		}
		else
		{
			vector <int> fds;
			fds.push_back(fd[0]);
			fds.push_back(fd[1]);
			unnamedPipes.push_back(fds);
		}
	}
}

void State::fillUnnamedPipes()
{
	for(int i = 0; i < cityCount; i++)
	{
		string message = directory;
		message += DIR_SEPERATOR;
		message += citiesName[i];
		message += SPACE;
		message += command;
		write(unnamedPipes[i][WRITE], message.c_str(), message.size());
		close(unnamedPipes[i][WRITE]);
	}
}

void State::forkCities()
{
	for(int i = 0; i < cityCount; i++)
	{
		pid_t pid = fork();

		if(pid < 0)
		{
			cerr << "Process construction failed " << endl;
		}

		else if(pid == 0)
		{
			//cout << "process " << i << " created!" << endl;
			runCity(i);
		}

		else
		{
			close(unnamedPipes[i][READ]);
			cities.push_back(pid);
		}
	}
}

void State::runCity(int i)
{
	close(unnamedPipes[i][WRITE]);

	char * argv[3];
	argv[0] = (char*) CITY_EXEC_PATH;
	argv[1] = (char*) to_string(unnamedPipes[i][READ]).c_str();
	argv[2] = NULL;

	execv(argv[0], argv);
}

void State::createNamedPipes()
{
	for(int i = 0; i < cityCount; i++)
	{
		string cityFifoPath = Tools::getNamedFifoName(to_string(cities[i]));
		mkfifo(cityFifoPath.c_str(), 0666);
	}
}

void State::getCitiesData() 
{
	resultStr = EMPTY_STRING;
	for (int i = 0 ; i < cityCount ; i++)
	{
		resultStr += this -> getCityData(i);
		waitpid(cities[i], NULL, 0);
		resultStr += SPACE;
	}
	
	result = Tools::splitSpace(resultStr);   
}

string State::getCityData(int i)
{
	string cityFifoPath = Tools::getNamedFifoName(to_string(cities[i]));

	int fd = open(cityFifoPath.c_str(), O_RDONLY);

	char buff[MAX_BUFF];
	int n = read(fd, buff, MAX_BUFF);

	buff[n] = '\0';
	close(fd);

	string buffStr = buff;
	return buffStr;
}


void State::sendContentsToW()
{
	string message = EMPTY_STRING;

	if (result.size() == 0)
	{
		message = to_string(-1);    
	}
	else
	{   
		for (int i = 0; i < result.size(); ++i)
		{
			message += result[i];
			message += SPACE; 
		}
	}

	int fd = open(fifoPath.c_str(), O_WRONLY | O_CREAT, 0666);
	write(fd, message.c_str(), message.size());
	close(fd);
}

State::~State()
{
	for(int i = 0 ; i < cityCount ; i++)
		waitpid(cities[i], NULL, 0);
}