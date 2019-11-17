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

class MyHash{
public:
	size_t operator()(const std::string& key, size_t capacity) const
	{
		size_t _hash = 0;

		for (size_t i = 0; i < key.length(); i++)
		{
			_hash = (_hash + 31 * key[i]) % capacity;
		}

		return _hash;
	}
};

template<typename T, typename Hash = MyHash>
class HashTable
{
	const double ALPHA = 0.75;
	const size_t INIT_SIZE = 8;

public:

	HashTable();
	~HashTable();
	HashTable& operator =(HashTable&&) = delete;
	HashTable (HashTable&&) = delete;
	HashTable(const HashTable&) = delete;
	HashTable& operator =(HashTable&) = delete;

	bool Insert(const T& key);
	bool Remove(const T& key);
	bool Has(const T& key) const;

private:
	struct Node
	{
		Node(const T& key);
		T data;
	};

	void Rehash();

	size_t capacity;
	size_t size;
	const std::string deleted = "!@#$%^__DELETED_^%$#@!";
	Node** table;
};

template<typename T, typename Hash>
HashTable<T, Hash>::HashTable()
	: capacity(INIT_SIZE)
	, size(0)
{
	table = new Node * [capacity];
	for (size_t i = 0; i < capacity; i++)
	{
		table[i] = nullptr;
	}
}

template<typename T, typename Hash>
HashTable<T, Hash>::~HashTable()
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

template<typename T, typename Hash>
bool HashTable<T, Hash>::Has(const T& key) const
{
	Hash hasher;
	size_t _hash = hasher(key, capacity);
	size_t i = 0;

	while (table[_hash] != nullptr && i < capacity)
	{
		if (table[_hash]->data.compare(deleted) && table[_hash]->data.compare(key) == 0)
		{
			return true;
		}
		++i;
		_hash = (_hash + i) % capacity;
	}

	return false;
}

template<typename T, typename Hash>
bool HashTable<T, Hash>::Insert(const T& key)
{

	if ((size / (double)capacity) >= ALPHA)
	{
		Rehash();
	}

	Hash hasher;
	size_t _hash = hasher(key, capacity);
	size_t i = 0;

	while (i < capacity)
	{
		if (table[_hash] == nullptr)
		{
			table[_hash] = new Node(key);
			size++;
			return true;
		}

		if (table[_hash]->data.compare(deleted) && table[_hash]->data.compare(key) == 0)
		{
			return false;
		}

		else if (table[_hash]->data.compare(deleted))
		{
			table[_hash]->data = key;
			size++;
			return true;
		}
		++i;
		// Quadratic probing
		_hash = (_hash + i) % capacity;
	}
	return false;
}

template<typename T, typename Hash>
HashTable<T, Hash>::Node::Node(const T& key)
	: data(key)
{
}

template<typename T, typename Hash>
bool HashTable<T, Hash>::Remove(const T& key)
{
	Hash hasher;
	size_t _hash = hasher(key, capacity);
	size_t i = 0;

	while (i < capacity)
	{
		if (table[_hash]->data.compare(deleted) && table[_hash]->data.compare(key) == 0)
		{
			table[_hash]->data = deleted;
			size--;
			return true;
		}
		++i;
		_hash = (_hash + i) % capacity;
	}
	return false;
}

template<typename T, typename Hash>
void HashTable<T, Hash>::Rehash()
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
			if (tmp[i]->data.compare(deleted))
				Insert(tmp[i]->data);
			delete tmp[i];
		}
	}

	delete[] tmp;
}

int main()
{
	HashTable<std::string> table;

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