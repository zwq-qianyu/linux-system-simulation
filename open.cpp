#include "head.h"
Inode inode_array[INODENUM];	// i节点数组
File_table file_array[FILENUM];	// 打开文件表数组

void open(int mymode, char *argv1){
	/*功能: 打开当前目录下的文件(open file1)*/
	int i, inum, mode, filenum, chk;
	for (i = 0; i < INODENUM; i++)
		if ((inode_array[i].inum > 0) &&
			(inode_array[i].type == '-') &&
			!strcmp(inode_array[i].file_name, argv1))
			break;
	if (i == INODENUM){
		printf("This is no %s file...\n",argv1);
		return;
	}
	inum = i;
	chk = check(i);               //检查该文件是否为当前用户的文件
	mode = mymode; 
	if (chk != 1){
		printf("This file is not yours !\n");
		return;
	}
	if ((mode < 1) || (mode > 3)){
		printf("Open mode is wrong.\n");
		return;
	}
	filenum = i;
	file_array[filenum].inum = inum;
	strcpy(file_array[filenum].file_name, inode_array[inum].file_name);
	file_array[filenum].mode = mode;
	file_array[filenum].offset = 0;
}
