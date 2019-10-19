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

struct Heap
{
	size_t length;
	size_t size;
	
	struct node {
		int value;
		size_t index;
		node() : value(-2147483647), index(0) {};
		node(int v, size_t i) : value(v), index(i) {}
	};
	node* array;
	~Heap() {
		delete array;
		array = nullptr;
	}
	Heap() {
		array = new node[8];
		size = 8;
		length = 0;
	}

	void siftDown(size_t);
	void popTop();
	void siftUp(size_t);
	void insert(const node &);

	node getMax() const { return array[0]; };
	void resize();
};
void Heap::siftDown(size_t index)
{
	size_t leftChildIndex = 2 * index + 1;
	size_t rightChildIndex = 2 * index + 2;

	if (leftChildIndex >= length)
		return; 

	size_t maxIndex = index;

	if (array[index].value < array [leftChildIndex].value)
	{
		maxIndex = leftChildIndex;
	}

	if ((rightChildIndex < length) && (array[maxIndex].value < array[rightChildIndex].value))
	{
		maxIndex = rightChildIndex;
	}

	if (maxIndex != index)
	{
		std::swap(array[index], array[maxIndex]);
		siftDown(maxIndex);
	}
}

void Heap::popTop()
{
	array[0].value = -2147483647;
	siftDown(0);
}

void Heap::siftUp(size_t index)
{
	if (index == 0)
		return;

	size_t parentIndex = (index - 1) / 2;

	if (array[parentIndex].value < array[index].value)
	{
		std::swap(array[index], array[parentIndex]);
		siftUp(parentIndex);
	}
}

void Heap::insert(const Heap::node & newValue)
{
	if (length + 1 == size)
		resize();
	array[length] = newValue;

	siftUp(length);
	length++;
}
void Heap::resize()
{
	node* temp = new node[size * 2];
	for (size_t i = 0; i < length; i++)
	{
		temp[i] = array[i];
	}
	delete array;

	size *= 2;
	array = temp;
}
int* SlidingMax(size_t size, const int array[], size_t wsize) {
	Heap heap;
	int* result = new int[size - wsize + 1];
	size_t i = 0;
	size_t j = 0;
	for (; i < wsize; i++)
	{
		heap.insert(*(new Heap::node(array[i], i)));
	}
	result[j++] = heap.getMax().value;
	for (; i < size; i++)
	{
		size_t temp = heap.getMax().index;
		while (i - heap.getMax().index + 1 > wsize)
		{
			heap.popTop();
			if (temp == heap.getMax().index) break;
		}
		heap.insert(*(new Heap::node(array[i], i)));
		result[j++] = heap.getMax().value;
	}
	return result;
}

int main()
{
	size_t n;
	size_t k;
	std::cin >> n;
	int arr[n];
	for (size_t i = 0; i < n; i++)
	{
		std::cin >> arr[i];
	}
	std::cin >> k;
	int* res = SlidingMax(n, arr, k);
	for (size_t i = 0; i < n - k + 1; i++)
	{
		std::cout << res[i] << " ";
	}
	delete res;
	return 0;

}
