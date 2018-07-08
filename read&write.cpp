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



void read(){
	/*功能: 从文件中读出字符(read file1)*/
	int i, start, num, inum;
	if (argc != 2){
		printf("command read must have one object. \n");
		return;
	}
	open(1,argv[1]);
	for (i = 0; i < FILENUM; i++)
		if ((file_array[i].inum > 0) &&
			!strcmp(file_array[i].file_name, argv[1]))
			break;
	inum = file_array[i].inum;
	//printf("The length of %s:%d.\n", argv[1], inode_array[inum].length);
	if (inode_array[inum].length > 0){
		read_blk(inum);
		for (i = 0; temp[i] != '\0'; i++)
			printf("%c", temp[i]);
		printf("\n");
	}
	close(argv[1]);
}


void write(){
	/*功能: 向文件中写入字符(write file1)*/
	int i, inum, length;
	if (argc != 2){
		printf("Command write must have one args. \n");
		return;
	}
	open(3, argv[1]);
	for (i = 0; i < FILENUM; i++)
		if ((file_array[i].inum>0) &&
			!strcmp(file_array[i].file_name, argv[1])) break;
	inum = file_array[i].inum;
	//printf("The length of %s:%d\n", inode_array[inum].file_name, inode_array[inum].length);
	if (inode_array[inum].length == 0){
		inode_array[inum].length = 1024;
		inode_array[inum].address[0] = get_blknum();    //一次分配只有512B的空间，所以调用2次get_blknum()获取1024B空间
		inode_array[inum].address[1] = get_blknum();
		save_inode(inum);
		printf("Input the data(Enter to end):\n");
		gets(temp);
		write_blk(inum);
	}
	else
		printf("This file has content,it can't be written.\n");
	close(argv[1]);
}
