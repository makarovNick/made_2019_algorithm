/*************************************************************************************/
//6_1.Обход дерева в порядке pre - order
//Ограничение времени	0.2 секунды
//Ограничение памяти	64Mb
//Ввод	стандартный ввод или input.txt
//Вывод	стандартный вывод или output.txt
//Дано число N < 106 и последовательность целых чисел из[-231..231] длиной N.
//Требуется построить бинарное дерево, заданное наивным порядком вставки.Т.е., 
//при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K, 
//то узел K добавляется в правое поддерево root; иначе в левое поддерево root.
//Выведите элементы в порядке pre - order(сверху вниз).
//
//Рекурсия запрещена.
/*************************************************************************************/
#include <iostream>
#include <stack>
#include <functional>

class tree
{
	using function = std::function<void(const int)>;
	struct node
	{
		int value;
		node* left;
		node* right;

		node(int v)
			: value(v)
			, left(nullptr)
			, right(nullptr)
		{
		}
		node(const node&) = delete;
		node& operator=(node&&) = delete;
		node(node&&) = delete;
		node& operator=(const node&) = delete;
		~node()
		{
			if (left != nullptr)
			{
				delete left;
				left = nullptr;
			}
			if (right != nullptr)
			{
				delete right;
				right = nullptr;
			}
		}
	};
public:
	tree() : root(nullptr) {}
	~tree()
	{
		delete root;
	}
	void insert(const int val);
	void preOrderTraversal(const function& func) const;
private:
	node* root;
	void insert(const int val, node* node);
};


void tree::insert(const int val) {
	if (root == nullptr)
	{
		root = new node(val);
	}
	else
	{
		insert(val, root);
	}
}

void tree::insert(const int val, node* node)
{
	tree::node* newnode = new tree::node(val);

	tree::node* x = root;
	tree::node* y = nullptr;

	while (x != nullptr) {
		y = x;
		if (val < x->value)
			x = x->left;
		else
			x = x->right;
	}
	if (val < y->value)
		y->left = newnode;
	else
		y->right = newnode;
}
void tree::preOrderTraversal(const function& func) const
{
	if (root == nullptr)
		return;

	std::stack<node*> nodeStack;
	nodeStack.push(root);

	while (nodeStack.empty() == false)
	{
		struct node* node = nodeStack.top();
		func(node->value);
		nodeStack.pop();

		if (node->right)
			nodeStack.push(node->right);
		if (node->left)
			nodeStack.push(node->left);
	}
}
int main()
{
	tree tr;
	size_t n;
	int v;
	std::cin >> n;
	for (size_t i = 0; i < n; i++)
	{
		std::cin >> v;
		tr.insert(v);
	}
	tr.preOrderTraversal([](const int v) {std::cout << v << " "; });
	return 0;
}