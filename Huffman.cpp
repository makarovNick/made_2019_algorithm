/*******************************************************/
// A. Алгоритм сжатия данных Хаффмана
// Ограничение времени	100 секунд
// Ограничение памяти	64Mb
// Ввод	стандартный ввод или input.txt
// Вывод	стандартный вывод или output.txt
// Реализуйте алгоритм построения оптимального префиксного кода Хаффмана. 
// При помощи алгоритма реализуйте две функции для создания архива из одного файла и извлечения файла из архива.
//
// // Метод архивирует данные из потока original
// void Encode(IInputStream& original, IOutputStream& compressed);
// // Метод восстанавливает оригинальные данные
// void Decode(IInputStream& compressed, IOutputStream& original);
//  где:
// typedef unsigned char byte;
//
// struct IInputStream {
// 	// Возвращает false, если поток закончился
// 	bool Read(byte& value) = 0;
// };
//
// struct IOutputStream {
// 	void Write(byte value) = 0;
// };
//
// В контест необходимо отправить .cpp файл содержащий функции Encode, Decode, а также включающий файл Huffman.h. 
// Тестирующая программа выводит скоринг зависящий от соотношения размера сжатого файла и исходного.
/*******************************************************/

#include "Huffman.h"

#include <queue>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

struct Node
{
	std::shared_ptr<Node> left;
	std::shared_ptr<Node> right;

	char symbol;
	int count;

	Node() = delete;
	~Node() = default;
	Node(Node&&) = delete;
	Node(const Node&) = delete;
	Node& operator=(Node&&) = delete;
	Node& operator=(const Node&) = delete;

	Node(char symbol = 0, int count = 0);
};

class BitOutStream
{
public:
	BitOutStream(IOutputStream& stream);

	bool writeBit(bool value);

private:
	IOutputStream& stream;

	byte cur_byte;
	char bits_written;
};

class BitInStream
{
public:
	BitInStream(IInputStream& stream);

	bool readBit(bool& value);

private:
	IInputStream& stream;

	byte cur_byte;
	char bits_read;
};

using sp_node = std::shared_ptr<Node>;
using codes_dict = std::unordered_map<char, long>;
using alphabet_dict = std::unordered_map<byte, long>;
using length_dict = std::unordered_map<byte, char>;

Node::Node(char symbol, int count)
	: symbol(symbol)
	, count(count)
	, left(nullptr)
	, right(nullptr)
{
}

BitInStream::BitInStream(IInputStream& stream)
	: stream(stream)
	, bits_read(0)
{
}
BitOutStream::BitOutStream(IOutputStream& stream)
	: stream(stream)
	, bits_written(0)
{
}

bool BitOutStream::writeBit(bool value)
{
	cur_byte <<= 1;
	cur_byte |= (value) ? 1 : 0;

	bits_written++;

	if (bits_written == 8)
	{
		stream.Write(cur_byte);
		bits_written = 0;

		return true;
	}

	return false;
}

bool BitInStream::readBit(bool& value)
{
	if (bits_read == 0)
	{
		byte tmp = 0;
		if (!stream.Read(tmp))
		{
			return false;
		}
		else
		{
			cur_byte = 0;
			for (int i = 0; i < 8; i++)
			{
				cur_byte <<= 1;
				cur_byte |= 1 & tmp;

				tmp >>= 1;
			}
		}
	}
	value = cur_byte & 1;

	cur_byte >>= 1;
	bits_read = (bits_read + 1) % 8;

	return true;
}

struct CountComp
{
	bool operator()(const sp_node& left, const sp_node& right) const
	{
		if (left->count != right->count)
		{
			return left->count > right->count;
		}
		else
		{
			return left->symbol > right->symbol;
		}
	}
};
//makes heap in count priority 
sp_node makeHeap(const alphabet_dict& alphabet)
{
	std::vector<sp_node> counts;
	for (auto leter : alphabet)
	{
		if (leter.second != 0)
		{
			counts.push_back(std::make_shared<Node>(leter.first, leter.second));
		}
	}

	std::priority_queue<sp_node, std::vector<sp_node>, CountComp>
		CountsHeap(counts.begin(), counts.end());

	while (CountsHeap.size() > 1)
	{
		auto first = CountsHeap.top();
		CountsHeap.pop();

		auto second = CountsHeap.top();
		CountsHeap.pop();

		auto new_node = std::make_shared<Node>(0, first->count + second->count);
		new_node->left = first;
		new_node->right = second;

		CountsHeap.push(new_node);
	}

	return CountsHeap.top();
}
//fils in dict
void inOrderTraversal(const sp_node& head, long code, codes_dict& dict)
{
	if (head->right == nullptr && head->left == nullptr)
	{
		long res = 1;
		//coding symbols
		while (code != 1)
		{
			res <<= 1;
			res |= code & 1;
			code >>= 1;
		}

		dict[head->symbol] = res != 1 ? res : 2;
		return;
	}

	if (head->left != nullptr)
	{
		inOrderTraversal(head->left, code << 1, dict);
	}
	if (head->right != nullptr)
	{
		inOrderTraversal(head->right, (code << 1) + 1, dict);
	}
}

length_dict getCodesLength(const codes_dict& dict)
{
	length_dict length;

	long code = 0;
	for (auto i : dict)
	{
		length[i.first] = 0;
		if (i.second != 0)
		{
			code = i.second;
			while (code != 1)
			{
				length[i.first]++;
				code >>= 1;
			}
		}
	}
	return length;
}

char getLongestCode(const length_dict& lens)
{
	char max = 0;
	for (auto i : lens)
	{
		if (i.second > max)
		{
			max = i.second;
		}
	}

	return max;
}

void deleteEmpty(sp_node node)
{
	if (!node)
	{
		return;
	}

	if (node->left && !node->right)
	{
		node->left = nullptr;
		return;
	}

	deleteEmpty(node->left);
	deleteEmpty(node->right);
}

void decodeRaw(		IOutputStream&	original
		, const sp_node&	head
		, const std::string&	data
		, const byte		remaining_bits)
{
	sp_node cur_node = head;

	for (int i = 0; i < data.length() - 1; i++)
	{
		for (int j = 7; j >= 0; j--)
		{
			if (((data[i] >> j) & 1) == 0)
			{
				cur_node = cur_node->left;
			}
			else
			{
				cur_node = cur_node->right;
			}
			if (cur_node->left == nullptr)
			{
				original.Write(cur_node->symbol);

				cur_node = head;
			}
		}
	}
	for (int j = 7; j >= remaining_bits; j--)
	{
		if (((data[data.length() - 1] >> j) & 1) == 0)
		{
			cur_node = cur_node->left;
		}
		else
		{
			cur_node = cur_node->right;
		}
		if (cur_node->left == nullptr)
		{
			original.Write(cur_node->symbol);
			cur_node = head;
		}
	}
}

void Encode(IInputStream& original, IOutputStream& compressed)
{
	alphabet_dict alphabet;
	codes_dict res;

	std::string data;
	byte symbol = 0;
	while (original.Read(symbol))
	{
		alphabet[symbol]++;
		data += symbol;
	}

	sp_node tree = makeHeap(alphabet);
	inOrderTraversal(tree, 1, res);

	length_dict code_len = getCodesLength(res);
	char max_len = getLongestCode(code_len);

	size_t codes_len = 0;
	byte alphabet_len = 0;

	for (auto i : alphabet)
	{
		if (i.second != 0)
		{
			alphabet_len++;
		}

		codes_len = (codes_len + i.second * code_len[i.first]) % 8;
	}

	byte remaining_bits = (8 - codes_len) % 8;
	byte code_bytes = (max_len % 8 == 0) ? (max_len / 8) : (max_len / 8 + 1);

	BitOutStream bos(compressed);

	compressed.Write(alphabet_len);
	compressed.Write(remaining_bits);
	compressed.Write(code_bytes);

	long code = 0;
	int j = 0;

	for (auto i : res)
	{
		if (alphabet[i.first] != 0)
		{
			code = i.second;
			compressed.Write(byte(i.first));

			while (code != 1)
			{
				bos.writeBit(code & 1);
				code >>= 1;
				j++;
			}
			for (; j < code_bytes * 8; j++)
			{
				bos.writeBit(0);
			}
		}
		j = 0;
	}

	for (auto i : data)
	{
		code = res[i];

		while (code != 1)
		{
			bos.writeBit(code & 1);
			code >>= 1;
		}
	}

	for (size_t i = 0; i < remaining_bits; i++)
	{
		bos.writeBit(0);
	}
}

void Decode(IInputStream& compressed, IOutputStream& original)
{
	byte alphabet_len;
	byte remaining_bits;
	byte code_bytes;

	compressed.Read(alphabet_len);
	compressed.Read(remaining_bits);
	compressed.Read(code_bytes);

	auto head = std::make_shared<Node>(0, 0);
	BitInStream bis(compressed);

	byte symbol = 0;
	bool bit;
	//firstly fill alphabet and codes
	for (int i = 0; i < alphabet_len; i++)
	{
		auto cur_node = head;
		compressed.Read(symbol);

		for (int j = 0; j < 8 * code_bytes; j++)
		{
			bis.readBit(bit);
			if (bit == 0)
			{
				if (cur_node->left == nullptr)
				{
					cur_node->left = std::make_shared<Node>(symbol, 0);
				}
				cur_node = cur_node->left;
			}
			else
			{
				if (cur_node->right == nullptr)
				{
					cur_node->right = std::make_shared<Node>(symbol, 0);
				}
				cur_node = cur_node->right;
			}
		}
	}

	deleteEmpty(head);

	std::string data;
	while (compressed.Read(symbol))
	{
		data += symbol;
	}
	//decode raw data with alphabet
	decodeRaw(original, head, data, remaining_bits);
}
