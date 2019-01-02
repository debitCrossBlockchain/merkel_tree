// MerkelTree.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include"merkerl_manger.h"
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	string check_str = "B";

	cout << "Enter Merkle leafes: " << endl;
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
	v.push_back("A");
	v.push_back("B");
	v.push_back("C");
	v.push_back("D");
	v.push_back("E");
	v.push_back("F");
	v.push_back("G");
	v.push_back("H");
	v.push_back("I");
	v.push_back("J");

	cout << "Enter leaf to verify: " << endl;
	//��������֤��Ҷ�ӽڵ�
	//cin >> check_str;
	check_str = picosha2::hash256_hex_string(check_str);

	tree ntree;
	ntree.buildBaseLeafes(v);
	ntree.buildTree();
	ntree.merklerootexclusive(v,"");

	cout << "Check if: " << check_str << " is contained in tree " << endl;

	//��֤��������ڵ� �����޸ı�
	if (ntree.verify(check_str))
	{
		cout << "All clear\n";
	}
	else
	{
		cout << "something is wrong\n";
	}
	return 0;
}

