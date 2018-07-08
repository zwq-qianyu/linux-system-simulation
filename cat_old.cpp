#include "head.h"
int argc;		// 用户命令的参数个数
char *argv[5];		// 用户命令的参数
Inode inode_array[INODENUM];	// i节点数组
char temp[2 * BLKSIZE];	// 缓冲区
 
void cat(){
	/*功能: 打开并查看当前目录下的文件(open file1)*/
	int i, inum, chk;
	if (argc != 2){         //若没有输入要操作的文件或目录对象，直接退出 
		printf("command open must have one object. \n");
		return;
	}
	for (i = 0; i < INODENUM; i++)
		if ((inode_array[i].inum > 0) &&
			(inode_array[i].type == '-') &&                //'-'代表普通文件 
			!strcmp(inode_array[i].file_name, argv[1]))    //argv[1]==该文件名    因为str1==str2时，strcmp=0 
			break;                    //如果是普通文件不用open，直接 read 
	//如果上面的for循环没有扫描到该文件，表示文件不存在
	if (i == INODENUM){
		printf("The file you want to open doesn't exsited.\n");
		return;
	}
	inum = i;
	chk = check(i);     //调用check()函数，判断是否是目前登陆用户的文件 
	if (chk != 1)
	{
		printf("This file is not yours !\n");
		return;
	}
	if (inode_array[inum].length > 0){
		read_blk(inum);                 //将i节点号为inum的文件读入temp缓存
		for (i = 0; temp[i] != '\0'; i++)    //读取空格之前的所有内容 
			printf("%c", temp[i]);
		printf("\n");
	}
} 
