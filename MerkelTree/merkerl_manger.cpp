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

		printTreeLevel(new_nodes);
		node_list_.insert(node_list_.end(), new_nodes.begin(), new_nodes.end());
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
	for (auto leaf : base_leafs){
		node* new_node = new node;
		new_node->setHash(leaf);
		cout << new_node->getHash() << '\n';
		new_nodes.push_back(new_node);
	}
	node_list_.insert(node_list_.end(), new_nodes.begin(), new_nodes.end());
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

string tree::HashMerkleBranches(const string &left, const string &right){
	return picosha2::hash256_hex_string(left + right);
}

void tree::BuildAuditTrail(vector<MerkleProofHash> &auditTrail, node* parent, node* child){
	//if (parent != null)
	//{
	//	Contract(() = > child.Parent == parent, "Parent of child is not expected parent.");
	//	var nextChild = parent.LeftNode == child ? parent.RightNode : parent.LeftNode;
	//	var direction = parent.LeftNode == child ? MerkleProofHash.Branch.Left : MerkleProofHash.Branch.Right;

	//	// For the last leaf, the right node may not exist.  In that case, we ignore it because it's
	//	// the hash we are given to verify.
	//	if (nextChild != null)
	//	{
	//		auditTrail.Add(new MerkleProofHash(nextChild.Hash, direction));
	//	}

	//	BuildAuditTrail(auditTrail, child.Parent.Parent, child.Parent);
	//}
	if (parent != NULL)
	{
		//Contract(() = > child.Parent == parent, "Parent of child is not expected parent.");
		auto nextChild = parent->getChildren(0) == child ? parent->getChildren(1) : parent->getChildren(0);
		auto direction = parent->getChildren(0) == child ? MerkleProofHash::Branch::Left : MerkleProofHash::Branch::Right;

		// For the last leaf, the right node may not exist.  In that case, we ignore it because it's
		// the hash we are given to verify.
		if (nextChild != NULL){
			auditTrail.push_back(MerkleProofHash(nextChild->getHash(), direction));
		}

		BuildAuditTrail(auditTrail, child->getParent()->getParent(), child->getParent());
	}
}

bool tree::VerifyAudit(const std::string &rootHash, const std::string& leafHash, std::vector<MerkleProofHash> &auditTrail){
	if (auditTrail.size() < 0){
		return false;
	}
	std::string testHash = leafHash;

	// TODO: Inefficient - compute hashes directly.

	for (auto iter = auditTrail.begin(); iter != auditTrail.end(); iter++){
		testHash = iter->direction_ == MerkleProofHash::Branch::Left ?
			HashMerkleBranches(testHash, iter->hash_) : HashMerkleBranches(iter->hash_, testHash);
	}
	return rootHash == testHash;
}

void tree::AuditProof(const std::string &leafHash, std::vector<MerkleProofHash> &auditTrail){

	node* leafNode = nullptr;
	string act_hash = leafHash; //����֤��Ҷ�ӽڵ�Ķ��ϣֵ

	//���base[0] ��Ҷ�ӽڵ�����һ���ڵ��hashֵ�������
	for (int i = 0; i < base[0].size(); i++)
	{
		if (base[0][i]->getHash() == leafHash)
		{
			leafNode = base[0][i]; //ָ��ýڵ�
		}
	}

	if (leafNode != NULL){
		if (leafNode->getParent() == nullptr){
			return;
		}
		auto parent = leafNode->getParent();
		BuildAuditTrail(auditTrail, parent, leafNode);
	}
}

void tree::TestVerifyAudit(){
	string hash = HashMerkleBranches("5feceb66ffc86f38d952786c6d696c79c2dbc239dd4e91b46729d73a27fb57e9","6b86b273ff34fce19d6b804eff5a3f5747ada4eaa22f1d49c01e52ddb7875b4b");
	vector<MerkleProofHash> auditTrail;
	AuditProof("4a44dc15364204a80fe80e9039455cc1608281820fe2b24f1e5233ade6af1dd5",auditTrail);
	VerifyAudit("a901f842b0016f1e350d20b751851a7179e26dfbb74b213c7a92d37f3c4fbb6c", "4a44dc15364204a80fe80e9039455cc1608281820fe2b24f1e5233ade6af1dd5", auditTrail);
}