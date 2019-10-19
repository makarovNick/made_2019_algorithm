/*******************************************************/
// 1_2.Дек с динамическим буфером
//Ограничение времени	1 секунда
//Ограничение памяти	5Mb
//Ввод	стандартный ввод или input.txt
//Вывод	стандартный вывод или output.txt
//Реализовать дек с динамическим зацикленным буфером.
//
//Обрабатывать команды push * и pop * .
//
//Формат ввода
//В первой строке количество команд n.n ≤ 1000000.
//
//Каждая команда задаётся как 2 целых числа : a b.
//
// a = 1 - push front
// a = 2 - pop front
// a = 3 - push back
// a = 4 - pop back
//Если дана команда pop*, то число b - ожидаемое значение.Если команда pop *
//вызвана для пустой структуры данных, то ожидается “ - 1”.
/*******************************************************/
#include <iostream>

class deque {
 private:
  size_t size;
  size_t length;
  size_t tail;
  size_t head;
  int* base;
  void resize(const size_t);

 public:
  deque(deque& st) = delete;
  deque(deque&& st) = delete;
  deque operator=(deque& st) = delete;
  deque operator=(deque&& st) = delete;
  deque() {
    base = new int[8];
    size = 8;
    length = 0;
    tail = 0;
    head = 7;
  }

  ~deque() {
    delete[] base;
    base = nullptr;
    size = NULL;
    tail = NULL;
    head = NULL;
    length = NULL;
  }
  void push_front(int);
  void push_back(int);
  int pop_front();
  int pop_back();
  bool is_empty() const { return length == 0; }
  int front() const { return base[head]; }
  int back() const { return base[tail]; }
};
void deque::push_front(int value) {
  length++;
  base[head] = value;
  if (head == 0)
    head = size - 1;
  else
    head--;
  if (length + 1 >= size) resize(size * 2);
}

void deque::push_back(int value) {
 
  length++;
  base[tail] = value;

  if (tail == size - 1)
    tail = 0;
  else
    tail++;
 
  if (length + 1 >= size) resize(size * 2);
}

int deque::pop_front() {

 if (length == 0) return -1;
 
  if (head == (size - 1))
    head = 0;
  else
    head++;
  length--;
  int val = base[head];
 
  if (length * 4 < size && size > 8) resize(size / 4);
 
  return val;
}

int deque::pop_back() {
  if (length == 0) return -1;
 
  if (tail == 0)
    tail = size - 1;
  else
    tail--;
  int val = base[tail];
  length--;
 
  if (length * 5 < size && size > 8) resize(size / 4);
  return val;
}

void deque::resize(const size_t value) {
 
  int* new_base = new int[value];
  for (size_t i = 0; i < length; i++) {
    head++;
    if (head == size) head = 0;
    new_base[i] = base[head];
  }
  tail = length;
  size = value;
  head = value - 1;
 
  delete[] base;
  base = new_base;
}

int main() {
 
  deque deq;

  int n = 0;
  std::cin >> n;
  int val;
  for (int i = 0; i < n; ++i) {
    int command = 0;
    int value = 0;
    std::cin >> command >> value;

    switch (command) {
      case 1:
        deq.push_front(value);
        break;
      case 2:
        if (deq.pop_front() != value) {
          std::cout << "NO" << std::endl;
          return 0;
        }
        break;
      case 3:
        deq.push_back(value);
        break;
      case 4:
        if (deq.pop_back() != value) {
          std::cout << "NO" << std::endl;
          return 0;
        }
        break;
      default:
        return -1;
    }
  }
  std::cout << "YES" << std::endl;
  return 0;
}
