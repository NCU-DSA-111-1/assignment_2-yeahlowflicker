#include <stdio.h>
#include <stdlib.h>
#include <ev.h>
#include <unistd.h> 

#include "shogi.h"
#include "replay.h"

int main(int argc, char** argv) {

	int nFlag = 0;
	int sFlag = 0;
	int lFlag = 0;
	int opt = 0;
	char* fileName = NULL;

	while((opt = getopt(argc, argv, "l:ns:")) != -1) {
		switch (opt) {
			case 'l': lFlag = 1; fileName = optarg; break;

			case 'n': nFlag = 1; break;
			case 's': sFlag = 1; fileName = optarg; break;
			case '?': return 1;
		}
	}

	if (nFlag && sFlag && fileName != NULL)
		ShogiGame(fileName);
	else if (lFlag && fileName != NULL)
		ShogiReplay(fileName);
    
	return 0;
}
