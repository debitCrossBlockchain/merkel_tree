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

	ntree.VerifySPV("a901f842b0016f1e350d20b751851a7179e26dfbb74b213c7a92d37f3c4fbb6c");

	vector<string> spv_v;
	spv_v.push_back("4a44dc15364204a80fe80e9039455cc1608281820fe2b24f1e5233ade6af1dd5");
	spv_v.push_back("1f12220da624f190b8e2d897ff9400503d1ada72cc2569499001ebf96485c80a");
	spv_v.push_back("d13f4af490794ac71f7a1c3570bbfc6adc92bef2e628ab6cddb24cc68e35399d");
	spv_v.push_back("599e90a987a3df1ef27a685ebcb8baf9fb1dc7d3f2bb135b63a1d8adcd4034b6");
	spv_v.push_back("a901f842b0016f1e350d20b751851a7179e26dfbb74b213c7a92d37f3c4fbb6c");
	bool flag = ntree.merklerootinclusive(spv_v, 0, "a901f842b0016f1e350d20b751851a7179e26dfbb74b213c7a92d37f3c4fbb6c");
	int b = 0;
	return 0;
}

