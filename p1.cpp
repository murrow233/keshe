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
 ��¼�ֹ���Ա���û�
 ����Ա���ܣ�
			1.��������Ϣ�������ӡ�ɾ�����޸ĺ�����Ϣ��
			2.�鿴��Ʊ��Ϣ����鿴���к������Ʊ�������ѯĳ�κ�����Ϣ)
 �û����ܣ�
			1.�鿴���к�����Ϣ�����ݺ���š������ء�Ŀ�ĵصȣ�
			2.��Ʊ
			3.��Ʊ
*/


//�������ڽṹ��
struct Date
{
	int year;//��
	int month;//��
	int day;//��
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


//����ʱ��
struct StTime
{
	int hour;//ʱ
	int mint;//��
	//int sec;//��
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


//���幺Ʊ��Ϣ�ṹ��
struct StTickInfo
{
	string id;       //�����ţ�����ʱ�����ɣ�
	Date date;       //��Ʊ����
	string hangban;  //����ţ�ͨ������ţ����Բ�ѯ������Ϣ
	string guest;    //��Ʊ����
	string guId;     //��Ʊ�����֤��
	string phone;    //��Ʊ�˵绰
};


//������Ϣ
struct StHangbanInfo
{
	string hangban;//�����
	string name;   //���չ�˾����
	string src;    //������
	string dst;    //Ŀ�ĵ�
	Date date;     //��������
	StTime tm;     //���ʱ��
	int nmbZw;     //��λ����
	int nmbUse;    //����Ʊ��
	int price;     //Ʊ��
};


//����<<�����������д�뺽����Ϣ
ostream& operator<<(ostream& output, const StHangbanInfo& info)
{
	output << info.hangban << "  " << info.name << "  " << info.src << "  " << info.dst << "  ";
	output << info.date << " ";
	output << info.tm << "  ";
	output << info.price << "  " << info.nmbZw << "  " << info.nmbUse;
	return output;
}


//����<<�����������д�빺Ʊ��Ϣ
ofstream& operator<<(ofstream& output, const StTickInfo& info)
{
	output << info.id;
	output << info.date << " ";
	output << info.hangban << " " << info.guest << " " << info.guId << " " << info.phone;
	return output;
}


//����>>����������ڶ��뺽����Ϣ
ifstream& operator>>(ifstream& input, StHangbanInfo& info)
{
	input >> info.hangban >> info.name >> info.src >> info.dst;
	input >> info.date;
	input >> info.tm;
	input >> info.price >> info.nmbZw >> info.nmbUse;
	return input;
}


//����>>����������ڶ��빺Ʊ��Ϣ
ifstream& operator>>(ifstream& input, StTickInfo& info)
{
	input >> info.id;
	input >> info.date ;
	input >> info.hangban >> info.guest >> info.guId >> info.phone;
	return input;
}


//����ȫ�ֱ�����������Ͷ�Ʊ��Ϣ
struct StHangbanInfo g_allHb[100]; //���浱ǰ���к�����Ϣ
int g_nmbHb = 0; //ʵ�ʺ�������


struct StTickInfo g_allTicks[200]; //���浱ǰ���еĶ�����Ϣ
int g_nmbTicks = 0; //��ǰϵͳ��ʵ�ʹ�Ʊ����


//���ݺ���Ų�ѯ������Ϣ
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
	cout << "δ�ҵ�������Ϣ" << endl;
	return stnull;
}


//����ʱ�����ɶ������
void getId(string buf)
{
	time_t now;
	struct tm* t;
	char tmp[100];
	time(&now); // ��ȡʱ���
	t = localtime(&now); // תΪʱ��ṹ
	strftime(tmp, sizeof(tmp), "%Y%m%d%H%M%S", t);
	buf = tmp;
}


//��ȡ��ǰʱ��
struct Date getCurrentDate()
{
	Date oo;
	time_t t;
	struct tm* lt;
	time(&t);//��ȡʱ���
	lt = localtime(&t);// תΪʱ��ṹ
	oo.month = lt->tm_mon;
	oo.day = lt->tm_mday;
	return oo;
}


//��ʾ������Ϣ����
void showHbTitle()
{
	cout << "�����  ���չ�˾  ��ʼ��  Ŀ�ĵ�   ������ڼ�ʱ��   �۸�  ��λ��  ʣ��Ʊ��" << endl;
}


//��ѯ����-����Ա���û�ͨ��
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
		cout << "Ŀǰ���޺�����Ϣ��������Ӻ���" << endl;
		system("pause");
		return;
	}
	while (1)
	{
		system("cls");
		cout << "1.���ݺ���Ų�ѯ" << endl;
		cout << "2.���ݳ����ء�Ŀ�ĵز�ѯ" << endl;
		cout << "��ѡ��" << endl;
		while (1)
		{
			scanf_s("%d", &opt);
			if (opt == 1 || opt == 2) break;
		}
		fflush(stdin); //������뻺��
		switch (opt)
		{
		case 1:
			cout << "�����뺽��ţ�";
			cin >> id;
			hb = searchById(id);
			if (hb.hangban.length() > 0)
			{
				showHbTitle();
				cout << hb << "  " << hb.nmbZw - hb.nmbUse << endl;
			}
			break;
		case 2:
			cout << "��������ʼ�ء�Ŀ�ĵأ�";
			cin >> start >> end;
			showHbTitle();
			for (i = 0; i < g_nmbHb; i++)
			{
				if (g_allHb[i].src == start && g_allHb[i].dst == end)
					cout << g_allHb[i] << "  " << g_allHb[i].nmbZw - g_allHb[i].nmbUse << endl;
			}
			break;
		}
		cout << "�Ƿ������ѯ(Y/N)?";
		while ((ch = getchar()) != '\n' && ch != EOF);
		scanf_s("%c", &ch, 1);
		if (ch == 'N' || ch == 'n')
			break;
	}

}


//----------------����Ա����------------------//
//��������
void addHangban()
{
	struct StHangbanInfo st;
	system("cls");
	fflush(stdin);
	cout << "�����뺽��ţ�";
	cin >> st.hangban;
	cout << "�����뺽�๫˾���ƣ�";
	cin >> st.name;
	cout << "��������ʼ�أ�";
	cin >> st.src;
	cout << "������Ŀ�ĵأ�";
	cin >> st.dst;
	cout << "�������������(YYYY-M-D)��";
	scanf("%d-%d-%d", &st.date.year, &st.date.month, &st.date.day);
	cout << "���������ʱ��(H:M)��";
	scanf("%d:%d", &st.tm.hour, &st.tm.mint);
	cout << "������Ʊ�ۣ�"; //���ﲻ������ͷ�Ȳպ;��ò�
	cin >> st.price;
	cout << "��������λ������";
	cin >> st.nmbZw;
	st.nmbUse = 0;
	g_allHb[g_nmbHb++] = st; //��������
	cout << "��Ӻ���ɹ�" << endl;
	system("pause");
}


//�Ӷ�����Ϣ��ɾ��ĳ��������ж�Ʊ��Ϣ
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
			i--; //i����һ�£�forѭ��i++����ָ���ɾ��Ԫ�ص�λ�ã�����ʼ�ж�
		}
	}
}


//ɾ�����ࣨ����Ҳ��Ҫ�Ӷ�Ʊ��Ϣ��ɾ����
void delHb()
{
	int i;
	char id[16] = { 0 };
	fflush(stdin);
	system("cls");
	cout << "������Ҫɾ���ĺ���ţ�";
	cin >> id;
	for (i = 0; i < g_nmbHb; i++)
	{
		if (id == g_allHb[i].hangban)
		{
			//�Ӷ�Ʊ��Ϣ��ɾ���ú�������ж�Ʊ��Ϣ
			delSpecHb(g_allHb[i].hangban);
			cout << "ɾ���ɹ�" << endl;
			g_nmbHb--;
			system("pause");
			return;
		}
	}
	cout << "δ�ҵ��ú����" << endl;
	system("pause");
}


//����Ա�������
void deal_manager()
{
	int opt;
	while (1)
	{
		system("cls");
		cout << "1.��ѯ����" << endl;
		cout << "2.��Ӻ���" << endl;
		cout << "3.ɾ������" << endl;
		cout << "4.�˳�" << endl;
		cout << "��ѡ��";
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


//----------------�ͻ�����------------------//
//��Ʊ
void gouPiao()
{
	struct StTickInfo tk;
	int i, flag = 0;

	system("cls");
	fflush(stdin);
	cout << "�����뺽��ţ�";
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
			cout << "δ�ҵ��ú��࣬���������뺽��ţ�";
		}
		else
		{
			if (flag == 1)
				cout << "�ú����Ʊ����������ѡ���������ࣺ" << endl;
			else
			{
				g_allHb[i].nmbUse += 1; //���ú��������Ʊ��+1
				break;
			}
		}

	}

	cout << "�����붩Ʊ��������";
	cin >> tk.guest;
	cout << "�����붩Ʊ�����֤�ţ�";
	cin >> tk.guId;
	cout << "�����붩Ʊ���ֻ��ţ�";
	cin >> tk.phone;
	//���ɶ�����
	getId(tk.id);
	//��乺Ʊʱ��
	tk.date = getCurrentDate();
	//����
	g_allTicks[g_nmbTicks++] = tk;
	cout << "��Ʊ�ɹ�" << endl;
	system("pause");
}


//��Ʊ��ͨ�����֤��Ʊ���޶�һ��ֻ����һ��Ʊ��
void tuiPiao()
{
	char buf[16];
	int i, j;
	fflush(stdin);
	cout << "��������Ʊ�˵����֤�ţ�";
	cin >> buf;
	for (i = 0; i < g_nmbTicks; i++)
	{
		if (g_allTicks[i].guId == buf)
		{
			//�޸ĸú��������Ʊ��
			for (j = 0; j < g_nmbHb; j++)
			{
				if (g_allHb[j].hangban == g_allTicks[i].hangban)
				{
					g_allHb[j].nmbUse -= 1;
					break;
				}
			}
			//ɾ���ö�Ʊ��Ϣ
			for (j = i; j < g_nmbTicks - 1; j++)
			{
				g_allTicks[j] = g_allTicks[j + 1];
			}
			g_nmbTicks--;
			cout << "��Ʊ�ɹ�" << endl;
			break;
		}
	}
	if (i == g_nmbTicks)
		cout << "û�д��˵Ĺ�Ʊ��Ϣ" << endl;
	system("pause");
}


//�ͻ��������
void deal_customer()
{
	int opt;
	while (1)
	{
		system("cls");
		cout << "1.��ѯ����" << endl;
		cout << "2.��Ʊ" << endl;
		cout << "3.��Ʊ" << endl;
		cout << "4.�˳�" << endl;
		cout << "��ѡ��";
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


//��ȡ���к�������
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


//��ȡ���й�Ʊ����
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


//��д�º�������
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


//��д�¹�Ʊ����
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
		cout << "|---------��Ʊ����ϵͳ---------|" << endl;
		cout << "|         1.����Ա��¼         |" << endl;
		cout << "|         2.�ͻ���¼           |" << endl;
		cout << "|         3.�˳�ϵͳ           |" << endl;
		cout << "|------------------------------|" << endl;
		cout << "��ѡ��";
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