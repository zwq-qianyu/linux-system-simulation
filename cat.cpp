#include "head.h"
//int argc;		// 用户命令的参数个数
//char *argv[5];		// 用户命令的参数
Inode inode_array[INODENUM];	// i节点数组
char temp[2 * BLKSIZE];	// 缓冲区
File_table file_array[FILENUM];	// 打开文件表数组

vector<string>vc_of_str;    // 用户命令的参数个数
string  s1, s2;            // 用户命令的参数

void cat(){
	/*功能: 从文件中读出字符(read file1)*/
	int i, start, num, inum;
	/*if (argc != 2){
		printf("command cat must have one object. \n");
		return;
	}*/
	open(1,s2);
	for (i = 0; i < FILENUM; i++)
		if ((file_array[i].inum > 0) &&
			(s2 == file_array[i].file_name))
			break;
	inum = file_array[i].inum;
	//printf("The length of %s:%d.\n", argv[1], inode_array[inum].length);
	if (inode_array[inum].length > 0){
		read_blk(inum);
		for (i = 0; temp[i] != '\0'; i++)
			printf("%c", temp[i]);
		printf("\n");
	}
	close(s2);
}
