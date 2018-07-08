#include "head.h"
//int argc;		// 用户命令的参数个数
//char *argv[5];		// 用户命令的参数
FILE *fp;	 // 打开文件指针
User user;		// 当前的用户
char temp[2 * BLKSIZE];	// 缓冲区

vector<string>vc_of_str;    // 用户命令的参数个数
string  s1, s2;            // 用户命令的参数

int main(){
	su();
	return 0;
}

void su(){
	/*功能: 切换当前用户(logout)*/ 
	char *p;
	int flag; 
	string user_name;
	char password[10];
	char file_name[10] = "user.txt";
	fp = fopen(file_name, "r");           //初始化指针，将文件系统的指针指向文件系统的首端(以只读方式打开文件)
	/*if (argc != 2){
		printf("command su must have one object. \n");
		return;
	}*/
	do{
		user_name = s2;
		printf("password:");
		p = password;
		while (*p = _getch()){
			if (*p == 0x0d){ 		//当输入回车键时，0x0d为回车键的ASCII码
				*p = '\0'; 			//将输入的回车键转换成空格
				break;
			}
			printf("*");   //将输入的密码以"*"号显示
			p++;
		}
		flag = 0;
		while (!feof(fp)){
			fread(&user, sizeof(User), 1, fp);
			// 已经存在的用户, 且密码正确
			if ((user.user_name == user_name) &&
				!strcmp(user.password, password)){
				fclose(fp);
				printf("\n");
				return;     //登陆成功，直接跳出登陆函数 
			}
			// 已经存在的用户, 但密码错误
			else if ((user.user_name == user_name)){
				printf("\nThis user is exist, but password is incorrect.\n");
				flag = 1;    //设置flag为1，表示密码错误，重新登陆 
				fclose(fp);
				break;
			} 
		}
		if (flag == 0){
			printf("\nThis user is not exist.\n");
			break;     //用户不存在，直接跳出循环，进行下一条指令的输入
		} 
	}while(flag);
}
