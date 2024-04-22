#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

/*
 登录分管理员和用户
 管理员功能：
			1.管理航班信息（如增加、删除、修改航班信息）
			2.查看订票信息（如查看所有航班的售票情况、查询某次航班信息)
 用户功能：
			1.查看所有航班信息（根据航班号、出发地、目的地等）
			2.订票
			3.退票
*/


//定义日期结构体
struct Date
{
	int year;//年
	int month;//月
	int day;//日
};

ostream& operator<<(ostream& output, const Date& date)
{
	output << setfill('0') << date.year << "-" << setw(2) << date.month << "-" << setw(2) << date.day;
	return output;
}

ofstream& operator<<(ofstream& output, const Date& date)
{
	output << date.year << " " << date.month + 1 << " " << date.day;
	return output;
}

ifstream& operator>>(ifstream& input, Date& date)
{
	input >> date.year >> date.month >> date.day;
	return input;
}


//定义时间
struct StTime
{
	int hour;//时
	int mint;//分
	//int sec;//秒
};

ostream& operator<<(ostream& output, const StTime& time)
{
	output << setfill('0') << setw(2) << time.hour << ":" << setw(2) << time.mint /* << ":" << setw(2) << time.sec*/;
	return output;
}

ofstream& operator<<(ofstream& output, const StTime& time)
{
	output << time.hour << " " << time.mint /* << " " << time.sec*/;
	return output;
}

ifstream& operator>>(ifstream& input, StTime& time)
{
	input >> time.hour >> time.mint/* >> time.sec*/;
	return input;
}


//定义购票信息结构体
struct StTickInfo
{
	string id;       //订单号（根据时间生成）
	Date date;       //购票日期
	string hangban;  //航班号，通过航班号，可以查询航班信息
	string guest;    //购票人名
	string guId;     //购票人身份证号
	string phone;    //购票人电话
};


//航班信息
struct StHangbanInfo
{
	string hangban;//航班号
	string name;   //航空公司名称
	string src;    //出发地
	string dst;    //目的地
	Date date;     //航班日期
	StTime tm;     //起飞时间
	int nmbZw;     //座位数量
	int nmbUse;    //已售票数
	int price;     //票价
};


//重载<<运算符，用于写入航班信息
ostream& operator<<(ostream& output, const StHangbanInfo& info)
{
	output << info.hangban << "  " << info.name << "  " << info.src << "  " << info.dst << "  ";
	output << info.date << " ";
	output << info.tm << "  ";
	output << info.price << "  " << info.nmbZw << "  " << info.nmbUse;
	return output;
}


//重载<<运算符，用于写入购票信息
ofstream& operator<<(ofstream& output, const StTickInfo& info)
{
	output << info.id;
	output << info.date << " ";
	output << info.hangban << " " << info.guest << " " << info.guId << " " << info.phone;
	return output;
}


//重载>>运算符，用于读入航班信息
ifstream& operator>>(ifstream& input, StHangbanInfo& info)
{
	input >> info.hangban >> info.name >> info.src >> info.dst;
	input >> info.date;
	input >> info.tm;
	input >> info.price >> info.nmbZw >> info.nmbUse;
	return input;
}


//重载>>运算符，用于读入购票信息
ifstream& operator>>(ifstream& input, StTickInfo& info)
{
	input >> info.id;
	input >> info.date ;
	input >> info.hangban >> info.guest >> info.guId >> info.phone;
	return input;
}


//定义全局变量，管理航班和订票信息
struct StHangbanInfo g_allHb[100]; //保存当前所有航班信息
int g_nmbHb = 0; //实际航班数量


struct StTickInfo g_allTicks[200]; //保存当前所有的订单信息
int g_nmbTicks = 0; //当前系统中实际购票数量


//根据航班号查询航班信息
struct StHangbanInfo searchById(char* id)
{
	int i;
	struct StHangbanInfo stnull;
	memset(&stnull.hangban, 0, 16);
	for (i = 0; i < g_nmbHb; i++)
	{
		if (g_allHb[i].hangban == id)
			return g_allHb[i];
	}
	cout << "未找到航班信息" << endl;
	return stnull;
}


//根据时间生成订单编号
void getId(string buf)
{
	time_t now;
	struct tm* t;
	char tmp[100];
	time(&now); // 获取时间戳
	t = localtime(&now); // 转为时间结构
	strftime(tmp, sizeof(tmp), "%Y%m%d%H%M%S", t);
	buf = tmp;
}


//获取当前时间
struct Date getCurrentDate()
{
	Date oo;
	time_t t;
	struct tm* lt;
	time(&t);//获取时间戳
	lt = localtime(&t);// 转为时间结构
	oo.month = lt->tm_mon;
	oo.day = lt->tm_mday;
	return oo;
}


//显示航班信息标题
void showHbTitle()
{
	cout << "航班号  航空公司  起始地  目的地   起飞日期及时间   价格  座位数  剩余票数" << endl;
}


//查询航班-管理员和用户通用
void search()
{
	int opt, i;
	char ch;
	char id[16] = { 0 };
	char start[16] = { 0 }, end[16] = { 0 };
	struct StHangbanInfo hb;
	int nmb = 0;
	system("cls");
	if (g_nmbHb == 0)
	{
		cout << "目前暂无航班信息，请先添加航班" << endl;
		system("pause");
		return;
	}
	while (1)
	{
		system("cls");
		cout << "1.根据航班号查询" << endl;
		cout << "2.根据出发地、目的地查询" << endl;
		cout << "请选择：" << endl;
		while (1)
		{
			scanf_s("%d", &opt);
			if (opt == 1 || opt == 2) break;
		}
		fflush(stdin); //清空输入缓存
		switch (opt)
		{
		case 1:
			cout << "请输入航班号：";
			cin >> id;
			hb = searchById(id);
			if (hb.hangban.length() > 0)
			{
				showHbTitle();
				cout << hb << "  " << hb.nmbZw - hb.nmbUse << endl;
			}
			break;
		case 2:
			cout << "请输入起始地、目的地：";
			cin >> start >> end;
			showHbTitle();
			for (i = 0; i < g_nmbHb; i++)
			{
				if (g_allHb[i].src == start && g_allHb[i].dst == end)
					cout << g_allHb[i] << "  " << g_allHb[i].nmbZw - g_allHb[i].nmbUse << endl;
			}
			break;
		}
		cout << "是否继续查询(Y/N)?";
		while ((ch = getchar()) != '\n' && ch != EOF);
		scanf_s("%c", &ch, 1);
		if (ch == 'N' || ch == 'n')
			break;
	}

}


//----------------管理员功能------------------//
//新增航班
void addHangban()
{
	struct StHangbanInfo st;
	system("cls");
	fflush(stdin);
	cout << "请输入航班号：";
	cin >> st.hangban;
	cout << "请输入航班公司名称：";
	cin >> st.name;
	cout << "请输入起始地：";
	cin >> st.src;
	cout << "请输入目的地：";
	cin >> st.dst;
	cout << "请输入起飞日期(YYYY-M-D)：";
	scanf("%d-%d-%d", &st.date.year, &st.date.month, &st.date.day);
	cout << "请输入起飞时间(H:M)：";
	scanf("%d:%d", &st.tm.hour, &st.tm.mint);
	cout << "请输入票价："; //这里不再区分头等舱和经济舱
	cin >> st.price;
	cout << "请输入座位数量：";
	cin >> st.nmbZw;
	st.nmbUse = 0;
	g_allHb[g_nmbHb++] = st; //保存数据
	cout << "添加航班成功" << endl;
	system("pause");
}


//从订单信息中删除某航班的所有订票信息
void delSpecHb(string id)
{
	int i, j;
	for (i = 0; i < g_nmbTicks; i++)
	{
		if (g_allTicks[i].hangban == id)
		{
			for (j = i; j < g_nmbTicks - 1; j++)
			{
				g_allTicks[j] = g_allTicks[j - 1];
			}
			g_nmbTicks--;
			i--; //i回退一下，for循环i++后，仍指向该删除元素的位置，并开始判断
		}
	}
}


//删除航班（航班也需要从订票信息中删除）
void delHb()
{
	int i;
	char id[16] = { 0 };
	fflush(stdin);
	system("cls");
	cout << "请输入要删除的航班号：";
	cin >> id;
	for (i = 0; i < g_nmbHb; i++)
	{
		if (id == g_allHb[i].hangban)
		{
			//从订票信息中删除该航班的所有订票信息
			delSpecHb(g_allHb[i].hangban);
			cout << "删除成功" << endl;
			g_nmbHb--;
			system("pause");
			return;
		}
	}
	cout << "未找到该航班号" << endl;
	system("pause");
}


//管理员功能入口
void deal_manager()
{
	int opt;
	while (1)
	{
		system("cls");
		cout << "1.查询航班" << endl;
		cout << "2.添加航班" << endl;
		cout << "3.删除航班" << endl;
		cout << "4.退出" << endl;
		cout << "请选择：";
		cin >> opt;
		switch (opt)
		{
			case 1:
				search();
				break;
			case 2:
				addHangban();
				break;
			case 3:
			delHb();
				break;
			case 4:
				return;
		}
	}
}


//----------------客户功能------------------//
//购票
void gouPiao()
{
	struct StTickInfo tk;
	int i, flag = 0;

	system("cls");
	fflush(stdin);
	cout << "请输入航班号：";
	while (1)
	{
		cin >> tk.hangban;
		for (i = 0; i < g_nmbHb; i++)
		{
			if (g_allHb[i].hangban == tk.hangban)
			{
				if (g_allHb[i].nmbZw - g_allHb[i].nmbUse == 0)
					flag = 1;
				break;
			}
		}
		if (i == g_nmbHb)
		{
			cout << "未找到该航班，请重新输入航班号：";
		}
		else
		{
			if (flag == 1)
				cout << "该航班机票已售罄，请选择其他航班：" << endl;
			else
			{
				g_allHb[i].nmbUse += 1; //将该航班的已售票数+1
				break;
			}
		}

	}

	cout << "请输入订票人姓名：";
	cin >> tk.guest;
	cout << "请输入订票人身份证号：";
	cin >> tk.guId;
	cout << "请输入订票人手机号：";
	cin >> tk.phone;
	//生成订单号
	getId(tk.id);
	//填充购票时间
	tk.date = getCurrentDate();
	//保存
	g_allTicks[g_nmbTicks++] = tk;
	cout << "购票成功" << endl;
	system("pause");
}


//退票（通过身份证退票，限定一人只能买一张票）
void tuiPiao()
{
	char buf[16];
	int i, j;
	fflush(stdin);
	cout << "请输入退票人的身份证号：";
	cin >> buf;
	for (i = 0; i < g_nmbTicks; i++)
	{
		if (g_allTicks[i].guId == buf)
		{
			//修改该航班的已售票数
			for (j = 0; j < g_nmbHb; j++)
			{
				if (g_allHb[j].hangban == g_allTicks[i].hangban)
				{
					g_allHb[j].nmbUse -= 1;
					break;
				}
			}
			//删除该订票信息
			for (j = i; j < g_nmbTicks - 1; j++)
			{
				g_allTicks[j] = g_allTicks[j + 1];
			}
			g_nmbTicks--;
			cout << "退票成功" << endl;
			break;
		}
	}
	if (i == g_nmbTicks)
		cout << "没有此人的购票信息" << endl;
	system("pause");
}


//客户功能入口
void deal_customer()
{
	int opt;
	while (1)
	{
		system("cls");
		cout << "1.查询航班" << endl;
		cout << "2.购票" << endl;
		cout << "3.退票" << endl;
		cout << "4.退出" << endl;
		cout << "请选择：";
		scanf_s("%d", &opt);
		switch (opt)
		{
		case 1:
			search();
			break;
		case 2:
			gouPiao();
			break;
		case 3:
			tuiPiao();
			break;
		case 4:
			return;
		}
	}
}


//读取已有航班数据
void ReadhbData(const string pFileName)
{
	int i;
	ifstream infile;
	infile.open(pFileName);

	if (!infile.is_open())
	{
		cout << "error" << endl;
		exit(1);
	}

	for (i = 0; i < 100; i++)
	{

		infile >> g_allHb[i];

		if (infile.fail())
		{
			g_nmbHb = i + 1;
			break;
		}
	}

	infile.close();
}


//读取已有购票数据
void ReadgpData(const string pFileName)
{
	int i;
	ifstream infile;
	infile.open(pFileName);

	if (!infile.is_open())
	{
		cout << "error" << endl;
		exit(1);
	}

	for (i = 0; i < 100; i++)
	{

		infile >> g_allTicks[i];

		if (infile.fail())
		{
			g_nmbTicks = i + 1;
			break;
		}
	}

	infile.close();
}


//覆写新航班数据
void OutputhbData(const string pFileName)
{
	int i;
	ofstream outfile;
	outfile.open(pFileName);

	if (!outfile.is_open())
	{
		cout << "error" << endl;
		exit(1);
	}

	for (i = 0; i < g_nmbHb; i++)
	{
		outfile << g_allHb[i] << endl;
	}

	outfile.close();
}


//覆写新购票数据
void OutputgpData(const string pFileName)
{
	int i;
	ofstream outfile;
	outfile.open(pFileName);

	if (!outfile.is_open())
	{
		cout << "error" << endl;
		exit(1);
	}

	for (i = 0; i < g_nmbTicks; i++)
	{
		outfile << g_allTicks[i] << endl;
	}

	outfile.close();
}


int main(void)
{
	int opt;
	const string pFileName1 = "../hbdata.txt";
	const string pFileName2 = "../gpdata.txt";
	ReadhbData(pFileName1);
	ReadgpData(pFileName2);
	while (1)
	{
		system("cls");
		cout << "|---------订票管理系统---------|" << endl;
		cout << "|         1.管理员登录         |" << endl;
		cout << "|         2.客户登录           |" << endl;
		cout << "|         3.退出系统           |" << endl;
		cout << "|------------------------------|" << endl;
		cout << "请选择：";
		cin >> opt;
		switch (opt)
		{
		case 1:
			deal_manager();
			break;
		case 2:
			deal_customer();
			break;
		case 3:
			OutputhbData(pFileName1);
			OutputgpData(pFileName2);
			return 0;
		}
	}
	return 0;
}