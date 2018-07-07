void open(int mymode, char *argv1){
	/*功能: 打开当前目录下的文件(open file1)*/
	int i, inum, filenum, chk;
	/*if (argc != 2)
	{
		printf("command open must have one args. \n");
		return;
	}*/
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
	if (chk != 1){
		printf("This file is not yours !\n");
		return;
	}
	//printf("Please input open mode:(1: read, 2: write, 3: read and write):");
	//scanf("%d", &mode);
	//gets(temp);
	//mode = mymode
	if ((mode < 1) || (mode > 3)){
		printf("Open mode is wrong.\n");
		return;
	}
	/*
	for (i = 0; i < FILENUM; i++)
		if (file_array[i].inum < 0) break;
	if (i == FILENUM){
		printf("The file table is full, please close some file.\n");
		return;
	}*/
	filenum = i;
	file_array[filenum].inum = inum;
	strcpy(file_array[filenum].file_name, inode_array[inum].file_name);
	file_array[filenum].mode = mymode;
	file_array[filenum].offset = 0;
	/*printf("Open file %s by ", file_array[filenum].file_name);
	if (mode == 1) printf("read only.\n");
	else if (mode == 2) printf("write only.\n");
	else printf("read and write.\n");*/
}


void close(char *argv1){
	/*功能: 关闭已经打开的文件(close file1)*/
	int i;
	/*if (argc != 2)
	{
		printf("Command close must have one args. \n");
		return;
	}*/
	for (i = 0; i < FILENUM; i++)
		if ((file_array[i].inum > 0) &&
			!strcmp(file_array[i].file_name, argv1)) break;
	if (i == FILENUM){
		printf("This file didn't be opened.\n");
		return;
	}
	else{
		file_array[i].inum = -1;
		//printf("Close %s successful!\n", argv1);
	}
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
	/*if (i == FILENUM){
		printf("Open %s first.\n", argv[1]);
		return;
	}
	else if (file_array[i].mode == 2){
		printf("Can't read %s.\n", argv[1]);
		return;
	}*/
	inum = file_array[i].inum;
	//printf("The length of %s:%d.\n", argv[1], inode_array[inum].length);
	if (inode_array[inum].length > 0){
		/*
		printf("The start position:");
		scanf("%d", &start);
		gets(temp);
		if ((start<0) || (start >= inode_array[inum].length)){
			printf("Start position is wrong.\n");
			return;
		}
		printf("The bytes you want to read:");
		scanf("%d", &num);
		gets(temp);
		if (num <= 0){
			printf("The num you want to read is wrong.\n");
			return;
		}
		*/
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
	/*if (i == FILENUM){
		printf("Open %s first.\n", argv[1]);
		return;
	}
	else if (file_array[i].mode == 1){
		printf("Can't write %s.\n", argv[1]);
		return;
	}*/
	inum = file_array[i].inum;
	//printf("The length of %s:%d\n", inode_array[inum].file_name, inode_array[inum].length);
	if (inode_array[inum].length == 0){
		/*printf("The length you want to write(0-1024):");
		scanf("%d", &length);
		gets(temp);
		if ((length < 0) && (length >1024)){
			printf("Input wrong.\n");
			return;
		}*/
		inode_array[inum].length = 1024;
		inode_array[inum].address[0] = get_blknum();
		inode_array[inum].address[1] = get_blknum();
		/*if (length > 512)
			inode_array[inum].address[1] = get_blknum();*/
		save_inode(inum);
		printf("Input the data(Enter to end):\n");
		gets(temp);
		write_blk(inum);
	}
	else
		printf("This file has content,it can't be written.\n");
	close(argv[1]);
}

