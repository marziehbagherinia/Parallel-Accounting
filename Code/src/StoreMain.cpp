#include "Store.h"

int main(int argc, char const *argv[])
{
	int unnamedPipeFd = atoi(argv[1]);
    Store store(unnamedPipeFd);
    store.readInstance();
    store.find_price();
    store.sendContentsToC();      
}