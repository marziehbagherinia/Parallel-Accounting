#include "City.h"

City::City(int pipeFd)
{
	char buff[MAX_BUFF];
	read(pipeFd, buff, MAX_BUFF);
	close(pipeFd);

	string helper = buff;
	
	vector<string> v = Tools::splitSpace(helper);
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

	storeCount = 0;
	fifoPath = Tools::getNamedFifoName(to_string(getpid()));
}

void City::setStores()
{
	DIR *dir;
	struct dirent *ent;

	if ((dir = opendir(directory.c_str())) != NULL) 
	{
		while ((ent = readdir(dir)) != NULL) 
		{
			string fileName;
			fileName = ent -> d_name;
		
			size_t found;
			found = fileName.find(DATABASE_FORMAT);
			
			if (found != string::npos) 
			{
				storesName.push_back(fileName);   
				storeCount ++ ;
			}
		}
		closedir (dir);
	} 
	else 
	{
		cerr << "could not open directory" << endl;
	}
}

void City::createUnnamedPipes()
{
	for(int i = 0; i < storeCount; i++)
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

void City::fillUnnamedPipes()
{
	for(int i = 0; i < storeCount; i++)
	{
		string message = directory;
		message += DIR_SEPERATOR;
		message += storesName[i];
		message += SPACE;
		message += command;
		write(unnamedPipes[i][WRITE], message.c_str(), message.size());
		close(unnamedPipes[i][WRITE]);
	}
}

void City::forkStores()
{
	for(int i = 0; i < storeCount; i++)
	{
		pid_t pid = fork();

		if(pid < 0)
		{
			cerr << "Process construction failed " << endl;
		}

		else if(pid == 0)
		{
			runStore(i);
		}

		else
		{
			close(unnamedPipes[i][READ]);
			stores.push_back(pid);
		}
	}
}

void City::runStore(int i)
{
	close(unnamedPipes[i][WRITE]);

	char * argv[3];
	argv[0] = (char*) STORE_EXEC_PATH;
	argv[1] = (char*) to_string(unnamedPipes[i][READ]).c_str();
	argv[2] = NULL;

	execv(argv[0], argv);
}

void City::createNamedPipes()
{
	for(int i = 0; i < storeCount; i++)
	{
		string cityFifoPath = Tools::getNamedFifoName(to_string(stores[i]));
		mkfifo(cityFifoPath.c_str(), 0666);
	}
}

void City::getStoresData()
{
	resultStr = EMPTY_STRING;

	for (int i = 0 ; i < storeCount ; i++)
	{
		resultStr += this -> getStoreData(i);
		waitpid(stores[i], NULL, 0);
		resultStr += SPACE;
	}

	result = Tools::splitSpace(resultStr);
}

string City::getStoreData(int i)
{
	int fd, n;
	char buff[MAX_BUFF];
	string cityFifoPath, buffStr;

	cityFifoPath = Tools::getNamedFifoName(to_string(stores[i]));

	fd = open(cityFifoPath.c_str(), O_RDONLY);
	n = read(fd, buff, MAX_BUFF);

	buff[n] = '\0';
	close(fd);

	buffStr = buff;
	return buffStr;
}


void City::sendContentsToS()
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

City::~City()
{
	for(int i = 0 ; i < storeCount ; i++)
		waitpid(stores[i], NULL, 0);
}