#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

#define BLKSIZE    512		// 数据块的大小 
#define BLKNUM     512		// 数据块的块数
#define INODESIZE  32		// i节点的大小
#define INODENUM   32		// i节点的数目
#define FILENUM    8		// 打开文件表的数目

// 用户(20B)
typedef struct
{
	char user_name[10];	    // 用户名
	char password[10];	    // 密码
} User;

// i节点(32B)
typedef struct
{
	short inum;          // 文件i节点号
	char  file_name[10]; // 文件名
	char  type;          // 文件类型
	char  user_name[10]; // 文件所有者
	short iparent;       // 父目录的i节点号 
	short length;        // 文件长度
	short address[2];    // 存放文件的地址		
} Inode;

// 打开文件表(16B)
typedef struct
{
	short inum;	         // i节点号
	char  file_name[10]; // 文件名
	short mode;	         // 读写模式(1:read, 2:write,
						 //         3:read and write)
	short offset;        // 偏移量
} File_table;

// 申明函数
void login(void);
void init(void);
int  analyse(char *);
void save_inode(int);
int	get_blknum(void);
void read_blk(int);
void write_blk(int);
void release_blk(int);
void pathset();
void delet(int innum);
int  check(int i);
// 用户命令处理函数 
void help(void);
void cd(void);
void dir(void);
void mkdir(void);
void creat(void);
void open(void);
void read(void);
void write(void);
void close(void);
void del(void);
void logout(void);
void command(void);
void rd();
void quit();
