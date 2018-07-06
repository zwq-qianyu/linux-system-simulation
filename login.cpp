#include "head.h"
FILE *fp;	 // 打开文件指针
User user;		// 当前的用户
char temp[2 * BLKSIZE];	// 缓冲区

int main(){
	login();
	return 0;
}
 
void login(){
	/*功能: 用户登陆，如果是新用户则创建用户*/
	char *p;
	int  flag;
	char user_name[10];
	char password[10];
	char file_name[10] = "user.txt";
	char choice;    //选择是否（y/n）
	do{
		printf("login:");
		gets(user_name);
		printf("password:");
		p = password;
		while (*p = _getch()){
			if (*p == 0x0d) //当输入回车键时，0x0d为回车键的ASCII码
			{
				*p = '\0'; //将输入的回车键转换成空格
				break;
			}
			printf("*");   //将输入的密码以"*"号显示
			p++;
		}
		flag = 0;
		if ((fp = fopen(file_name, "r+")) == NULL){
			printf("\nCan't open file %s.\n", file_name);
			printf("This filesystem is not exist now, it will be create~~~\n");
			format();     //黄春伟负责这个模块 
			login();
		}
		while (!feof(fp)){
			fread(&user, sizeof(User), 1, fp);
			// 已经存在的用户, 且密码正确
			if (!strcmp(user.user_name, user_name) &&
				!strcmp(user.password, password)){
				fclose(fp);
				printf("\n");
				return;     //登陆成功，直接跳出登陆函数 
			}
			// 已经存在的用户, 但密码错误
			else if (!strcmp(user.user_name, user_name)){
				printf("\nThis user is exist, but password is incorrect.\n");
				flag = 1;    //设置flag为1，表示密码错误，重新登陆 
				fclose(fp);
				break;
			} 
		}
		if (flag == 0) break;    //用户不存在，先跳出循环创建新用户 
	} while (flag);
	
	// 创建新用户
	if (flag == 0){
		printf("\nDo you want to creat a new user?(y/n):");
		scanf("%c", &choice);
		gets(temp);
		if ((choice == 'y') || (choice == 'Y')){
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
