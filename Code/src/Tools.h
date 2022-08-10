#ifndef _TOOLS_H
#define _TOOLS_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h> 
#include <sys/wait.h> 
#include <fcntl.h>
#include <unistd.h>
#include <algorithm>
#include <bits/stdc++.h>
#include <dirent.h>

using namespace std;

#define READ      0
#define WRITE     1
#define NULL_CHAR 0
#define MAX_BUFF  4096

#define DOT           '.'
#define COMMA         ','
#define SPACE         ' '
#define UNDERLINE     '_'
#define EMPTY_STRING  ""
#define DIR_SEPERATOR '/'

#define MAX "MAX"
#define MIN "MIN"
#define DATABASE_FORMAT  ".csv"
#define FIFO_FILE_PATH 	 "./namedPipe/fifo"
#define STORE_EXEC_PATH  "bin/Store"
#define CITY_EXEC_PATH   "bin/City"
#define STATE_EXEC_PATH  "bin/State"
#define WORKER_EXEC_PATH "bin/Worker"

class Tools
{
public:
	static bool doesContainDot(string line);

	static int findMaximum(vector <int> input);
	static int findMinimum(vector <int> input);

	static string getNamedFifoName(string weightVectorsName);
	static string removeAllSpaces(string);
	static string vectorToString(const vector <int> &vec);

	static vector <int>    splitSlash(string date);
	static vector <string> splitSpace(string date);

	static void splitCommand(string dates, int &id, string &start_date, string &end_date);
};

#endif