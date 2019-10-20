/*******************************************************/
//3_2.Сортировка почти упорядоченной последовательности
//Ограничение времени	2 секунды
//Ограничение памяти	20Mb
//Ввод	стандартный ввод или input.txt
//Вывод	стандартный вывод или output.txt
//Дана последовательность целых чисел a1...an и натуральное число k, такое что для любых i, j: если j >= i + k, то a[i] <= a[j].
//Требуется отсортировать последовательность.Последовательность может быть очень длинной.
//Время работы O(n* log(k)).Доп.память O(k).Использовать слияние.
/*******************************************************/
#include <iostream>
#include <cstring>
void Merge(int* a, size_t a_len, int* b, size_t b_len, int* out) {
	size_t i = 0, j = 0;
	while (i < a_len && j < b_len) {
		if (a[i] < b[j]) {
			out[i + j] = a[i];
			i++;
		}
		else {
			out[i + j] = b[j];
			j++;
		}
	}

	if (i == a_len) {
		for (; j < b_len; j++) out[i + j] = b[j];
	}
	else {
		for (; i < a_len; i++) out[i + j] = a[i];
	}
}

void MergeSort(int* arr, size_t len) {
	if (len <= 1) return;

	size_t firstLen = len / 2;
	size_t secondLen = len - firstLen;

	MergeSort(arr, firstLen);
	MergeSort(arr + firstLen, secondLen);

	int* tempArray = new int[len];
	Merge(arr, firstLen, arr + firstLen, secondLen, tempArray);
	memcpy(arr, tempArray, len * sizeof(int));

	delete[] tempArray;
}
void AlmostSortedSort(int* arr, size_t k, size_t size) {
	int* a = new int[k];
	int* b = new int[k];
	int* mergedArray = new int[2 * k];
	size_t z = 0;
	for (size_t i = 0; i < k; i++) {
		a[i] = arr[i];
	}
	MergeSort(a, k);

	if (size == k) {
		for (size_t i = 0; i < k; i++) arr[i] = a[i];
		return;
	}

	for (size_t i = k; i < size; i++) {
		b[i % k] = arr[i];
		if ((i + 1) % k == 0) {
			MergeSort(b, k);
			Merge(a, k, b, k, mergedArray);
			for (size_t j = 0; j < k; j++) {
				arr[z] = mergedArray[j];
				z++;
			}
			if (i + 1 == size) {
				for (size_t j = k; j < 2 * k; j++) {
					arr[z] = mergedArray[j];
					z++;
				}
			}
			for (size_t j = 0; j < k; j++) a[j] = mergedArray[j + k];
		}
	}

	if (size % k != 0) {
		MergeSort(b, size % k);
		Merge(a, k, b, size % k, mergedArray);
		for (size_t i = 0; i < k + (size % k); i++) {
			arr[z] = mergedArray[i];
			z++;
		}
	}

	delete[] a;
	delete[] b;
	delete[] mergedArray;
}
int main() {
	size_t length;
	size_t k;
	std::cin >> length >> k;

	int* array = new int[length];
	for (size_t i = 0; i < length; ++i) {
		std::cin >> array[i];
	}

	AlmostSortedSort(array, k, length);

	for (size_t i = 0; i < length; ++i) {
		std::cout << array[i] << ' ';
	}

	delete[] array;
	return 0;
}
