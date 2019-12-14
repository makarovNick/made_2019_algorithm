#include <algorithm>
#include <iostream>
#include <numeric>
#include <memory>
#include <vector>
#include <string>
#include <map>

class Bor 
{
    struct Node 
    {
        bool is_terminal = false;
        char character;

        size_t this_lenght = 0;
        std::vector<size_t> template_indexes;

        Node* parent;
        Node* suff_link;
        Node* shortened_suff_link;

        std::map<char, std::shared_ptr<Node>> sons;
        std::map<char, Node*> links;

        Node()
            : suff_link(nullptr)
            , parent(nullptr)
            , shortened_suff_link(nullptr)
        {
        }
    };

public:
	Bor()
        : root(std::shared_ptr<Node>(new Node))
    {
    }

	void Add(const std::string& input_str);
	std::vector<int> Find(const std::string& text);

private:

    std::string temp;

	Node* getLink(Node* node, const char character);
	Node* getSuffLink(Node* node);
	Node* getShortenedSL(Node* node);

	std::shared_ptr<Node> root;
	size_t times_in = 0;
	size_t size_ = 0;
};

void Bor::Add(const std::string& str) 
{
	Node* node = root.get();
	size_t position = 0;
	size_t lenght = 0;
	size_ = str.length();
    temp = str;

	for (size_t i = 0; i < str.length(); i++) 
    {
		if (str[i] != '?') 
        {
			lenght++;

			if (node == root.get()) 
            {
				position = i;
			}

			if (node->sons.find(str[i]) == node->sons.end()) 
            {
				std::shared_ptr<Node> next_node(new Node);
				next_node->parent = node;
				next_node->character = str[i];
				node->sons[str[i]] = next_node;
			}
			node = node->sons[str[i]].get();
		}

		if ((i >= 1 && str[i - 1] != '?' && str[i] == '?') 
            || (i == str.length() - 1 && str[i] != '?')) 
        {
			node->template_indexes.push_back(position);
			node->is_terminal = true;
			node->this_lenght = lenght;
            times_in++;
			
            lenght = 0;
			node = root.get();
		}

	}
}

Bor::Node* Bor::getLink(Node* node, const char character) 
{
	if (node->links.find(character) == node->links.end()) 
    {
	
		if (node->sons.find(character) != node->sons.end()) 
        {
			node->links[character] = node->sons[character].get();
		}
		else if (node == root.get()) 
        {
			node->links[character] = root.get();
		}
		else 
        {
			node->links[character] = getLink(getSuffLink(node), character);
		}
	}

	return node->links[character];
}

Bor::Node* Bor::getSuffLink(Node* node) 
{
	if (!node->suff_link) 
    {
		if (node != root.get() && node->parent != root.get()) 
        {
            node->suff_link = getLink(getSuffLink(node->parent), node->character);
		}
		else 
        {
            node->suff_link = root.get();
		}
	}

	return node->suff_link;
}

Bor::Node* Bor::getShortenedSL(Node* node) 
{
	if (!node->shortened_suff_link) 
    {
		if (getSuffLink(node)->is_terminal) 
        {
			node->shortened_suff_link = getSuffLink(node);
		}
		else if (node == root.get()) 
        {
			node->shortened_suff_link = node;
		}
		else 
        {
			node->shortened_suff_link = getShortenedSL(getSuffLink(node));
		}
	}

	return node->shortened_suff_link;
}

std::vector<int> Bor::Find(const std::string& text) 
{
	std::vector<int> patterns(text.length());
	std::vector<int> result;
    Node* node = root.get();
    int index;

    if(std::all_of(temp.begin(), temp.end(), [](char c){return c=='?';}))
    {
        result.resize(text.size() - temp.length() + 1);
        std::iota(result.begin(), result.end(), 0);
        return result;
    }

	for (size_t i = 0; i < text.length(); i++) 
    {
		node = getLink(node, text[i]);
        Node* new_node = node;
        index = 0;

        while (new_node != root.get()) 
        {
            if (!new_node->is_terminal) 
            {
                new_node = getShortenedSL(new_node);
                continue;
            }
            for (size_t j = 0; j < new_node->template_indexes.size(); j++) 
            {
                index = i - new_node->this_lenght - new_node->template_indexes[j] + 1;

                if (index >= 0 && ++patterns[index] == times_in && (index + size_ <= patterns.size())) 
                {
                    result.push_back(index);
                }
            }
            new_node = getShortenedSL(new_node);
        }
	}

    return result;
}

int main() 
{
	Bor bor;

	std::string string, pattern;
	std::cin >> string;
    bor.Add(string);
    std::cin >> pattern;

	auto result = bor.Find(pattern);

    for(auto i : result)
    {
        std::cout << i << " ";
    }

	return 0;
}