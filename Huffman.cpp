#include "Huffman.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

struct Node 
{    
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;
    char symbol;
    int freq;

    explicit Node(char symbol = 0, int freq = 0)
        : symbol(symbol)
        , freq(freq)
        , left(nullptr)
        , right(nullptr) 
    {
    }
};

using spNode = std::shared_ptr<Node>;

using encode_dict = std::unordered_map<char, long>;

class BitOutStream
{
public:

    BitOutStream(IOutputStream &stream);

    bool writeBit(bool value);

private:
    IOutputStream &stream;
    byte currentByte;
    char bitsWritten;
};

class BitInStream 
{
public:
    BitInStream(IInputStream &stream);

    bool readBit(bool &value);

private:
    IInputStream &stream;

    byte currentByte;
    char bitsRead;
};



struct freqComparator 
{
    bool operator()(const spNode &left, const spNode &right) const 
    {
        if (left->freq != right->freq) 
        {
            return left->freq > right->freq;
        } 
        else 
        {
            return left->symbol > right->symbol;
        }
    }
};

BitInStream::BitInStream(IInputStream &stream) 
    : stream(stream)
    , bitsRead(0) 
{
}

bool BitInStream::readBit(bool &value) 
{
    if (bitsRead == 0) 
    {
        byte tmp = 0;
        if (!stream.Read(tmp))
        {
            return false;
        }
        else 
        {
            currentByte = 0;
            for (int i = 0; i < 8; i++) 
            {
                currentByte <<= 1;
                currentByte |= 1 & tmp;
                tmp >>= 1;
            }
        }
    }
    value = currentByte & 1;
    currentByte >>= 1;
    bitsRead = (bitsRead + 1) % 8;

    return true;
}

BitOutStream::BitOutStream(IOutputStream &stream) 
    : stream(stream)
    , bitsWritten(0) 
{
}

bool BitOutStream::writeBit(bool value) 
{
    currentByte <<= 1;
    currentByte |= (value) ? 1 : 0;
    bitsWritten++;
    if (bitsWritten == 8) 
    {
        stream.Write(currentByte);
        bitsWritten = 0;
     
        return true;
    }
    return false;
}

spNode makeTree(const std::unordered_map<byte, long> &alphabet) 
{
    std::vector<spNode> frequncies;
    for (auto leter : alphabet) 
    {
        if (leter.second != 0) 
        {
            frequncies.push_back(std::make_shared<Node>(leter.first, leter.second));
        }
    }
    std::priority_queue<spNode, std::vector<spNode>, freqComparator>
            queue_for_min_freq(frequncies.begin(), frequncies.end());
    while (queue_for_min_freq.size() >= 2) 
    {
        auto first = queue_for_min_freq.top();
        queue_for_min_freq.pop();
        auto second = queue_for_min_freq.top();
        queue_for_min_freq.pop();

        auto new_node =
                std::make_shared<Node>(0, first->freq + second->freq);
        new_node->left = first;
        new_node->right = second;
        queue_for_min_freq.push(new_node);
    }

    return queue_for_min_freq.top();
}

void inOrderTraversal(const spNode &head, long code, encode_dict &dict) 
{
    if (head->right == nullptr && head->left == nullptr) 
    {
        long result = 1;
        while (code != 1) 
        {
            result <<= 1;
            result |= code & 1;
            code >>= 1;
        }

        dict[head->symbol] = result != 1 ? result : 2;
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

std::unordered_map<byte, char> getCodesLength(const encode_dict &dict) 
{
    std::unordered_map<byte, char> result;
    long code = 0;
    for (auto i : dict) 
    {
        result[i.first] = 0;
        if (i.second != 0) 
        {
            code = i.second;
            while (code != 1) 
            {
                result[i.first]++;
                code >>= 1;
            }
        }
    }
    return result;
}

char getLongestCode(const std::unordered_map<byte, char> &lens) 
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

void deleteEmptyNode(spNode node) 
{
    if (node == nullptr) 
    {
        return;
    }

    if (node->left != nullptr && node->right == nullptr) 
    {
        node->left = nullptr;
        return;
    }

    deleteEmptyNode(node->left);
    deleteEmptyNode(node->right);
}

void decodeHelper(       IOutputStream &original
                 , const spNode        &head
                 , const std::string   &text
                 , const byte          rest_bits) 
{
    spNode cur_node = head;
    for (int i = 0; i < text.length() - 1; i++)
    {
        for (int j = 7; j >= 0; j--) 
        {
            if (((text[i] >> j) & 1) == 0) 
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
    for (int j = 7; j >= rest_bits; j--) 
    {
        if (((text[text.length() - 1] >> j) & 1) == 0) 
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

void Encode(IInputStream &original, IOutputStream &compressed) 
{
    std::unordered_map<byte, long> alphabet;
    encode_dict result;

    std::string text;
    byte symbol = 0;
    while (original.Read(symbol)) 
    {
        alphabet[symbol]++;
        text += symbol;
    }

    spNode tree = makeTree(alphabet);
    inOrderTraversal(tree, 1, result);

    std::unordered_map<byte, char> len_of_code = getCodesLength(result);
    char max_len = getLongestCode(len_of_code);

    size_t encode_size = 0;
    byte alphabet_size = 0;

    for (auto i : alphabet) 
    {
        if (i.second != 0) 
        {
            alphabet_size++;
        }

        encode_size = (encode_size + i.second * len_of_code[i.first]) % 8;
    }

    byte rest_bits = (8 - encode_size) % 8;
    byte bytes_in_code = (max_len % 8 == 0) ? (max_len / 8) : (max_len / 8 + 1);

    BitOutStream bout(compressed);
    compressed.Write(alphabet_size);
    compressed.Write(rest_bits);
    compressed.Write(bytes_in_code);

    long code = 0;
    int j = 0;
    for (auto i : result) 
    {
        if (alphabet[i.first] != 0) 
        {
            code = i.second;
            compressed.Write(byte(i.first));

            while (code != 1) 
            {
                bout.writeBit(code & 1);
                code >>= 1;
                j++;
            }
            for (; j < bytes_in_code * 8; j++) 
            {
                bout.writeBit(0);
            }
        }
        j = 0;
    }

    for (auto i : text) 
    {
        code = result[i];

        while (code != 1) 
        {
            bout.writeBit(code & 1);
            code >>= 1;
        }
    }

    for (size_t i = 0; i < rest_bits; i++) 
    {
        bout.writeBit(0);
    }
}

void Decode(IInputStream &compressed, IOutputStream &original) 
{
    byte alphabet_size;
    byte rest_bits;
    byte bytes_in_code;

    compressed.Read(alphabet_size);
    compressed.Read(rest_bits);
    compressed.Read(bytes_in_code);

    auto head = std::make_shared<Node>(0, 0);
    BitInStream bin(compressed);

    byte symbol = 0;
    bool cur_bit;

    for (int i = 0; i < alphabet_size; i++) 
    {
        auto cur_node = head;
        compressed.Read(symbol);

        for (int j = 0; j < 8 * bytes_in_code; j++) 
        {
            bin.readBit(cur_bit);
            if (cur_bit == 0) 
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

    deleteEmptyNode(head);

    std::string text;
    while (compressed.Read(symbol)) 
    {
        text += symbol;
    }

    decodeHelper(original, head, text, rest_bits);
}