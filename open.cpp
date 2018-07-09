void open(int mymode, string argv1) {
	/*功能: 打开当前目录下的文件(open file1)*/
	int i, inum, mode, filenum, chk;
	for (i = 0; i < INODENUM; i++)
		if ((inode_array[i].inum > 0) &&
			(inode_array[i].type == '-') &&
			argv1 == inode_array[i].file_name &&
			!strcmp(inode_array[i].user_name, user.user_name) &&
			inode_array[i].iparent == inum_cur)     //判断是否在当前目录下 
			break;
	if (i == INODENUM) {
		cout << "This is no " + argv1 + " file...\n";
		return;
	}
	inum = i;
	chk = check(i);               //检查该文件是否为当前用户的文件
	mode = mymode;
	if (chk != 1) {
		printf("This file is not yours !\n");
		return;
	}
	if ((mode < 1) || (mode > 3)) {
		printf("Open mode is wrong.\n");
		return;
	}
	filenum = i;
	file_array[filenum].inum = inum;
	strcpy(file_array[filenum].file_name, inode_array[inum].file_name);
	file_array[filenum].mode = mode;
	file_array[filenum].offset = 0;
}
