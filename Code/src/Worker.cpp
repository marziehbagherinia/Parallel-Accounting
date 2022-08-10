#include "Worker.h"

Worker::Worker(int pipeFd)
{
	char buff[MAX_BUFF];
	read(pipeFd, buff, MAX_BUFF);
	close(pipeFd);

	string helper = buff;
		
	vector <string> v = Tools::splitSpace(helper);
	
	if (v.size() >= 5)
	{
		directory = v[0];
		type = v[1];
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

	stateCount = 0;
	answer = "";
	fifoPath = Tools::getNamedFifoName(to_string(getpid()));
}

void Worker::setStates()
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
				statesName.push_back(fileName);
				stateCount ++ ;
			}
		}
		closedir (dir);
	} 
	else 
	{
		cerr << "could not open directory" << endl;
	}
}

void Worker::createUnnamedPipes()
{
	for(int i = 0; i < stateCount; i++)
	{
		int fd[2];
		if(pipe(fd) < 0)
		{
			cout << "State's Pipe construction failed " << i << endl ;
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

void Worker::fillUnnamedPipes()
{
	for(int i = 0; i < stateCount; i++)
	{
		string message = directory;
		message += "/";
		message += statesName[i];
		message += " ";
		message += command;
		write(unnamedPipes[i][WRITE], message.c_str(), message.size());
		close(unnamedPipes[i][WRITE]);
	}
}

void Worker::forkStates()
{
	for(int i = 0; i < stateCount; i++)
	{
		pid_t pid = fork();

		if(pid < 0)
		{
			cerr << "Process construction failed " << endl;
		}

		else if(pid == 0)
		{
			//cout << "process " << i << " created!" << endl;
			runState(i);
		}

		else
		{
			close(unnamedPipes[i][READ]);
			states.push_back(pid);
		}
	}
}

void Worker::runState(int i)
{
	close(unnamedPipes[i][WRITE]);

	char * argv[3];
	argv[0] = (char*) STATE_EXEC_PATH;
	argv[1] = (char*) to_string(unnamedPipes[i][READ]).c_str();
	argv[2] = NULL;

	execv(argv[0], argv);
}

void Worker::createNamedPipes()
{
	for(int i = 0; i < stateCount; i++)
	{
		string stateFifoPath = Tools::getNamedFifoName(to_string(states[i]));
		mkfifo(stateFifoPath.c_str(), 0666);
	}
}

void Worker::getStatesData() 
{
	resultStr = "";
	for (int i = 0 ; i < stateCount ; i++)
	{
		resultStr += this -> getStateData(i);
		waitpid(states[i], NULL, 0);
		resultStr += " ";
	}
	
	vector <string> v = Tools::splitSpace(resultStr);
	
	for (int i = 0; i < v.size(); ++i)
	{
		result.push_back(stoi(v[i]));
	}  
}

string Worker::getStateData(int i)
{
	int fd, n;
	char buff[MAX_BUFF];
	string stateFifoPath, buffStr;
	
	stateFifoPath = Tools::getNamedFifoName(to_string(states[i]));
	fd = open(stateFifoPath.c_str(), O_RDONLY);
	n = read(fd, buff, MAX_BUFF);

	buff[n] = '\0';
	close(fd);

	buffStr = buff;
	return buffStr;
}

void Worker::provideAnswer()
{
	if (type == MAX)
		answer = to_string(Tools::findMaximum(result));
	else if (type == MIN)
		answer = to_string(Tools::findMinimum(result));
}

void Worker::sendContentsToLB()
{
	int fd = open(fifoPath.c_str(), O_WRONLY | O_CREAT, 0666);
	write(fd, answer.c_str() , answer.size());
	close(fd);
}

Worker::~Worker()
{
	for(int i = 0 ; i < stateCount ; i++)
		waitpid(states[i], NULL, 0);
}