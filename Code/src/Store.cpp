#include "Store.h"

Store::Store(int pipeFd)
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

	while (!isdigit(command[command.size() - 1]))
	{
		command.erase(command.begin() + command.size() - 1);
	}

	Tools :: splitCommand(command, id, start_date, end_date);

	fifoPath = Tools::getNamedFifoName(to_string(getpid()));
}

void Store::readInstance()
{
	string line, field, firstLine;  
	ifstream myfile(directory.c_str(), ios::out);
	int i = 0;

	if (myfile.is_open())
	{
		getline(myfile, firstLine);
		while (getline(myfile,line))
		{
			i = 0;
			stringstream ss(line);   
			while(getline(ss,field,COMMA))
			{
				if (i == 0)
				{
					dates.push_back(Tools::splitSlash(field));
				}
				else if (i == 1)
				{
					product_id.push_back(stoi(field));
				}
				else if (i == 2)
				{
					price.push_back(stoi(field));
				}
				i++;
			}
		}
	}
}

bool Store::is_valid_date(vector <int> date)
{
	vector <int> start_vec = Tools::splitSlash(start_date);
	vector <int> end_vec = Tools::splitSlash(end_date);

	if (date.size() >= 3 && start_vec.size() >= 3 && end_vec.size() >= 3)
	{
		if (start_vec[0] <= date[0] && date[0] <= end_vec[0])
		{
			if (start_vec[0] < date[0] && date[0] < end_vec[0])
			{
				return true;
			}
			if (start_vec[0] == date[0] && date[0] < end_vec[0])
			{
				if (start_vec[1] <= date[1])
				{
					if (start_vec[1] < date[1])
					{
						return true;
					}
					if (start_vec[1] == date[1])
					{
						if (start_vec[2] <= date[2])
							return true;
						else
							return false;
					}
				}
				else
				{
					return false;
				}
			}
			if (start_vec[0] < date[0] && date[0] == end_vec[0])
			{
				if (date[1] <= end_vec[1])
				{
					if (date[1] < end_vec[1])
					{
						return true;
					}
					if (date[1] == end_vec[1])
					{
						if (date[2] <= end_vec[2])
							return true;
						else
							return false;
					}
				}
				else
				{
					return false;
				}
			}
			if (start_vec[0] == date[0] && date[0] == end_vec[0])
			{
				if (start_vec[1] <= date[1] && date[1] <= end_vec[1])
				{
					if (start_vec[1] < date[1] && date[1] < end_vec[1])
					{
						return true;
					}
					else if (start_vec[1] == date[1] && date[1] < end_vec[1])
					{
						if (start_vec[2] <= date[2])
							return true;
						else
							return false;
					}
					else if (start_vec[1] < date[1] && date[1] == end_vec[1])
					{
						if (date[2] <= end_vec[2])
							return true;
						else
							return false;
					}
					else if (start_vec[1] == date[1] && date[1] == end_vec[1])
					{
						if (start_vec[2] <= date[2] && date[2] <= end_vec[2])
							return true;
						else
							return false;
					}                                                       
				}
				else
				{
					return false;
				}
			}

		}
		else
		{
			return false;
		}
	}

	return false;
}

void Store::find_price()
{
	for (int i = 0; i < product_id.size(); ++i)
	{
		if (id == product_id[i])
		{
			if (is_valid_date(dates[i]))
			{
				result.push_back(price[i]);
			}
		}
	}
}


void Store::sendContentsToC()
{
	string message;

	if (result.size() == 0)
	{
		message = to_string(-1);    
	}
	else
	{
		message = EMPTY_STRING;
		
		for (int i = 0; i < result.size(); ++i)
		{
			message += to_string(result[i]);
			message += SPACE; 
		}
	}

	int fd = open(fifoPath.c_str(), O_WRONLY | O_CREAT, 0666);
	write(fd, message.c_str(), message.size());
	close(fd);
}

Store::~Store()
{
	exit(0);
}