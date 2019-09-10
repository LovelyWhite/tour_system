#include"head.h"
int fuzzyCompare(char* origin, char * textin)
{
	int countorigin = 0, counttextin = 0, i, j, err = 0;
	countorigin = strlen(origin);
	counttextin = strlen(textin);
	if (counttextin > countorigin)
	{
		return 0;
	}
	else
	{
		for (i = 0; i < countorigin - counttextin + 1; i++)
		{
			err = 0;//放在这里的原因是为了不影响下一次推移的比较 
			for (j = 0; j < counttextin; j++)
			{
				if (textin[j] == origin[j + i])
				{
					err++;
				}
			}
			if (err == counttextin)//一比较完就判断是否相同 
			{
				return 1;
			}
			else
				return 0;
		}


	}
}
void addSlash(char *name)
{
	int i = 0, j = 0;
	char ini1 = '*', ini2 = '*';
	while (name[i] != '\0')
	{
		if (name[i] == '\\')
		{
			j = i;
			ini1 = name[j];
			while (ini1 != '\0')
			{
				ini2 = ini1;
				ini1 = name[j + 1];
				name[j + 1] = ini2;
				j++;
			}
			name[j + 1] = '\0';
			i++;
		}
		i++;
	}
}
void initSystem(DestPtr** destHead, UserPtr** userHead, System** setting)
{
	system("mode con cols=90 lines=16");//定义宽和高
	FILE* Sets = NULL;
	*destHead = malloc(sizeof(DestPtr));
	(*destHead)->next = NULL;
	*userHead = malloc(sizeof(UserPtr));
	(*userHead)->next = NULL;
	*setting = malloc(sizeof(System));
	strcpy_s((*setting)->setFileAddr, sizeof(char) * 30, "setting.ini");
	strcpy_s((*setting)->usersFileAddr, sizeof(char) * 30, "users.dat");
	strcpy_s((*setting)->interestFileAddr, sizeof(char) * 30, "dest.dat");
	(*setting)->curVisibility = 0;
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(handle, &CursorInfo);//获取控制台光标信息  
	CursorInfo.bVisible = (*setting)->curVisibility; //隐藏控制台光标
	SetConsoleCursorInfo(handle, &CursorInfo);//设置控制台光标状态
	Sets = fopen((*setting)->setFileAddr, "rb");
	if (!Sets)//设置文件不存在
	{
		Sets = fopen((*setting)->setFileAddr, "wb");
		fwrite((*setting), sizeof(System), 1, Sets);
		fclose(Sets);
	}
	else
	{
		if (fgetc(Sets) == EOF)
		{
			fclose(Sets);
			Sets = fopen((*setting)->setFileAddr, "wb");
			fwrite((*setting), sizeof(System), 1, Sets);
			fclose(Sets);
		}
		else
		{
			rewind(Sets);
			fread((*setting), sizeof(System), 1, Sets);
			fclose(Sets);
		}
	}
	printxy(18, 2, "操作说明：");
	printxy(18, 3, "-  +  移动");
	printxy(18, 4, "Enter 进入");
	printxy(40, 4, "旅游管理系统");
	printxy(18, 5, "Esc 退出");
	printxy(18, 8,"Press Any Key skip");
	_getch();
}
void setSystem(System* setting)
{
	if (currentUser.authority == 1)
	{
		System s;
		printxy(0, 10, "是否显示光标？(1/0)");
		scanf("%d", &s.curVisibility);
		printxy(0, 10, "请输入用户文件位置及名称");
		scanf("%s", s.usersFileAddr);
		printxy(0, 10, "请输入设置文件位置及名称");
		scanf("%s", s.setFileAddr);
		printxy(0, 10, "请输入景点文件位置及名称");
		scanf("%s", s.interestFileAddr);
		addSlash(s.usersFileAddr);
		addSlash(s.setFileAddr);
		addSlash(s.interestFileAddr);
		DeleteFile(setting->setFileAddr);//删除文件
		DeleteFile(setting->interestFileAddr);
		DeleteFile(setting->usersFileAddr);
		*setting = s;
	}
	else
	{
		printxy(0, 10, "不是管理员");
		SLEEP;
	}
}
int load(DestPtr** destHead, UserPtr** userHead, System setting,int* userCount, int* destCount)
{
	int count = 0;
	FILE* Users=NULL;
	FILE* Dests = NULL;
	Users = fopen(setting.usersFileAddr, "rb");
	if (!Users)//用户文件不存在
	{
		count+=1;
	}
	else
	{
		if (fgetc(Users) == EOF)//用户文件无信息
		{
			fclose(Users);
			count+=2;
		}
		else//用户文件有信息
		{
			rewind(Users);
			UserPtr* nodeU = malloc(sizeof(UserPtr));
			nodeU->next = NULL;
			while (!ferror(Users)&&!feof(Users))
			{
				nodeU = malloc(sizeof(UserPtr));
				fread(nodeU, sizeof(UserPtr), 1, Users);
				nodeU->next = (*userHead)->next;
				(*userHead)->next = nodeU;
				(*userCount)++;
			}
		}
	}
	if ((*userHead)->next != NULL)
	{
		(*userHead)=(*userHead)->next;
		(*userCount)--;
	}
	Dests = fopen(setting.interestFileAddr, "rb");
	if (!Dests)//景点文件不存在
	{
		count+= 4;
	}
	else
	{
		if (fgetc(Dests) == EOF)//景点文件无信息
		{
			fclose(Dests);
			count+=8;
		}
		else//景点文件有信息
		{
			rewind(Dests);
			DestPtr* nodeD = malloc(sizeof(DestPtr));
			nodeD->next = NULL;
			while (!ferror(Dests) && !feof(Dests))
			{
				nodeD = malloc(sizeof(DestPtr));
				fread(nodeD, sizeof(DestPtr), 1, Dests);
				nodeD->next = (*destHead)->next;
				(*destHead)->next = nodeD;
				destCount++;
			}
		}
	}
	if ((*userHead)->next != NULL)
	{
		(*destHead) = (*destHead)->next;
		(*destCount)--;
	}
	return count;
}
void save(DestPtr* destHead, UserPtr* userHead, System setting)
{
	
	FILE* dests = fopen(setting.interestFileAddr, "wb");
	while (destHead->next != NULL)
	{
		fwrite(destHead->next, sizeof(DestPtr), 1, dests);
	}
	fclose(dests);
	FILE* users = fopen(setting.usersFileAddr, "wb");
	while (userHead->next != NULL)
	{
		fwrite(userHead->next, sizeof(UserPtr), 1, users);
		userHead = userHead->next;
	}
	fclose(users);
	FILE* sets = fopen(setting.setFileAddr, "wb");
	fwrite(&setting, sizeof(System), 1, sets);
	fclose(sets);
	
}
void printxy(int x, int y, const char* words)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x; pos.Y = y;
	if (y == 10)
	{
		SetConsoleCursorPosition(handle, pos);
		printf("                                                                 ");//清空输出缓冲
	}
	SetConsoleCursorPosition(handle, pos);
	printf("%s",words);
}
void menu()
{
	printxy(0, 0, "------------------------------");
	printxy(3, 1, "Add");
	printxy(3, 2, "Delete");
	printxy(3, 3, "Motify");
	printxy(3, 4, "LookUp");
	printxy(3, 5, "SysSet");
	printxy(3, 6, "SaveAll");
	printxy(0, 7, "------------------------------");
}
void hello()
{
	printxy(0, 0, "------------------------------");
	printxy(3, 1, "Sign in");
	printxy(3, 2, "Sign up");
	printxy(3, 3, " ");
	printxy(3, 4, " ");
	printxy(3, 5, " ");
	printxy(3, 6, " ");
	printxy(0, 7, "------------------------------");
}
void addInterface()
{
	printxy(0, 0, "------------------------------");
	printxy(3, 1, "Add User Interest");
	printxy(3, 2, "Add System Interest(Admin)");
	printxy(3, 3, " ");
	printxy(3, 4, " ");
	printxy(3, 5, " ");
	printxy(0, 6, " ");
	printxy(0, 7, "------------------------------");
}
void deleteInterface()
{
	printxy(0, 0, "------------------------------");
	printxy(3, 1, "Delete User Interest");
	printxy(3, 2, "Delete System Interest(Admin)");
	printxy(3, 3, " ");
	printxy(3, 4, " ");
	printxy(3, 5, " ");
	printxy(3, 6, " ");
	printxy(0, 6, "------------------------------");
}
void lookUpInterface()
{
	printxy(0, 0, "------------------------------");
	printxy(3, 1, "Normal LookUp Interest");
	printxy(3, 2, "Fuzzy LookUp Interest");
	printxy(3, 3, "Normal LookUp User");
	printxy(3, 4, "Fuzzy LookUp User");
	printxy(3, 5, " ");
	printxy(3, 6, " ");
	printxy(0, 7, "------------------------------");
}
int login(UserPtr* userHead, int userCount)
{
	int count = 0;
	char userNumber[20];
	char userPassword[20];
	if (userCount == 0)
	{
		return 4;//代表账户为空
		//
	}
	else
	{
		printxy(0,10, "请输入账号");
		scanf("%s", userNumber);
		while (userHead->next != NULL)
		{
			count = 0;//账户验证失败
			if (strcmp(userHead->next->data.number, userNumber)==0)
			{
				printxy(0,10, "请输入密码");
				scanf("%s", userPassword);
				if (strcmp(userHead->next->data.password, userPassword)==0)
				{
					currentUser = userHead->next->data;
					count += 1;//密码验证成功
					break;
				}
				else
				{
					count += 2;//密码验证失败
				}
			}
			userHead = userHead->next;
		}
	}
	return count;//0 代表无此账户 1代表登陆成功 2代表有此账户但是密码错误
}
void sign(UserPtr* userHead, int* userCount)
{
	int i = 0;
	char userName[20];
	char userNumber[20];
	char userPassword[20];
	char userPasswordConfirm[20];
	UserPtr* ptr = userHead;//需要遍历一遍账户
	printxy(0,10, "请输入姓名");
	scanf("%s", userName);
	printxy(0,10, "请输入账号");
	scanf("%s", userNumber);
	while (ptr->next != NULL)
	{
		if (strcmp(ptr->next->data.number, userNumber)==0)
		{
			printxy(0,10, "已存在账户");
			SLEEP;
			return;
		}
		ptr = ptr->next;
	}
	UserPtr* node = malloc(sizeof(UserPtr));
	strcpy(node->data.name, userName);
	strcpy(node->data.number, userNumber);
	printxy(0,10, "请输入密码");
	scanf("%s", userPassword);
	printxy(0,10, "请确认密码");
	scanf("%s", userPasswordConfirm);
	if (strcmp(userNumber, userPasswordConfirm)!=0)
	{
		printxy(0,10, "密码不一致");
		SLEEP;
	}
	else
	{
		strcpy(node->data.password, userPassword);
		if ((*userCount) == 0)
		{
			node->data.authority = 1;//初始化权限为1
		}
		else
		{
			node->data.authority = 0;//初始化权限为0
		}
		for (i = 0; i < 20; i++)
		{
			node->data.destNumber[i] = -1;
		}
		node->next = userHead->next;
		userHead->next = node;
		(*userCount)++;
		printxy(0,10, "注册成功，请登录");
		SLEEP;
	}
}
void add(UserPtr* userHead, DestPtr* destHead,int* userCount, int* destCount)
{
	int select = 1, s = 0;
	char x[10];
	while (1)
	{
		while (s != ENTER)
		{
			system("cls");
			addInterface();
			printxy(1, select, ">");
			sprintf(x, "Current:%1d", select);
			printxy(0, 8, x);
			s = _getch();
			if (s == ESC)
				return;
			printxy(1, select, " ");
			if (s == '-')select--;
			else if (s == '=')select++;
			if (select == 0)//1增 2删 3改 4查 5设置
				select++;
			else if (select == 3)
				select--;
			printxy(1, select, ">");
		}
		s = 0;
		switch (select)
		{
		case 1:addUserInterest(userHead); break;
		case 2:addSystemInterest(destHead); break;
		}
		system("cls");
	}
}
void delete(UserPtr* userHead, DestPtr* destHead, int* userCount, int* destCount)
{
	int select = 1, s = 0;
	char x[10];
	while (1)
	{
		while (s != ENTER)
		{
			system("cls");
			deleteInterface();
			printxy(1, select, ">");
			sprintf(x, "Current:%1d", select);
			printxy(0, 8, x);
			s = _getch();
			if (s == ESC)
				return;
			printxy(1, select, " ");
			if (s == '-')select--;
			else if (s == '=')select++;
			if (select == 0)//1增 2删 3改 4查 5设置
				select++;
			else if (select == 4)
				select--;
			printxy(1, select, ">");
		}
		s = 0;
		switch (select)
		{
		case 1:deleteUserInterest(userHead); break;
		case 2:deleteSystemInterest(destHead); break;
		case 3:deleteUser(userHead); break;
		}
	}
}
void motify(UserPtr* userHead, DestPtr* destHead)
{

}
void LookUp(UserPtr* userHead, DestPtr* destHead)
{
	int select = 1, s = 0;
	char x[10];
	while (1)
	{
		while (s != ENTER)
		{
			system("cls");
			lookUpInterface();
			printxy(1, select, ">");
			sprintf(x, "Current:%1d", select);
			printxy(0, 8, x);
			s = _getch();
			if (s == ESC)
				return;
			printxy(1, select, " ");
			if (s == '-')select--;
			else if (s == '=')select++;
			if (select == 0)//1增 2删 3改 4查 5设置
				select++;
			else if (select == 5)
				select--;
			printxy(1, select, ">");
		}
		s = 0;
		switch (select)
		{
		case 1:normalLookUpInterest(); break;
		case 2:fuzzyLookUpInterest(); break;
		case 3:normalLookUpUser(); break;
		case 4:fuzzyLookUpUser(); break;
		}
	}
}
void normalLookUpInterest()
{

}
void fuzzyLookUpInterest()
{

}
void normalLookUpUser()
{

}
void fuzzyLookUpUser()
{

}
void addUserInterest(UserPtr* userHead)
{
	int count,i=0,current=0;
	char x[20];
	UserPtr* ptr =userHead;
	printxy(0,10, "请输入需要添加的个数");
	scanf("%d", &count);
	while (i != 20&&currentUser.destNumber[i]!=-1)
	{
		current++;
		i++;
	}
	current = 20 - current;
	if (current < count)
	{
		printxy(0,10, "超出个数");
		SLEEP;
	}
	else
	{
		
		for (i = 20-current; i < count+20-current; i++)
		{
			sprintf(x, "请输入第%d个", i + 1);
			printxy(0, 10, x);
			scanf("%d", &currentUser.destNumber[i]);
		}
		while (ptr->next != NULL)
		{
			if (ptr->next->data.number == currentUser.number)
			{
				memcpy(&(ptr->next->data), &currentUser, sizeof(Users));
			}
			ptr = ptr->next;
		}
	}
}
void addSystemInterest(DestPtr* destHead)
{
	DestPtr* ptr = destHead;
	int number;
	if (currentUser.authority == 1)
	{
		printxy(0,10, "请输入编号");
		scanf("%d", &number);
		while (ptr->next != NULL)
		{
			if (ptr->next->data.number == number)
			{
				printxy(0, 10, "已存在该景点");
				SLEEP;
				return;
			}
			ptr = ptr->next;
		}
		DestPtr* dest = malloc(sizeof(DestPtr));
		dest->next = NULL;
		dest->data.number = number;
		printxy(0,10, "请输入城市");
		scanf("%s", dest->data.interest);
		printxy(0,10, "请输入景点级别");
		scanf("%d", &dest->data.level);
		printxy(0,10, "请输入开始时间");
		scanf("%d", &dest->data.time[0]);
		printxy(0,10, "请输入结束时间");
		scanf("%d", &dest->data.time[0]);
		printxy(0,10, "请输入价格");
		scanf("%lf", &dest->data.price);
		dest->next = destHead->next;
		destHead->next = dest;
		printxy(0,10, "添加成功");
		SLEEP;
	}
	else
	{
		printxy(0,10, "不是管理员");
		SLEEP;
	}
	return;
}
void deleteUserInterest(UserPtr* userHead)
{

}
void deleteSystemInterest(DestPtr* destHead)
{

}
void deleteUser(UserPtr* userHead)
{

}