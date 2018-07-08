#include "head.h"

int main(){
	help();
	return 0;
}
void help(){
	/*功能: 显示帮助命令*/
	printf("command: \n\
	help   ---  show help menu \n\
	cd     ---  change directory \n\
	clear  ---  clear the screen \n\
	ls     ---  show all the files and directories in particular directory \n\
	mkdir  ---  make a new directory   \n\
	touch  ---  create a new file \n\
	cat    ---  open and read an exist file \n\
	vi     ---  open and write something to a particular file \n\
	rm     ---  delete a exist file \n\
	su     ---  switch current user \n\
	clear  ---  clear the screen \n\
	format ---  format a exist filesystem \n\
	exit   ---  exit this system \n\
	rmdir  ---  delete a directory \n\
	df     ---  show the useage of storage \n");
}
