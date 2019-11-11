/********************************************************************************************/
//7_1.Солдаты ДД
//Ограничение времени	1 секунда
//Ограничение памяти	500Mb
//Ввод	стандартный ввод или input.txt
//Вывод	стандартный вывод или output.txt
//В одной военной части решили построить в одну шеренгу по росту.Т.к.часть была далеко не образцовая, то солдаты часто приходили не вовремя, 
//а то их и вовсе приходилось выгонять из шеренги за плохо начищенные сапоги.
//Однако солдаты в процессе прихода и ухода должны были всегда быть выстроены по росту – сначала самые высокие, 
//а в конце – самые низкие.За расстановку солдат отвечал прапорщик, который заметил интересную особенность – все солдаты в части разного роста.
//
//Ваша задача состоит в том, чтобы помочь прапорщику правильно расставлять солдат, а именно для каждого приходящего солдата указывать, 
//перед каким солдатом в строе он должен становится.Требуемая скорость выполнения команды - O(log n) в среднем.
//
//В реализации используйте декартовы деревья.
//
//Формат ввода
//Первая строка содержит число N – количество команд(1 ≤ N ≤ 90 000).
//
//В каждой следующей строке содержится описание команды : число 1 и X если 
//солдат приходит в строй(X – рост солдата, натуральное число до 100 000 включительно) и число 2 и Y если солдата, 
//стоящим в строе на месте Y надо удалить из строя.Солдаты в строе нумеруются с нуля.
//
//Формат вывода
//На каждую команду 1 (добавление в строй) вы должны выводить число K – номер позиции, на которую должен встать этот солдат(все стоящие за ним двигаются назад).
/********************************************************************************************/
#include <iostream>

template <typename T>
class treap
{

	struct node
	{
		const T& value;
		unsigned priority;
		int tree_size = 1;
		node* left = nullptr;
		node* right = nullptr;

		int sizeOfLeft()
		{
			return (left != nullptr) ? left->tree_size : 0;
		}

		node(T init_key, unsigned init_priority)
			: value(init_key)
			, priority(init_priority)
		{
		}
		node(T init_key)
			: node(init_key, getPriority())
		{
		}
		~node()
		{
			delete left;
			delete right;
		}

	private:
		unsigned getPriority();
	};

public:
	bool insert(T val);
	bool contains(T val);
	bool erase(T val);
	int getPosition(T val);
	T findByPosition(int keyPos)
	{
		return findByPosition(root, keyPos + 1);
	}

	treap()
		: root(nullptr) {};
	~treap()
	{
		delete root;
	};

private:
	node* root;
	// implements split + merge
	node* rotateRight(node* x);
	// implements split + merge
	node* rotateLeft(node* y);
	node* _insert(node* curNode, T val);
	// finds position of value if treap contains it
	// with each step on tree change value of keyPos
	// returns node
	node* _find(node* curNode, T val, int& keyPos);
	// split + delete + merge
	node* _erase(node* curNode, T val, bool& erased);
	T findByPosition(node* curNode, int keyPos);
};


template <typename T>
unsigned treap<T>::node::getPriority()
{
	//static std::mt19937 engine{ std::random_device{}() };
	//static std::uniform_int_distribution<unsigned> distribution{
	//	std::numeric_limits<unsigned>::min(), std::numeric_limits<unsigned>::max()
	//};
	//return distribution(engine);

	return rand() ^ (rand() << 15);
}

template <typename T>
typename treap<T>::node* treap<T>::rotateRight(node* x)
{
	node* y = x->left;
	node* T2 = y->right;

	y->right = x;
	x->left = T2;

	return y;
}

template <typename T>
typename treap<T>::node* treap<T>::rotateLeft(node* y)
{
	node* x = y->right;
	node* T2 = x->left;

	x->left = y;
	y->right = T2;

	return x;
}

template <typename T>
typename treap<T>::node* treap<T>::_find(node* curNode, const T& value, int& keyPos)
{
	if (curNode == nullptr)
		return nullptr;

	node* answer = nullptr;
	int rootPos = 1 + curNode->sizeOfLeft();

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
inline bool treap<T>::contains(const T& value)
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
inline int treap<T>::getPosition(const T& value)
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

template <typename T>
inline bool treap<T>::erase(const T& value)
{
	bool erased = false;
	root = _erase(root, value, erased);

	return erased;
}

template <typename T>
T treap<T>::findByPosition(node* curNode, int keyPos)
{

	int rootPos = 1 + curNode->sizeOfLeft();

	if (keyPos == rootPos)
	{
		return curNode->value;
	}
	else if (keyPos < rootPos)
	{
		return findByPosition(curNode->left, keyPos);
	}
	else
	{
		return findByPosition(curNode->right, keyPos - rootPos);
	}
}

template <typename T>
typename treap<T>::node* treap<T>::_insert(node* curNode, const T& value)
{
	if (curNode == nullptr)
		return new node(value);

	if (value > curNode->value)
	{
		curNode->left = _insert(curNode->left, value);

		if (curNode->left->priority > curNode->priority)
			curNode = rotateRight(curNode);
	}
	else
	{
		curNode->right = _insert(curNode->right, value);

		if (curNode->right->priority > curNode->priority)
			curNode = rotateLeft(curNode);
	}

	return curNode;
}

template <typename T>
inline bool treap<T>::insert(const T& value)
{
	if (contains(value))
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
typename treap<T>::node* treap<T>::_erase(node* curNode, const T& value, bool& erased)
{
	if (curNode == nullptr)
		return nullptr;

	node* left = curNode->left;
	node* right = curNode->right;

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
		curNode = rotateRight(curNode);
		curNode->right = _erase(curNode->right, value, erased);
	}
	else
	{
		curNode = rotateLeft(curNode);
		curNode->left = _erase(curNode->left, value, erased);
	}

	return curNode;
}


int main()
{
	treap<int> tr;

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
			tr.insert(v);
			std::cout << tr.getPosition(v)<<std::endl;
			break;
		case 2:
			tr.erase(tr.findByPosition(v));
			break;
		default:
			break;
		}
	}

	return 0;
}