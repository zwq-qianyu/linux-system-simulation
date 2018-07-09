void vi() {
	/*功能: 向文件中写入字符(write file1)*/
	int i, inum, length;
	//length = 0;
	/*if (argc != 2){
		printf("Command write must have one args. \n");
		return;
	}*/
	open(3, s2);
	for (i = 0; i < FILENUM; i++)
		if ((file_array[i].inum>0) &&
			s2==file_array[i].file_name) break;
	if (i == FILENUM){
		cout << "Open " << s2 << " false.\n";
		return;
	}
	inum = file_array[i].inum;
	//printf("The length of %s:%d\n", inode_array[inum].file_name, inode_array[inum].length);
	if (inode_array[inum].length == 0){
		printf("The length you want to write(0-1024):");
		scanf("%d", &length);
		gets(temp);
		if ((length < 0) && (length >1024)){
			printf("You can't creat a file which data less than 0 byte or more than 1024 bytes.'.\n");
			return;
		}
		inode_array[inum].length = length;
		inode_array[inum].address[0] = get_blknum();
		if (length > 512)
			inode_array[inum].address[1] = get_blknum();
		save_inode(inum);
		printf("Input the data(Enter to end):\n");
		gets(temp);
		write_blk(inum);
	}
	else{
		printf("Are you sure to write this file? Data in it will all be deleted.(y/n):");
		scanf("%c", &choice);
		if ((choice == 'y') || (choice == 'Y')){
			printf("The length you want to write(0-1024):");
			scanf("%d", &length);
			gets(temp);
			if ((length < 0) && (length >1024)){
				printf("You can't creat a file which data less than 0 byte or more than 1024 bytes.'.\n");
				return;
			}
			inode_array[inum].length = length;
			inode_array[inum].address[0] = get_blknum();
			if (length > 512)
				inode_array[inum].address[1] = get_blknum();
			save_inode(inum);
			printf("Input the data(Enter to end):\n");
			gets(temp);
			write_blk(inum);
		}
	}
	close(s2);
}
