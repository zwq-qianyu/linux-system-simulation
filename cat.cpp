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
