#include"stdafx.h"
#include"merkerl_manger.h"
#include<list>
using namespace std;

node::node()
{
	parent = nullptr;
	children[0] = nullptr;
	children[1] = nullptr;
}

//设置哈希值
void node::setHash(string hash_str)
{
	this->hash_str = picosha2::hash256_hex_string(hash_str);
}
node* node::getParent()
{
	return parent;
}
void node::setParent(node *parent)
{
	this->parent = parent;
}
void node::setChildren(node *children_l, node *children_r)
{
	children[0] = children_l;
	children[1] = children_r;
}
node* node::getSibling() //是左孩子得到右孩子，是右孩子得到左孩子
{
	//得到该节点的父节点
	node* parent = getParent();

	//判断父节点的左孩子和本节点是否相同
	//相同返回右孩子，不同返回左孩子
	return parent->getChildren(0) == this ? parent->getChildren(1) : parent->getChildren(0);
}
node* node::getChildren(int index)
{
	return index <= 1 ? children[index] : nullptr;
}
string node::getHash()
{
	return hash_str;
}
int node::checkDir()
{
	//如果其父节点的左孩子是该节点 返回0 否则则返回1
	return parent->getChildren(0) == this ? 0 : 1;
}

node::~node() {}

tree::tree() {}

//由于blockchain里面都merkle运算要求叶子节点是偶数，所以，当一个区块内包含当交易数量为奇数时，
//把最后一个交易复制一份，凑成偶数。


int tree::makeBinary(vector<node*> &node_vector) //使叶子节点成为双数
{
	int vectSize = node_vector.size();
	if ((vectSize % 2) != 0) //如果元素个数为奇数，就把再最后一个节点push_back一次
	{
		node_vector.push_back(node_vector.end()[-1]); //push_back最后一个元素 即.end()-1 
		vectSize++;
	}
	return vectSize;
}

void tree::buildTree() //建造merkle tree
{
	do
	{
		vector<node*> new_nodes;
		makeBinary(base.end()[-1]); //传入尾元素 即一个节点列表

		for (int i = 0; i < base.end()[-1].size(); i += 2)
		{
			node* new_parent = new node;
			//设置父亲节点 传入最后一个元素 即一个节点列表的第i和i+1个
			base.end()[-1][i]->setParent(new_parent);
			base.end()[-1][i + 1]->setParent(new_parent);

			//通过两个孩子节点的哈希值设置父节点哈希值
			new_parent->setHash(base.end()[-1][i]->getHash() + base.end()[-1][i + 1]->getHash());
			//将该父节点的左右孩子节点设置为这两个
			new_parent->setChildren(base.end()[-1][i], base.end()[-1][i + 1]);
			//将new_parent压入new_nodes
			new_nodes.push_back(new_parent);

			cout << "Hash togther: " << base.end()[-1][i]->getHash() << \
				" and " << base.end()[-1][i + 1]->getHash() << " attached: " << \
				&new_parent << endl;
		}

		// printTreelevel(new_nodes);

		//将新一轮的父节点new_n  odes压入base
		base.push_back(new_nodes);

		cout << "Hashed level with: " << base.end()[-1].size() << '\n';
	} while (base.end()[-1].size() > 1); //这样每一轮得到新一层的父节点，知道得到根节点 退出循环

	merkleRoot = base.end()[-1][0]->getHash(); //根节点的哈希值

	cout << "Merkle Root is : " << merkleRoot << endl << endl;
}
void tree::printTreeLevel(vector<node*> v) //打印每个节点的哈希值
{
	cout << endl;

	for (node* el : v)
	{
		cout << el->getHash() << endl;
	}
}
void tree::buildBaseLeafes(vector<string> base_leafs) //建立叶子节点列表
{
	vector<node*> new_nodes;

	cout << "Root leafs are : " << '\n';

	//给每一个字符串创建对应节点，并通过这个字符串设置哈希值
	for (auto leaf : base_leafs)
	{
		node* new_node = new node;
		new_node->setHash(leaf);
		cout << new_node->getHash() << '\n';

		new_nodes.push_back(new_node);
	}

	base.push_back(new_nodes);
	cout << '\n';
}

int tree::verify(string hash)
{
	node* el_node = nullptr;
	string act_hash = hash; //想验证的叶子节点的额哈希值

	//如果base[0] 即叶子节点中有一个节点的hash值和其相等
	for (int i = 0; i < base[0].size(); i++)
	{
		if (base[0][i]->getHash() == hash)
		{
			el_node = base[0][i]; //指向该节点
		}
	}
	if (el_node == nullptr)
	{
		return 0;
	}

	cout << "Hash verify: " << act_hash << '\n';

	do  //验证merkle tree是否改变过 
	{
		//父节点的哈希是左孩子的哈希string+右孩子的哈希string
		//如果el_node的父节点的左节点是el_node
		if (el_node->checkDir() == 0)
		{
			//是左孩子就 做孩子的哈希string+右孩子的哈希string
			act_hash = picosha2::hash256_hex_string(act_hash + el_node->getSibling()->getHash());
		}
		else
		{
			act_hash = picosha2::hash256_hex_string(el_node->getSibling()->getHash() + act_hash);
		}

		std::cout << "Hash verify: " << act_hash << '\n';

		el_node = el_node->getParent();
	} while ((el_node->getParent()) != NULL); //到达根节点

	return act_hash == merkleRoot ? 1 : 0;
}
void tree::iterateUp(int element){
	node* el_node = this->base[0][element];

	do {
		std::cout << "Current Hash: " << el_node->getHash() << '\n';
		/*std::cout << "Next Node: " << el_node << '\n';
		std::cout << "Next Parent: " << el_node->getParent() << '\n';*/
	} while ((el_node = el_node->getParent()) != NULL);
	//std::cout << "Done iterating" << '\n';
}
tree::~tree() {}

string tree::buildflags(int tx_sum, int target_index){
	std::list<int> hash;
	int nodes_length = tx_sum * 2 - 1;
	string sub_str;
	int cur = target_index;

	while (cur < nodes_length - 1){
		if (cur % 2 == 0) {
			sub_str = "1" + sub_str + "0";
			if (cur == target_index){
				hash.push_front(cur);
				hash.push_back(cur + 1);
			}
			else {
				hash.push_back(cur + 1);
			}
		}
		else {
			sub_str = "01" + sub_str;
			if (cur == target_index){
				hash.push_front(cur);
				hash.push_back(cur - 1);
			}
			else {
				hash.push_front(cur - 1);
			}
		}
		cur = nodes_length - (nodes_length - cur - (cur + 1) % 2) / 2;
	}
	std::cout << "hash list of " << target_index << " :" << endl;
	std::list<int>::const_iterator iter = hash.begin();
	while (iter != hash.end()){
		std::cout << " " << *iter << ";";
	}
	std::cout << endl;
	return "1" + sub_str; //root 节点
}

void tree::minproof(int tx_sum, int target_index){
	string flag;
	flag = buildflags(tx_sum, target_index);
	std::cout << "minflag :" << flag << endl;
}

void tree::maxproof(int tx_sum, int target_index){
	string flag;
	flag = buildflags(tx_sum, target_index);
	std::cout << "maxflag :" << flag << endl;
}

void tree::normalproof(int tx_sum, int pre_index, int next_index){
	string pre_flag, next_flag;
	pre_flag = buildflags(tx_sum, pre_index);
	next_flag = buildflags(tx_sum, next_index);
	std::cout << "preflag :" << pre_flag << endl;
	std::cout << "nextflag :" << next_flag << endl;
}

void tree::merklerootexclusive(const std::vector<string> &hash, const std::string &target_hash){
	//已确认target不在txs里，merkle树已构造出，给出不存在性证明
	int length = hash.size();

	int index = 1;
	int slice = 0;
	while (index < length)
	{
		std::string cur = hash[index];
		if (target_hash.compare(cur)>0) {
			index++;
			continue;
		}
		else if (target_hash.compare(cur) == 0) {
			continue;
		}
		else {
			if (index == 1){
				//next = sortedtxhash[0]
				//fmt.Printf("mintx : %s vs \ntarget : %s \n", reverseHash(cur).String(), reverseHash(targettxhash).String()) //最小tx>目标交易
				minproof(length, 1);                                                                                        //给出cur的proof路径及hash，并锁定cur指向最小tx
				return;
			}
			else {
				//fmt.Printf("pretx : %s <\ntargettx : %s <\nnexttx : %s \n", sortedtxhash[index - 1].String(), targettxhash.String(), sortedtxhash[index]) //最大tx<目标交易
				normalproof(length, index - 1, index);                                                                                                   //给出pre，next的proof路径及hash，spv根据生成的merkle路径定位可知pre与next相邻
				return;
			}
		}
	}
	//fmt.Printf("maxtx : %s vs \ntarget : %s \n", sortedtxhash[length - 1], reverseHash(targettxhash).String()) //最大tx<目标交易
	maxproof(length, length - 1);                                                                              //给出cur的proof路径及hash，锁定cur指向最大tx
	return;

}