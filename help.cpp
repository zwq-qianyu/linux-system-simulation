#include "head.h"

int main(){
	help();
	return 0;
}
void help(){
	/*功能: 显示帮助命令*/
	printf("command: \n\
	help   ---  show help menu \n\
	clear  ---  clear the screen \n\
	cd     ---  change directory \n\
	mkdir  ---  make directory   \n\
	touch  ---  create a new file \n\
	cat    ---  open and read a exist file \n\
	vi     ---  open and write something to a file \n\
	close  ---  close a file \n\
	rm     ---  delete a exist file \n\
	rm -r  ---  delete a directory \n\
	format ---  format a exist filesystem \n\
	su     ---  switch current user \n\
	exit   ---  exit this system \n");
}
