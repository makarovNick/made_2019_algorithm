/*******************************************************/
// 4. Порядковая статистика
//Ограничение времени	1 секунда
//Ограничение памяти	16Mb
//Ввод	стандартный ввод или input.txt
//Вывод	стандартный вывод или output.txt
//Даны неотрицательные целые числа n, k и массив целых чисел из
//диапазона[0..109] размера n.
//Требуется найти k - ю порядковую статистику.т.е.напечатать число, которое бы
//стояло на позиции с индексом k ∈[0..n - 1] в отсортированном массиве.
//Напишите нерекурсивный алгоритм.
//Требования к дополнительной памяти : O(n).
//Требуемое среднее время работы : O(n).
//Функцию Partition следует реализовывать методом прохода двумя итераторами в
//одном направлении.
//Описание для случая прохода от начала массива к концу :
//Выбирается опорный элемент.
//Опорный элемент меняется с последним элементом массива.
//Во время работы Partition в начале массива содержатся элементы, не бОльшие
//опорного.Затем располагаются элементы, строго бОльшие опорного.В конце массива
//лежат нерассмотренные элементы.Последним элементом лежит опорный.
//Итератор(индекс) i указывает на начало группы элементов, строго бОльших
//опорного.
//Итератор j больше i, итератор j указывает на первый нерассмотренный элемент.
//Шаг алгоритма.Рассматривается элемент, на который указывает j.Если он больше
//опорного, то сдвигаем j.Если он не больше опорного, то меняем a[i] и a[j]
//местами, сдвигаем i и сдвигаем j.
//В конце работы алгоритма меняем опорный и элемент, на который указывает
//итератор i.
/*******************************************************/
#include <iostream>

int* MedianOfThree(int* first, int* last) {
	int* middle = first + ((last - first) / 2);

	if (*first < *middle) {
		if (*middle < *last) {
			return middle;
		}
		else if (*first < *last) {
			return last;
		}
		else {
			return first;
		}
	}
	else {
		if (*first < *last) {
			return first;
		}
		else if (*middle < *last) {
			return last;
		}
		else {
			return middle;
		}
	}
}
int* Partition(int* begin, int* end) {
	int* pivot = MedianOfThree(begin, end);

	std::iter_swap(begin, pivot);

	int* i = end;
	int* j = end;

	for (; j > begin; --j) {
		if (*j >= *begin) {
			std::iter_swap(i, j);
			--i;
		}
	}

	std::iter_swap(begin, i);

	return i;
}
int FindKStatistics(int* begin, int* end, size_t index) {
	int* first = begin;
	int* last = end;

	while (true) {
		int* pivot = Partition(first, last);
		int pivot_element_position = pivot - begin;

		if (pivot_element_position == index) {
			return *(begin + index);
		}
		else if (pivot_element_position > index) {
			last = pivot;
		}
		else {
			first = pivot + 1;
		}
	}
}
int main() {
	int n;
	std::cin >> n;

	size_t index;
	std::cin >> index;
	int* arr = new int[n];
	for (int i = 0; i < n; ++i) {
		int item;
		std::cin >> item;
		arr[i] = item;
	}

	std::cout << FindKStatistics(arr, arr + n - 1, index);
	delete[] arr;
	return 0;
}
