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
	//叶子节点为string类
	vector<string> v;

	//输入叶子节点
	//while (1)
	//{
	//	string str;
	//	cin >> str;
	//	if (str != ";")
	//	{
	//		//存入v
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
	//输入想验证的叶子节点
	//cin >> check_str;
	check_str = picosha2::hash256_hex_string(check_str);

	tree ntree;
	ntree.buildBaseLeafes(v);
	ntree.buildTree();
	ntree.merklerootexclusive(v,"");

	cout << "Check if: " << check_str << " is contained in tree " << endl;

	//验证有无这个节点 树有无改变
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

