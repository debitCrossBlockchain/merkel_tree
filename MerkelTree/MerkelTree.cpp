// MerkelTree.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include"merkerl_manger.h"
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	string check_str = "10";

	//cout << "Enter Merkle leafes: " << endl;
	//Ҷ�ӽڵ�Ϊstring��
	vector<string> v;

	//����Ҷ�ӽڵ�
	//while (1)
	//{
	//	string str;
	//	cin >> str;
	//	if (str != ";")
	//	{
	//		//����v
	//		v.push_back(str);
	//	}
	//	else
	//	{
	//		break;
	//	}
	//}
	v.push_back("0");
	v.push_back("1");
	v.push_back("2");
	v.push_back("3");
	v.push_back("4");
	v.push_back("5");
	v.push_back("6");
	v.push_back("7");
	v.push_back("8");
	v.push_back("9");
	v.push_back("10");
	v.push_back("11");
	v.push_back("12");
	v.push_back("13");
	v.push_back("14");
	v.push_back("15");

	//cout << "Enter leaf to verify: " << endl;
	//��������֤��Ҷ�ӽڵ�
	//cin >> check_str;
	check_str = picosha2::hash256_hex_string(check_str);

	tree ntree;
	ntree.buildBaseLeafes(v);
	ntree.buildTree();
	ntree.merklerootexclusive(v, "10");

	//cout << "Check if: " << check_str << " is contained in tree " << endl;

	////��֤��������ڵ� �����޸ı�
	//if (ntree.verify(check_str))
	//{
	//	cout << "All clear\n";
	//}
	//else
	//{
	//	cout << "something is wrong\n";
	//}
	return 0;
}

