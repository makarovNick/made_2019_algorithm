/*******************************************************/
//2_4.Скользящий максимум
//Ограничение времени	2 секунды
//Ограничение памяти	5Mb
//Ввод	стандартный ввод или input.txt
//Вывод	стандартный вывод или output.txt
//Дан массив целых чисел A[0..n), n не превосходит 100 000. 
//Так же задан размер некотрого окна(последовательно расположенных элементов массива) в этом массиве k, k <= n.
//Требуется для каждого положения окна(от 0 и до n - k) вывести значение максимума в окне.
//Скорость работы O(n log n), память O(n).
//Формат ввода
//Вначале вводится n - количество элементов массива.
//Затем вводится n строк со значением каждого элемента.
//Затем вводится k - размер окна.
//Формат вывода
//Разделенные пробелом значения максимумов для каждого положения окна.
/*******************************************************/
#include <iostream>
#include <vector>
#include <algorithm> 
class priority_queue {
private:
	struct node {
		int value;
		size_t id;
		node(int v, size_t ind) : value(v), id(ind) {};
	};
	std::vector<node> heap;
public:
	node max() const {
		return heap.front();
	}
	void push(int value, size_t index) {
		heap.push_back(*new node(value, index));
		std::push_heap(heap.begin(), heap.end(), 
			[](node n1, node n2) {return n1.value < n2.value; });
	}
	void pop() {
		std::pop_heap(heap.begin(), heap.end(),
			[](node n1, node n2) {return n1.value < n2.value; });
		heap.pop_back();
	}	
	size_t size() const { return heap.size(); }
};
std::vector<int> SlidingMax(const std::vector<int> & array, size_t wsize) {
	priority_queue hep;
	std::vector<int> result;
	size_t i = 0;
	for (; i < wsize; i++)
	{
		hep.push(array[i], i);
	}
	result.push_back(hep.max().value);
	for (; i < array.size(); i++)
	{
		while (hep.size() != 0 && i - hep.max().id + 1 > wsize )
		{
			hep.pop();
		}
		hep.push(array[i], i);
		result.push_back(hep.max().value);
	}
	return result;
}

int main() {
	priority_queue heap;
	size_t n, m;
	std::cin >> n;
	std::vector<int> array(n);
	std::vector<int> result;
	for (size_t i = 0; i < n; i++)
	{
		std::cin >> array[i];
	}
	std::cin >> m;
	result = SlidingMax(array, m);
	for (size_t i = 0; i < result.size(); i++)
	{
		std::cout << result[i] << " ";
	}
	return 0;
}
