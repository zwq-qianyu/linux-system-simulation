#include "stdafx.h"
#include "head.h"
char		choice;
int		argc;		// 用户命令的参数个数
char		*argv[5];		// 用户命令的参数
int		inum_cur;		// 当前目录
char		temp[2 * BLKSIZE];	// 缓冲区
User		user;		// 当前的用户
char		bitmap[BLKNUM];	// 位图数组
Inode	inode_array[INODENUM];	// i节点数组
File_table file_array[FILENUM];	// 打开文件表数组
char	image_name[10] = "hd.dat";	// 文件系统名称
FILE		*fp;					// 打开文件指针

									//创建映像hd，并将所有用户和文件清除
void format(void)
{
	FILE  *fp;
	int   i;
	Inode inode;
	printf("Will be to format filesystem...\n");
	printf("WARNING:ALL DATA ON THIS FILESYSTEM WILL BE LOST!\n");
	printf("Proceed with Format(Y/N)?");
	scanf("%c", &choice);
	gets_s(temp);
	if ((choice == 'y') || (choice == 'Y'))
	{
		if ((fp = fopen(image_name, "w+b")) == NULL)
		{
			printf("Can't create file %s\n", image_name);
			exit(-1);
		}
		for (i = 0; i < BLKSIZE; i++)
			fputc('0', fp);
		inode.inum = 0;
		strcpy(inode.file_name, "/");
		inode.type = 'd';
		strcpy(inode.user_name, "all");
		inode.iparent = 0;
		inode.length = 0;
		inode.address[0] = -1;
		inode.address[1] = -1;
		fwrite(&inode, sizeof(Inode), 1, fp);
		inode.inum = -1;
		for (i = 0; i < 31; i++)
			fwrite(&inode, sizeof(Inode), 1, fp);
		for (i = 0; i < BLKNUM*BLKSIZE; i++)
			fputc('\0', fp);
		fclose(fp);
		// 打开文件user.txt
		if ((fp = fopen("user.txt", "w+")) == NULL)
		{
			printf("Can't create file %s\n", "user.txt");
			exit(-1);
		}
		fclose(fp);
		printf("Filesystem created successful.Please first login!\n");
	}
	return;
}
// 功能: 用户登陆，如果是新用户则创建用户
void login(void)
{
	char *p;
	int  flag;
	char user_name[10];
	char password[10];
	char file_name[10] = "user.txt";
	do
	{
		printf("login:");
		gets_s(user_name);
		printf("password:");
		p = password;
		while (*p = _getch())
		{
			if (*p == 0x0d) //当输入回车键时，0x0d为回车键的ASCII码
			{
				*p = '\0'; //将输入的回车键转换成空格
				break;
			}
			printf("*");   //将输入的密码以"*"号显示
			p++;
		}
		flag = 0;
		if ((fp = fopen(file_name, "r+")) == NULL)
		{
			printf("\nCan't open file %s.\n", file_name);
			printf("This filesystem not exist, it will be create!\n");
			format();
			login();
		}
		while (!feof(fp))
		{
			fread(&user, sizeof(User), 1, fp);
			// 已经存在的用户, 且密码正确
			if (!strcmp(user.user_name, user_name) &&
				!strcmp(user.password, password))
			{
				fclose(fp);
				printf("\n");
				return;
			}
			// 已经存在的用户, 但密码错误
			else if (!strcmp(user.user_name, user_name))
			{
				printf("\nThis user is exist, but password is incorrect.\n");
				flag = 1;
				fclose(fp);
				break;
			}
		}
		if (flag == 0) break;
	} while (flag);
	// 创建新用户
	if (flag == 0)
	{
		printf("\nDo you want to creat a new user?(y/n):");
		scanf("%c", &choice);
		gets_s(temp);
		if ((choice == 'y') || (choice == 'Y'))
		{
			strcpy(user.user_name, user_name);
			strcpy(user.password, password);
			fwrite(&user, sizeof(User), 1, fp);
			fclose(fp);
			return;
		}
		if ((choice == 'n') || (choice == 'N'))
			login();
	}
}
// 功能: 将所有i节点读入内存
void init(void)
{
	int   i;
	if ((fp = fopen(image_name, "r+b")) == NULL)
	{
		printf("Can't open file %s.\n", image_name);
		exit(-1);
	}
	// 读入位图
	for (i = 0; i < BLKNUM; i++)
		bitmap[i] = fgetc(fp);
	// 显示位图
	// 读入i节点信息
	for (i = 0; i < INODENUM; i++)
		fread(&inode_array[i], sizeof(Inode), 1, fp);
	// 显示i节点
	// 当前目录为根目录
	inum_cur = 0;
	// 初始化打开文件表
	for (i = 0; i < FILENUM; i++)
		file_array[i].inum = -1;
}
// 功能: 分析用户命令, 将分析结果填充argc和argv
// 结果: 0-14为系统命令, 15为命令错误
int analyse(char *str)
{
	int  i;
	char temp[20];
	char *ptr_char;
	const char  *syscmd[] = { "help", "cd", "dir", "mkdir", "creat", "open", "read", "write", "close", "delete", "logout", "clear","format","quit","rd" };
	argc = 0;
	for (i = 0, ptr_char = str; *ptr_char != '\0'; ptr_char++)
	{
		if (*ptr_char != ' ')
		{
			while (*ptr_char != ' ' && (*ptr_char != '\0'))
				temp[i++] = *ptr_char++;
			argv[argc] = (char *)malloc(i + 1);
			strncpy(argv[argc], temp, i);
			argv[argc][i] = '\0';
			argc++;
			i = 0;
			if (*ptr_char == '\0') break;
		}
	}
	if (argc != 0)
	{
		for (i = 0; (i < 15) && strcmp(argv[0], syscmd[i]); i++);
		return i;
	}
	else return 15;
}
// 功能: 将num号i节点保存到hd.dat
void save_inode(int num)
{
	if ((fp = fopen(image_name, "r+b")) == NULL)
	{
		printf("Can't open file %s\n", image_name);
		exit(-1);
	}
	fseek(fp, 512 + num * sizeof(Inode), SEEK_SET);
	fwrite(&inode_array[num], sizeof(Inode), 1, fp);
	fclose(fp);
}
// 功能: 申请一个数据块
int get_blknum(void)
{
	int i;
	for (i = 0; i < BLKNUM; i++)
		if (bitmap[i] == '0') break;
	// 未找到空闲数据块
	if (i == BLKNUM)
	{
		printf("Data area is full.\n");
		exit(-1);
	}
	bitmap[i] = '1';
	if ((fp = fopen(image_name, "r+b")) == NULL)
	{
		printf("Can't open file %s\n", image_name);
		exit(-1);
	}
	fseek(fp, i, SEEK_SET);
	fputc('1', fp);
	fclose(fp);
	return i;
}
// 功能: 将i节点号为num的文件读入temp 
void read_blk(int num)
{
	int  i, len;
	char ch;
	int  add0, add1;
	len = inode_array[num].length;
	add0 = inode_array[num].address[0];
	if (len > 512)
		add1 = inode_array[num].address[1];
	if ((fp = fopen(image_name, "r+b")) == NULL)
	{
		printf("Can't open file %s.\n", image_name);
		exit(-1);
	}
	fseek(fp, 1536 + add0 * BLKSIZE, SEEK_SET);
	ch = fgetc(fp);
	for (i = 0; (i < len) && (ch != '\0') && (i < 512); i++)
	{
		temp[i] = ch;
		ch = fgetc(fp);
	}
	if (i >= 512)
	{
		fseek(fp, 1536 + add1 * BLKSIZE, SEEK_SET);
		ch = fgetc(fp);
		for (; (i < len) && (ch != '\0'); i++)
		{
			temp[i] = ch;
			ch = fgetc(fp);
		}
	}
	temp[i] = '\0';
	fclose(fp);
}
// 功能: 将temp的内容输入hd的数据区
void write_blk(int num)
{
	int  i, len;
	int  add0, add1;
	add0 = inode_array[num].address[0];
	len = inode_array[num].length;
	if ((fp = fopen(image_name, "r+b")) == NULL)
	{
		printf("Can't open file %s.\n", image_name);
		exit(-1);
	}
	fseek(fp, 1536 + add0 * BLKSIZE, SEEK_SET);
	for (i = 0; (i<len) && (temp[i] != '\0') && (i < 512); i++)
		fputc(temp[i], fp);
	if (i == 512)
	{
		add1 = inode_array[num].address[1];
		fseek(fp, 1536 + add1 * BLKSIZE, SEEK_SET);
		for (; (i < len) && (temp[i] != '\0'); i++)
			fputc(temp[i], fp);
	}
	fputc('\0', fp);
	fclose(fp);
}
// 功能: 释放文件块号为num的文件占用的空间
void release_blk(int num)
{
	FILE *fp;
	if ((fp = fopen(image_name, "r+b")) == NULL)
	{
		printf("Can't open file %s\n", image_name);
		exit(-1);
	}
	bitmap[num] = '0';
	fseek(fp, num, SEEK_SET);
	fputc('0', fp);
	fclose(fp);
}
// 功能: 显示帮助命令
void help(void)
{
	printf("command: \n\
help   ---  show help menu \n\
clear  ---  clear the screen \n\
cd     ---  change directory \n\
mkdir  ---  make directory   \n\
creat  ---  create a new file \n\
open   ---  open a exist file \n\
read   ---  read a file \n\
write  ---  write something to a file \n\
close  ---  close a file \n\
delete ---  delete a exist file \n\
format ---  format a exist filesystem \n\
logout ---  exit user \n\
rd     ---  delete a directory \n\
quit   ---  exit this system\n");
}
//设置文件路径
void pathset()
{
	char path[50];
	int m, n;
	if (inode_array[inum_cur].inum == 0)
		strcpy(path, user.user_name);
	else
	{
		strcpy(path, user.user_name);
		m = 0;
		n = inum_cur;
		while (m != inum_cur)
		{
			while (inode_array[n].iparent != m)
			{
				n = inode_array[n].iparent;
			}
			strcat(path, "/");
			strcat(path, inode_array[n].file_name);
			m = n;
			n = inum_cur;
		}
	}
	printf("[%s]@", path);
}
// 功能: 切换目录(cd .. 或者 cd dir1)
void cd(void)
{
	int i;
	if (argc != 2)
	{
		printf("Command cd must have two args. \n");
		return;
	}
	if (!strcmp(argv[1], ".."))
		inum_cur = inode_array[inum_cur].iparent;
	else
	{
		// 遍历i节点数组
		for (i = 0; i < INODENUM; i++)
			if ((inode_array[i].inum>0) &&
				(inode_array[i].type == 'd') &&
				(inode_array[i].iparent == inum_cur) &&
				!strcmp(inode_array[i].file_name, argv[1]) &&
				check(i))
				break;
		if (i == INODENUM)
			printf("This directory isn't exsited.\n");
		else inum_cur = i;
	}
}
// 功能: 显示当前目录下的子目录和文件(dir)
void dir(void)
{
	int i;
	int dcount = 0, fcount = 0;
	short bcount = 0;
	if (argc != 1)
	{
		printf("Command dir must have one args. \n");
		return;
	}
	// 遍历i节点数组, 显示当前目录下的子目录和文件名
	for (i = 0; i < INODENUM; i++)
		if ((inode_array[i].inum> 0) &&
			(inode_array[i].iparent == inum_cur))
		{
			if (inode_array[i].type == 'd' && check(i))
			{
				dcount++;
				printf("%-20s<DIR>\n", inode_array[i].file_name);
			}
			if (inode_array[i].type == '-' && check(i))
			{
				fcount++;
				bcount += inode_array[i].length;
				printf("%-20s%12d bytes\n", inode_array[i].file_name, inode_array[i].length);
			}
		}
	printf("\n                    %d file(s)%11d bytes\n", fcount, bcount);
	printf("                    %d dir(s) %11d bytes FreeSpace\n", dcount, 1024 * 1024 - bcount);
}
// 功能: 删除目录树(rd dir1)
void rd()
{
	int i, j, t, flag = 0;
	if (argc != 2)
	{
		printf("Command delete must have one args. \n");
		return;
	}
	for (i = 0; i < INODENUM; i++)//查找待删除目录
		if ((inode_array[i].inum > 0) &&//是否为空
			(inode_array[i].iparent == inum_cur) &&
			(inode_array[i].type == 'd') &&
			(!strcmp(inode_array[i].file_name, argv[1])))
		{
			int chk = check(i);//检查用户权限
			if (chk != 1)
			{
				printf("This directory is not your !\n");
				return;
			}
			else j = inode_array[i].inum;
			for (t = 0; t<INODENUM; t++)
			{
				if ((inode_array[t].inum>0) &&
					(inode_array[t].iparent == j) &&
					(inode_array[i].type == '-'))
					delet(t);//目录下有文件则删除
				else if ((inode_array[t].inum>0) &&
					(inode_array[t].iparent == j) &&
					(inode_array[i].type == 'd'))
					delet(t);//目录下有空目录则删除
			}
			if (t == INODENUM)
				delet(j);//下层目录为空删除之
		}
	if (i == INODENUM)
		delet(i);//待删除目录为空删除之
	return;
}
// 功能: 在当前目录下创建子目录(mkdir dir1)
void mkdir(void)
{
	int i;
	if (argc != 2)
	{
		printf("command mkdir must have two args. \n");
		return;
	}
	// 遍历i节点数组, 查找未用的i节点
	for (i = 0; i < INODENUM; i++)
		if (inode_array[i].inum < 0) break;
	if (i == INODENUM)
	{
		printf("Inode is full.\n");
		exit(-1);
	}
	inode_array[i].inum = i;
	strcpy(inode_array[i].file_name, argv[1]);
	inode_array[i].type = 'd';
	strcpy(inode_array[i].user_name, user.user_name);
	inode_array[i].iparent = inum_cur;
	inode_array[i].length = 0;
	save_inode(i);
}
// 功能: 在当前目录下创建文件(creat file1)
void creat(void)
{
	int i;
	if (argc != 2)
	{
		printf("command creat must have one args. \n");
		return;
	}
	for (i = 0; i < INODENUM; i++)
	{
		if ((inode_array[i].inum > 0) &&
			(inode_array[i].type == '-') &&
			!strcmp(inode_array[i].file_name, argv[1]))
		{
			printf("This file is exsit.\n");
			return;
		}
	}
	for (i = 0; i < INODENUM; i++)
		if (inode_array[i].inum < 0) break;
	if (i == INODENUM)
	{
		printf("Inode is full.\n");
		exit(-1);
	}
	inode_array[i].inum = i;
	strcpy(inode_array[i].file_name, argv[1]);
	inode_array[i].type = '-';
	strcpy(inode_array[i].user_name, user.user_name);
	inode_array[i].iparent = inum_cur;
	inode_array[i].length = 0;
	save_inode(i);
}
// 功能: 打开当前目录下的文件(open file1)
void open()
{
	int i, inum, mode, filenum, chk;
	if (argc != 2)
	{
		printf("command open must have one args. \n");
		return;
	}
	for (i = 0; i < INODENUM; i++)
		if ((inode_array[i].inum > 0) &&
			(inode_array[i].type == '-') &&
			!strcmp(inode_array[i].file_name, argv[1]))
			break;
	if (i == INODENUM)
	{
		printf("The file you want to open doesn't exsited.\n");
		return;
	}
	inum = i;
	chk = check(i);
	if (chk != 1)
	{
		printf("This file is not your !\n");
		return;
	}
	printf("Please input open mode:(1: read, 2: write, 3: read and write):");
	scanf("%d", &mode);
	gets_s(temp);
	if ((mode < 1) || (mode > 3))
	{
		printf("Open mode is wrong.\n");
		return;
	}
	for (i = 0; i < FILENUM; i++)
		if (file_array[i].inum < 0) break;
	if (i == FILENUM)
	{
		printf("The file table is full, please close some file.\n");
		return;
	}
	filenum = i;
	file_array[filenum].inum = inum;
	strcpy(file_array[filenum].file_name, inode_array[inum].file_name);
	file_array[filenum].mode = mode;
	file_array[filenum].offset = 0;
	printf("Open file %s by ", file_array[filenum].file_name);
	if (mode == 1) printf("read only.\n");
	else if (mode == 2) printf("write only.\n");
	else printf("read and write.\n");
}
// 功能: 从文件中读出字符(read file1)
void read()
{
	int i, start, num, inum;
	if (argc != 2)
	{
		printf("command read must have one args. \n");
		return;
	}
	for (i = 0; i < FILENUM; i++)
		if ((file_array[i].inum > 0) &&
			!strcmp(file_array[i].file_name, argv[1]))
			break;
	if (i == FILENUM)
	{
		printf("Open %s first.\n", argv[1]);
		return;
	}
	else if (file_array[i].mode == 2)
	{
		printf("Can't read %s.\n", argv[1]);
		return;
	}
	inum = file_array[i].inum;
	printf("The length of %s:%d.\n", argv[1], inode_array[inum].length);
	if (inode_array[inum].length > 0)
	{
		printf("The start position:");
		scanf("%d", &start);
		gets_s(temp);
		if ((start<0) || (start >= inode_array[inum].length))
		{
			printf("Start position is wrong.\n");
			return;
		}
		printf("The bytes you want to read:");
		scanf("%d", &num);
		gets_s(temp);
		if (num <= 0)
		{
			printf("The num you want to read is wrong.\n");
			return;
		}
		read_blk(inum);
		for (i = 0; (i < num) && (temp[i] != '\0'); i++)
			printf("%c", temp[start + i]);
		printf("\n");
	}
}
// 功能: 向文件中写入字符(write file1)
void write()
{
	int i, inum, length;
	if (argc != 2)
	{
		printf("Command write must have one args. \n");
		return;
	}
	for (i = 0; i < FILENUM; i++)
		if ((file_array[i].inum>0) &&
			!strcmp(file_array[i].file_name, argv[1])) break;
	if (i == FILENUM)
	{
		printf("Open %s first.\n", argv[1]);
		return;
	}
	else if (file_array[i].mode == 1)
	{
		printf("Can't write %s.\n", argv[1]);
		return;
	}
	inum = file_array[i].inum;
	printf("The length of %s:%d\n", inode_array[inum].file_name, inode_array[inum].length);
	if (inode_array[inum].length == 0)
	{
		printf("The length you want to write(0-1024):");
		scanf("%d", &length);
		gets_s(temp);
		if ((length < 0) && (length >1024))
		{
			printf("Input wrong.\n");
			return;
		}
		inode_array[inum].length = length;
		inode_array[inum].address[0] = get_blknum();
		if (length > 512)
			inode_array[inum].address[1] = get_blknum();
		save_inode(inum);
		printf("Input the data(Enter to end):\n");
		gets_s(temp);
		write_blk(inum);
	}
	else
		printf("This file can't be written.\n");
}
// 功能: 关闭已经打开的文件(close file1)
void close(void)
{
	int i;
	if (argc != 2)
	{
		printf("Command close must have one args. \n");
		return;
	}
	for (i = 0; i < FILENUM; i++)
		if ((file_array[i].inum > 0) &&
			!strcmp(file_array[i].file_name, argv[1])) break;
	if (i == FILENUM)
	{
		printf("This file doesn't be opened.\n");
		return;
	}
	else
	{
		file_array[i].inum = -1;
		printf("Close %s successful!\n", argv[1]);
	}
}
//删除目录树
void delet(int innum)
{
	/*int chk;
	chk=check(innum);
	if(chk!=1)
	{
	//printf("This directory is not yours !\n");
	return ;
	}*/
	inode_array[innum].inum = -1;
	if (inode_array[innum].length >= 0)
	{
		release_blk(inode_array[innum].address[0]);
		if (inode_array[innum].length >= 512)
			release_blk(inode_array[innum].address[1]);
	}
	save_inode(innum);
}
// 功能: 删除文件(delete file1)
void del(void)
{
	int i, chk;
	if (argc != 2)
	{
		printf("Command delete must have one args. \n");
		return;
	}
	for (i = 0; i < INODENUM; i++)
		if ((inode_array[i].inum > 0) &&
			(inode_array[i].type == '-') &&
			!strcmp(inode_array[i].file_name, argv[1])) break;
	if (i == INODENUM)
	{
		printf("This file doesn't exist.\n");
		return;
	}
	chk = check(i);
	if (chk != 1)
	{
		printf("This file is not your !\n");
		return;
	}
	/*inode_array[i].inum = -1;
	if(inode_array[i].length > 0)
	{
	release_blk(inode_array[i].address[0]);
	if(inode_array[i].length > 512)
	release_blk(inode_array[i].address[1]);
	}
	save_inode(i);*/
	delet(i);
}
// 功能: 退出当前用户(logout)
void logout()
{
	char choice;
	printf("Do you want to exit this user(y/n)?");
	scanf("%c", &choice);
	gets_s(temp);
	if ((choice == 'y') || (choice == 'Y'))
	{
		printf("\nCurrent user exited!\nPlease to login by other user!\n");
		login();
	}
	return;
}
//检查当前I节点的文件是否属于当前用户
int check(int i)
{
	int j;
	char *uuser, *fuser;
	uuser = user.user_name;
	fuser = inode_array[i].user_name;
	j = strcmp(fuser, uuser);
	if (j == 0)  return 1;
	else      return 0;
}
// 功能: 退出文件系统(quit)
void quit()
{
	char choice;
	printf("Do you want to exist(y/n):");
	scanf("%c", &choice);
	gets_s(temp);
	if ((choice == 'y') || (choice == 'Y'))
		exit(0);
}
// 功能: 显示错误
void errcmd()
{
	printf("Command Error!!!\n");
}
//清空内存中存在的用户名
void free_user()
{
	int i;
	for (i = 0; i<10; i++)
		user.user_name[i] = '\0';
}
// 功能: 循环执行用户输入的命令, 直到logout
// "help", "cd", "dir", "mkdir", "creat", "open","read", "write", "close", "delete", "logout", "clear", "format","quit","rd"
void command(void)
{
	char cmd[100];
	system("cls");
	do
	{
		pathset();
		gets_s(cmd);
		switch (analyse(cmd))
		{
		case 0:
			help();
			break;
		case 1:
			cd();
			break;
		case 2:
			dir();
			break;
		case 3:
			mkdir();
			break;
		case 4:
			creat();
			break;
		case 5:
			open();
			break;
		case 6:
			read();
			break;
		case 7:
			write();
			break;
		case 8:
			close();
			break;
		case 9:
			del();
			break;
		case 10:
			logout();
			break;
		case 11:
			system("cls");
			break;
		case 12:
			format();
			init();
			free_user();
			login();
			break;
		case 13:
			quit();
			break;
		case 14:
			rd();
			break;
		case 15:
			errcmd();
			break;
		default:
			break;
		}
	} while (1);
}
// 主函数
int main(void)
{
	login();
	init();
	command();
	return 0;
}

