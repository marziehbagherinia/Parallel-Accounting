#include "LoadBalancer.h"

LoadBalancer::LoadBalancer(string commandsFile, string _dirName)
{
	processCount = 0;
	dirName = _dirName;
	this->readCommands(commandsFile);
}

void LoadBalancer::readCommands(string commandsFile)
{
	string line, field; 
	vector <string> gettingLine;

	ifstream myfile(commandsFile.c_str());
	
	if(myfile.is_open())
	{
		while (getline(myfile, line))
		{
			gettingLine.clear();
			stringstream ss(line);
			while(getline(ss,field,' '))
			{
				gettingLine.push_back(field);
			}
			commands.push_back(gettingLine); 
		}
	}

	processCount = commands.size();
}

void LoadBalancer::createUnnamedPipes()
{
	for(int i = 0; i < processCount; i++)
	{
		int fd[2];
		if(pipe(fd) < 0)
		{
			cerr << "Pipe construction failed " << i << endl ;
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

void LoadBalancer :: fillUnnamedPipes()
{
	string result = dirName;

	for(int i = 0; i < processCount; i++)
	{
		string result = dirName;
		for (int j = 0; j < commands[i].size(); ++j)
		{
			result += " ";
			result += commands[i][j];
		}
		write(unnamedPipes[i][WRITE], result.c_str(), result.size());
		close(unnamedPipes[i][WRITE]);
	}
}

void LoadBalancer::forkWorkers()
{
	for(int i = 0; i < processCount; i++)
	{
		pid_t pid = fork();

		if(pid < 0)
		{
			cerr << "Process construction failed " << endl;
		}
		else if(pid == 0)
		{
			//cout << "process " << i << " created!" << endl;
			runWorker(i);
			close(unnamedPipes[i][READ]);
		}
		else
		{
			close(unnamedPipes[i][READ]);
			workers.push_back(pid);
		}
	}
}

void LoadBalancer::runWorker(int i)
{
	close(unnamedPipes[i][WRITE]);

	char * argv[3];
	argv[0] = (char*) WORKER_EXEC_PATH;
	argv[1] = (char*) to_string(unnamedPipes[i][READ]).c_str();
	argv[2] = NULL;

	execv(argv[0], argv);
}

void LoadBalancer::createNamedPipes()
{
	for(int i = 0; i < processCount; i++)
	{
		string workerFifoPath = Tools::getNamedFifoName(to_string(workers[i]));
		mkfifo(workerFifoPath.c_str(), 0666);
	}
}

void LoadBalancer::getWorkersData() 
{
	for (int i = 0; i < processCount; ++i)
	{
		answer.push_back("");
	}

	for (int i = 0 ; i < processCount ; i++)
	{
		answer[i] = this -> getWorkerData(i);
		waitpid(workers[i], NULL, 0);
	}   
}

string LoadBalancer::getWorkerData(int i)
{
	int fd, n;
	string fifoPath, buffStr;
	char buff[MAX_BUFF];
	
	fifoPath = Tools::getNamedFifoName(to_string(workers[i]));
	fd = open(fifoPath.c_str(), O_RDONLY);
	n = read(fd, buff, MAX_BUFF);
	buff[n] = '\0';
	close(fd);

	buffStr = buff;
	return buffStr;
}

void LoadBalancer::showResult()
{
	if (processCount == 0)
	{
		cout << "-1" << endl;
	}

	for (int i = 0; i < processCount; ++i)
	{
		cout << answer[i] << endl;
	}
}

LoadBalancer::~LoadBalancer()
{
	for(int i = 0 ; i < processCount ; i++)
		waitpid(workers[i], NULL, 0);
}