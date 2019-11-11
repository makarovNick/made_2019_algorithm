/*******************************************************/
// 8. Хеш - таблица
//Ограничение времени	0.2 секунды
//Ограничение памяти	15Mb
//Ввод	стандартный ввод или input.txt
//Вывод	стандартный вывод или output.txt
//Реализуйте структуру данных типа “множество строк” на основе динамической хеш - таблицы с открытой
//адресацией.
//Хранимые строки непустые и состоят из строчных латинских букв.
//Хеш - функция строки должна быть реализована с помощью вычисления значения многочлена методом
//Горнера.
//Начальный размер таблицы должен быть равным 8 - ми.
//Перехеширование выполняйте при добавлении элементов в случае, когда коэффициент заполнения таблицы
//достигает 3 / 4.
//Структура данных должна поддерживать операции добавления строки в множество,
//удаления строки из множества и проверки принадлежности данной строки множеству.
// 1_1.Для разрешения коллизий используйте квадратичное пробирование.
// i - ая проба g(k, i) = g(k, i - 1) + i(mod m).m - степень двойки.
// 1_2.Для разрешения коллизий используйте двойное хеширование.
/*******************************************************/
#include <iostream>
#include <string>

class HashTable
{
	const double ALPHA = 0.75;
	const size_t INIT_SIZE = 8;

public:
	HashTable();
	~HashTable();

	bool Insert(const std::string& key);
	bool Remove(const std::string& key);
	bool Has(const std::string& key) const;

private:
	struct Node
	{
		Node(const std::string& key);
		std::string data;
		bool deleted;
	};

	void Rehash();
	size_t Hash(const std::string& key) const;

	size_t capacity;
	size_t size;
	Node** table;
};

HashTable::Node::Node(const std::string& key)
	: data(key)
	, deleted(false)
{
}

HashTable::HashTable()
	: capacity(INIT_SIZE)
	, size(0)
{
	table = new Node * [capacity];
	for (size_t i = 0; i < capacity; i++)
	{
		table[i] = nullptr;
	}
}

HashTable::~HashTable()
{
	for (size_t i = 0; i < capacity; ++i)
	{
		if (table[i] != nullptr)
		{
			delete table[i];
		}
	}
	delete[] table;
}


bool HashTable::Has(const std::string& key) const
{
	size_t _hash = Hash(key);
	size_t i = 0;

	while (table[_hash] != nullptr && i < capacity)
	{
		if (!table[_hash]->deleted && table[_hash]->data.compare(key) == 0)
		{
			return true;
		}
		++i;
		_hash = (_hash + i) % capacity;
	}

	return false;
}

bool HashTable::Insert(const std::string& key)
{
	if (Has(key))
	{
		return false;
	}

	if ((size / (double)capacity) >= ALPHA)
	{
		Rehash();
	}

	size_t _hash = Hash(key);
	size_t i = 0;

	while (i < capacity)
	{
		if (table[_hash] == nullptr)
		{
			table[_hash] = new Node(key);
			size++;
			return true;
		}
		else if (table[_hash]->deleted)
		{
			table[_hash]->data = key;
			table[_hash]->deleted = false;
			size++;
			return true;
		}
		++i;
		// Quadratic probing
		_hash = (_hash + i) % capacity;
	}
	return false;
}

bool HashTable::Remove(const std::string& key)
{
	if (!Has(key))
	{
		return false;
	}

	size_t _hash = Hash(key);
	size_t i = 0;

	while (i < capacity)
	{
		if (!table[_hash]->deleted && table[_hash]->data.compare(key) == 0)
		{
			table[_hash]->deleted = true;
			size--;
			return true;
		}
		++i;
		_hash = (_hash + i) % capacity;
	}
	return false;
}

void HashTable::Rehash()
{
	Node** tmp = table;
	capacity *= 2;
	size = 0;
	table = new Node * [capacity];

	for (size_t i = 0; i < capacity; ++i)
	{
		table[i] = nullptr;
	}

	for (size_t i = 0; i < capacity / 2; ++i)
	{
		if (tmp[i] != nullptr)
		{
			if (!tmp[i]->deleted)
				Insert(tmp[i]->data);
			delete tmp[i];
		}
	}

	delete[] tmp;
}


size_t HashTable::Hash(const std::string& key) const
{
	size_t _hash = 0;

	for (size_t i = 0; i < key.length(); i++)
	{
		_hash = (_hash + 31 * key[i]) % capacity;
	}

	return _hash;
}

int main()
{
	HashTable table;

	char command;
	std::string key;

	while (std::cin >> command >> key)
	{
		switch (command)
		{
		case '?':
			std::cout << (table.Has(key) ? "OK" : "FAIL") << std::endl;
			break;
		case '+':
			std::cout << (table.Insert(key) ? "OK" : "FAIL") << std::endl;
			break;
		case '-':
			std::cout << (table.Remove(key) ? "OK" : "FAIL") << std::endl;
			break;
		default:
			break;
		}
	}

	return 0;
}