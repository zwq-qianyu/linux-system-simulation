void close(char *argv1){
	/*功能: 关闭已经打开的文件(close file1)*/
	int i;
	for (i = 0; i < FILENUM; i++)
		if ((file_array[i].inum > 0) &&
			!strcmp(file_array[i].file_name, argv1)) break;
	if (i == FILENUM){
		printf("This file didn't be opened.\n");
		return;
	}
	else
		file_array[i].inum = -1;
}
