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
	//输入想验证的叶子节点
	//cin >> check_str;
	check_str = picosha2::hash256_hex_string(check_str);

	tree ntree;
	ntree.buildBaseLeafes(v);
	ntree.buildTree();
	ntree.merklerootexclusive(v, "10");

	//cout << "Check if: " << check_str << " is contained in tree " << endl;

	////验证有无这个节点 树有无改变
	if (ntree.verify(check_str))
	{
		cout << "All clear\n";
	}
	else
	{
		cout << "something is wrong\n";
	}

	vector<string> spv_v;
	spv_v.push_back("e11a20bae8379fdc0ed560561ba33f30c877e0e95051aed5acebcb9806f6521f");
	spv_v.push_back("7ffba6de305b8e12dfa42b70467ae808662c249dbf85798e608ea9486365637b");
	spv_v.push_back("10");
	spv_v.push_back("11");
	spv_v.push_back("0032459c69d6498f98eaaed1fbd94302c6527476ca64e7113c3555ad62271074");
	spv_v.push_back("a901f842b0016f1e350d20b751851a7179e26dfbb74b213c7a92d37f3c4fbb6c");
	bool flag = ntree.merklerootinclusive(spv_v, 2, "a901f842b0016f1e350d20b751851a7179e26dfbb74b213c7a92d37f3c4fbb6c");
	int b = 0;
	return 0;
}

