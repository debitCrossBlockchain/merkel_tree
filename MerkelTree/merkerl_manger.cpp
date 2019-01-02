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

//���ù�ϣֵ
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
node* node::getSibling() //�����ӵõ��Һ��ӣ����Һ��ӵõ�����
{
	//�õ��ýڵ�ĸ��ڵ�
	node* parent = getParent();

	//�жϸ��ڵ�����Ӻͱ��ڵ��Ƿ���ͬ
	//��ͬ�����Һ��ӣ���ͬ��������
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
	//����丸�ڵ�������Ǹýڵ� ����0 �����򷵻�1
	return parent->getChildren(0) == this ? 0 : 1;
}

node::~node() {}

tree::tree() {}

//����blockchain���涼merkle����Ҫ��Ҷ�ӽڵ���ż�������ԣ���һ�������ڰ�������������Ϊ����ʱ��
//�����һ�����׸���һ�ݣ��ճ�ż����


int tree::makeBinary(vector<node*> &node_vector) //ʹҶ�ӽڵ��Ϊ˫��
{
	int vectSize = node_vector.size();
	if ((vectSize % 2) != 0) //���Ԫ�ظ���Ϊ�������Ͱ������һ���ڵ�push_backһ��
	{
		node_vector.push_back(node_vector.end()[-1]); //push_back���һ��Ԫ�� ��.end()-1 
		vectSize++;
	}
	return vectSize;
}

void tree::buildTree() //����merkle tree
{
	do
	{
		vector<node*> new_nodes;
		makeBinary(base.end()[-1]); //����βԪ�� ��һ���ڵ��б�

		for (int i = 0; i < base.end()[-1].size(); i += 2)
		{
			node* new_parent = new node;
			//���ø��׽ڵ� �������һ��Ԫ�� ��һ���ڵ��б�ĵ�i��i+1��
			base.end()[-1][i]->setParent(new_parent);
			base.end()[-1][i + 1]->setParent(new_parent);

			//ͨ���������ӽڵ�Ĺ�ϣֵ���ø��ڵ��ϣֵ
			new_parent->setHash(base.end()[-1][i]->getHash() + base.end()[-1][i + 1]->getHash());
			//���ø��ڵ�����Һ��ӽڵ�����Ϊ������
			new_parent->setChildren(base.end()[-1][i], base.end()[-1][i + 1]);
			//��new_parentѹ��new_nodes
			new_nodes.push_back(new_parent);

			cout << "Hash togther: " << base.end()[-1][i]->getHash() << \
				" and " << base.end()[-1][i + 1]->getHash() << " attached: " << \
				&new_parent << endl;
		}

		// printTreelevel(new_nodes);

		//����һ�ֵĸ��ڵ�new_n  odesѹ��base
		base.push_back(new_nodes);

		cout << "Hashed level with: " << base.end()[-1].size() << '\n';
	} while (base.end()[-1].size() > 1); //����ÿһ�ֵõ���һ��ĸ��ڵ㣬֪���õ����ڵ� �˳�ѭ��

	merkleRoot = base.end()[-1][0]->getHash(); //���ڵ�Ĺ�ϣֵ

	cout << "Merkle Root is : " << merkleRoot << endl << endl;
}
void tree::printTreeLevel(vector<node*> v) //��ӡÿ���ڵ�Ĺ�ϣֵ
{
	cout << endl;

	for (node* el : v)
	{
		cout << el->getHash() << endl;
	}
}
void tree::buildBaseLeafes(vector<string> base_leafs) //����Ҷ�ӽڵ��б�
{
	vector<node*> new_nodes;

	cout << "Root leafs are : " << '\n';

	//��ÿһ���ַ���������Ӧ�ڵ㣬��ͨ������ַ������ù�ϣֵ
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
	string act_hash = hash; //����֤��Ҷ�ӽڵ�Ķ��ϣֵ

	//���base[0] ��Ҷ�ӽڵ�����һ���ڵ��hashֵ�������
	for (int i = 0; i < base[0].size(); i++)
	{
		if (base[0][i]->getHash() == hash)
		{
			el_node = base[0][i]; //ָ��ýڵ�
		}
	}
	if (el_node == nullptr)
	{
		return 0;
	}

	cout << "Hash verify: " << act_hash << '\n';

	do  //��֤merkle tree�Ƿ�ı�� 
	{
		//���ڵ�Ĺ�ϣ�����ӵĹ�ϣstring+�Һ��ӵĹ�ϣstring
		//���el_node�ĸ��ڵ����ڵ���el_node
		if (el_node->checkDir() == 0)
		{
			//�����Ӿ� �����ӵĹ�ϣstring+�Һ��ӵĹ�ϣstring
			act_hash = picosha2::hash256_hex_string(act_hash + el_node->getSibling()->getHash());
		}
		else
		{
			act_hash = picosha2::hash256_hex_string(el_node->getSibling()->getHash() + act_hash);
		}

		std::cout << "Hash verify: " << act_hash << '\n';

		el_node = el_node->getParent();
	} while ((el_node->getParent()) != NULL); //������ڵ�

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
	return "1" + sub_str; //root �ڵ�
}

void tree::minproof(int tx_sum, int target_index){
	string flag;
	flag = buildflags(tx_sum, target_index);
	std::cout << "minflag :" << flag << endl;
}
