#ifndef head2h
#define head2h
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<conio.h>
#include<string.h>
#include<windows.h>
#define TURE 1
#define FALSE 0
#define ENTER 13
#define BACKSPACE 8
#define ESC 27
#define SLEEP Sleep(300)
typedef struct Dest
{
	int number;//编号
	char city[20];//城市
	char interest[20];//景点
	int level;//景点级别
	int time[2];//时间段
	double price;//价格
}Destination;//旅游地点数据结构
typedef struct Use
{
	char number[20];//编号
	char name[20];//姓名
	int destNumber[20];//目的地景点编号
	char password[20];//密码
	int authority;//权限
}Users;
typedef struct DestPt
{
	struct Dest data;
	struct DestPt* next;
}DestPtr;
typedef struct UsePt
{
	struct Use data;
	struct UsePt* next;
}UserPtr;
typedef struct Sys
{
	char usersFileAddr[30];
	char interestFileAddr[30];
	char setFileAddr[30];
	int curVisibility; //光标可见
}System;
Users currentUser;
void initSystem(DestPtr** destHead, UserPtr** userHead, System** setting);//初始化系统
void setSystem(System* setting);//系统设置
int fuzzyCompare(char* origin, char * textin);//模糊比较字符串函数
int load(DestPtr** destHead, UserPtr** userHead, System setting, int* userCount, int* destCount);//载入函数
void save(DestPtr* destHead, UserPtr* userHead, System setting);//保存函数
void printxy(int x,int y,const char* words);//定点输出函数
void menu();//主菜单
void hello();//注册登陆界面
void addInterface();//添加界面
void deleteInterface();//删除界面
void lookUpInterface();//查找界面
int login(UserPtr* userHead,int userCount);//登陆函数
void sign(UserPtr* userHead  ,int* userCount);//注册函数
void add(UserPtr* userHead, DestPtr* destHead, int* userCount, int* destCount);//添加函数
void addUserInterest(UserPtr* userHead);//添加用户景点
void addSystemInterest(DestPtr* destHead);//添加系统景点
void delete(UserPtr* userHead, DestPtr* destHead, int* userCount, int* destCount);//删除函数
void deleteUserInterest(UserPtr* userHead);//删除用户景点
void deleteSystemInterest(DestPtr* destHead);//删除系统景点
void deleteUser(UserPtr* userHead);//删除用户
void motify(UserPtr* userHead, DestPtr* destHead);//修改函数
void LookUp(UserPtr* userHead, DestPtr* destHead);//查找函数
void addSlash(char *name);//添加双斜杠函数
void normalLookUpInterest();//精确查找景点
void fuzzyLookUpInterest();//模糊查找景点
void normalLookUpUser();//精确查找用户
void fuzzyLookUpUser();//模糊查找用户
#endif // !head2h

