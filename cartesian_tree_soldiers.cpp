/********************************************************************************************/
// 7_1.Солдаты ДД
//Ограничение времени	1 секунда
//Ограничение памяти	500Mb
//Ввод	стандартный ввод или input.txt
//Вывод	стандартный вывод или output.txt
//В одной военной части решили построить в одну шеренгу по росту.Т.к.часть была далеко не
//образцовая, то солдаты часто приходили не вовремя,
//а то их и вовсе приходилось выгонять из шеренги за плохо начищенные сапоги.
//Однако солдаты в процессе прихода и ухода должны были всегда быть выстроены по росту – сначала
//самые высокие,
//а в конце – самые низкие.За расстановку солдат отвечал прапорщик, который заметил интересную
//особенность – все солдаты в части разного роста.
//
//Ваша задача состоит в том, чтобы помочь прапорщику правильно расставлять солдат, а именно для
//каждого приходящего солдата указывать,
//перед каким солдатом в строе он должен становится.Требуемая скорость выполнения команды - O(log n)
//в среднем.
//
//В реализации используйте декартовы деревья.
//
//Формат ввода
//Первая строка содержит число N – количество команд(1 ≤ N ≤ 90 000).
//
//В каждой следующей строке содержится описание команды : число 1 и X если
//солдат приходит в строй(X – рост солдата, натуральное число до 100 000 включительно) и число 2 и Y
//если солдата,
//стоящим в строе на месте Y надо удалить из строя.Солдаты в строе нумеруются с нуля.
//
//Формат вывода
//На каждую команду 1 (добавление в строй) вы должны выводить число K – номер позиции, на которую
//должен встать этот солдат(все стоящие за ним двигаются назад).
/********************************************************************************************/
#include <iostream>

template <typename T>
class Treap
{

	class Node
	{
	public:
		T value;
		unsigned priority;
		int tree_size = 1;
		Node* left = nullptr;
		Node* right = nullptr;

		int size();

		Node(const T& init_key, unsigned init_priority)
			: value(init_key)
			, priority(init_priority)
		{
		}
		Node(const T& init_key)
			: Node(init_key, getPriority())
		{
		}
		~Node()
		{
			if (left)
			{
				delete left;
				left = nullptr;
			}
			if (right)
			{
				delete right;
				right = nullptr;
			}
		}
		Node(const Node&) = delete;
		Node& operator=(const Node&) = delete;
		Node& operator=(Node&&) = delete;


	private:
		unsigned getPriority();
	};

public:
	bool Insert(const T& val);
	bool Contains(const T& val);
	bool Erase(const T& val);


	int GetPosition(const T& val);
	T FindByPosition(int keyPos) const;

	int size() const
	{
		return (root != nullptr) ? root->tree_size : 0;
	}

	Treap()
		: root(nullptr)
	{
	};
	~Treap()
	{
		delete root;
	};
	Treap(const Treap&) = delete;
	Treap& operator=(const Treap&) = delete;
	Treap(Treap&&) = delete;
	Treap& operator=(const Treap&&) = delete;

private:
	Node* root;

	Node* _insert(Node* curNode, const T& value);
	Node* _find(Node* curNode, const T& value, int& keyPos) const;
	Node* _erase(Node* curNode, const T& val, bool& erased);

	std::pair<Treap::Node*, Treap::Node*> split(Node* curNode);
	Node* merge(Node* left, Node* right);

	T FindByPosition(Node* curNode, int keyPos) const;
};


template <typename T>
unsigned Treap<T>::Node::getPriority()
{
	return rand() ^ (rand() << 15);
}

template <typename T>
inline int Treap<T>::Node::size()
{
	int left_size = (left != nullptr) ? left->tree_size : 0;
	int right_size = (right != nullptr) ? right->tree_size : 0;

	return tree_size = left_size + 1 + right_size;
}

template <typename T>
std::pair<typename Treap<T>::Node*, typename Treap<T>::Node*> Treap<T>::split(Node* x)
{
	Node* subtree;
	Node* y;

	if (x->left != nullptr && x->left->priority > x->priority)
	{
		y = x->left;
		subtree = y->right;
		return { x, y->right };
	}
	else if (x->right != nullptr && x->right->priority < x->priority)
	{
		y = x->right;
		subtree = y->left;
		return { x, subtree };
	}
	return { x, nullptr };
}

template <typename T>
typename Treap<T>::Node* Treap<T>::merge(Node* x, Node* subtree)
{
	if (subtree == nullptr)
		return x;

	if (x->left != nullptr && x->left->priority > x->priority)
	{
		Node* y = x->left;
		y->right = x;
		x->left = subtree;

		x->size();
		y->size();

		return y;
	}
	else if (x->right != nullptr && x->right->priority < x->priority)
	{
		Node* y = x->right;
		y->left = x;
		x->right = subtree;

		x->size();
		y->size();

		return y;
	}
}

template <typename T>
bool Treap<T>::Contains(const T& value)
{
	int keyPos = 0;

	if (_find(root, value, keyPos) != nullptr)
	{
		return true;
	}
	else
	{
		return false;
	}
}

template <typename T>
typename Treap<T>::Node* Treap<T>::_find(Node* curNode, const T& value, int& keyPos) const
{
	if (curNode == nullptr)
		return nullptr;

	Node* answer = nullptr;
	int rootPos = 1;
	if (curNode->left != nullptr)
	{
		rootPos += curNode->left->tree_size;
	}
	if (value == curNode->value)
	{
		keyPos += rootPos;
		answer = curNode;
	}
	else if (value > curNode->value)
	{
		answer = _find(curNode->left, value, keyPos);
	}
	else if (value < curNode->value)
	{
		keyPos += rootPos;
		answer = _find(curNode->right, value, keyPos);
	}

	return answer;
}

template <typename T>
inline int Treap<T>::GetPosition(const T& value)
{
	int keyPos = 0;
	if (_find(root, value, keyPos) != nullptr)
	{
		return keyPos - 1;
	}
	else
	{
		return -1;
	}
}

template<typename T>
T Treap<T>::FindByPosition(int keyPos) const
{
	return FindByPosition(root, keyPos + 1);
}

template <typename T>
inline bool Treap<T>::Erase(const T& value)
{
	bool erased = false;
	root = _erase(root, value, erased);

	return erased;
}

template <typename T>
T Treap<T>::FindByPosition(Node* curNode, int keyPos) const
{
	int rootPos = 1;
	if (curNode->left != nullptr)
	{
		rootPos += curNode->left->tree_size;
	}

	if (keyPos == rootPos)
	{
		return curNode->value;
	}
	else if (keyPos < rootPos)
	{
		return FindByPosition(curNode->left, keyPos);
	}
	else
	{
		return FindByPosition(curNode->right, keyPos - rootPos);
	}
}

template <typename T>
typename Treap<T>::Node* Treap<T>::_insert(Node* curNode, const T& value)
{
	if (curNode == nullptr)
		return new Node(value);

	if (value > curNode->value)
	{
		curNode->left = _insert(curNode->left, value);
	}
	else
	{
		curNode->right = _insert(curNode->right, value);
	}

	auto splitted = split(curNode);
	curNode = merge(splitted.first, splitted.second);

	curNode->size();

	return curNode;
}


template <typename T>
inline bool Treap<T>::Insert(const T& value)
{
	if (Contains(value))
	{
		return false;
	}
	else
	{
		root = _insert(root, value);
		return true;
	}
}

template <typename T>
typename Treap<T>::Node* Treap<T>::_erase(Node* curNode, const T& value, bool& erased)
{
	if (curNode == nullptr)
		return nullptr;

	Node* left = curNode->left;
	Node* right = curNode->right;

	auto splitted = split(curNode);
	curNode = merge(splitted.first, splitted.second);

	if (value > curNode->value)
	{
		curNode->left = _erase(left, value, erased);
	}
	else if (value < curNode->value)
	{
		curNode->right = _erase(right, value, erased);
	}

	else if (left == nullptr && right == nullptr)
	{
		delete curNode;
		erased = true;
		curNode = nullptr;
	}
	else if (left == nullptr)
	{
		curNode->right = nullptr;
		delete curNode;
		erased = true;
		curNode = right;
	}
	else if (right == nullptr)
	{
		curNode->left = nullptr;
		delete curNode;
		erased = true;
		curNode = left;
	}
	else if (left->priority > right->priority)
	{
		curNode->right = _erase(curNode->right, value, erased);
	}
	else
	{
		curNode->left = _erase(curNode->left, value, erased);
	}

	if (curNode != nullptr)
		curNode->size();

	return curNode;
}


int main()
{
	Treap<int> tr;

	size_t n;
	std::cin >> n;

	int c;
	int v;
	for (size_t i = 0; i < n; i++)
	{
		std::cin >> c >> v;
		switch (c)
		{
		case 1:
			tr.Insert(v);
			std::cout << tr.GetPosition(v) << std::endl;
			break;
		case 2:
			tr.Erase(tr.FindByPosition(v));
			break;
		default:
			break;
		}
	}

	return 0;
}