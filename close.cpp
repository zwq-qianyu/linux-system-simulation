#include "head.h"
File_table file_array[FILENUM];	// 打开文件表数组

void close(string argv1){
	/*功能: 关闭已经打开的文件(close file1)*/
	int i;
	for (i = 0; i < FILENUM; i++)
		if ((file_array[i].inum > 0) &&
			(argv1 == file_array[i].file_name)) break;
	if (i == FILENUM){
		printf("This file didn't be opened.\n");
		return;
	}
	else
		file_array[i].inum = -1;
}
