#define DEBUG

#include <iostream>
#include <unordered_map>
#include <queue>
#include <algorithm>

const short N = 4;
const short SOLVED[N*N] = {1,  2,  3,  4
						, 5,  6,  7,  8
						, 9,  10, 11, 12
						, 13, 14, 15, 0};

class State 
{
public:
    State(const short f[N*N]) 
	{
        for(size_t i = 0; i < N*N; i++ )
		{
			field[i] = f[i];
            if( field[i] == 0 )
			{
                zero_pos = i;
			}
		}
    }
    State(const State& state) 
		: zero_pos(state.zero_pos)
	{
		for(size_t i = 0; i < N*N; i++ )
		{
			field[i] = state.field[i];
		}
    }

    bool isSolved() const 
	{
		for (size_t i = 0; i < N*N; i++)
		{
			if (field[i] != SOLVED[i])
			{
				return false;
			}
		}

		return true;
    }

    State moveLeft() const 
	{
        State new_state(*this);
        std::swap(new_state.field[zero_pos], new_state.field[zero_pos + 1]);
        new_state.zero_pos++;
        return new_state;
    }
    State moveRight() const 
	{
        State new_state(*this);
        std::swap(new_state.field[zero_pos], new_state.field[zero_pos - 1]);
        new_state.zero_pos--;
        return new_state;
    }
    State moveUp() const 
	{
        State new_state(*this);
        std::swap(new_state.field[zero_pos], new_state.field[zero_pos + 4]);
        new_state.zero_pos += 4;
        return new_state;
    }
    State moveDown() const 
	{
        State new_state(*this);
        std::swap(new_state.field[zero_pos], new_state.field[zero_pos - 4]);
        new_state.zero_pos -= 4;
        return new_state;
    }

    bool operator==(const State& state) const 
	{
        for (size_t i = 0; i < N*N; i++)
		{
			if (field[i] != state.field[i])
			{
				return false;
			}
		}

		return true;
    }

    friend struct Hasher;

	short getZeroPos() const
	{
		return zero_pos;
	}
	short getHeuristic() const
	{
		return manhattan();
	}

	auto getField() const
	{
		return field;
	}
private:

	short linear() const
    {
		auto conflicts = 0;

    {
        bool in_col[N*N * N*N] = { };
        bool in_row[N*N * N*N] = { }; 

        for (auto y = 0; y != N*N; ++y)
        {
            for (auto x = 0; x != N*N; ++x)
            {
                auto i = y * N*N + x;

                auto bx = field[i] % N*N;
                auto by = field[i] / N*N;

                in_col[i] = (bx == x);
                in_row[i] = (by == y);
            }
        }

        for (auto y = 0; y != N*N; ++y)
        {
            for (auto x = 0; x != N*N; ++x)
            {
                auto i = y * N*N + x;

                if (field[i] == 0) // ignore the empty space
                    continue;

                if (in_col[i])
                {
                    for (auto r = y; r != N*N; ++r) // check down for conflicts
                    {
                        auto j = r * N*N + x;

                        if (field[j] == 0) // ignore the empty space
                            continue;

                        if (in_col[j] && field[j] < field[i])
                            ++conflicts;
                    }
                }

                if (in_row[i])
                {
                    for (auto c = x; c != N*N; ++c) // check right for conflicts
                    {
                        auto j = y * N*N + c;

                        if (field[j] == 0) // ignore the empty space
                            continue;

                        if (in_row[j] && field[j] < field[i])
                            ++conflicts;
                    }
                }
            }
        }
    }

    return 2 * conflicts;
    }

	short hamming() const
	{
		short count = 0;
		for (size_t i = 0; i < N*N; i++)
		{
			if (field[i] != SOLVED[i])
			{
				count++;
			}
		}
		return count;
	}

    short manhattan() const 
	{
		int sum = 0;
        int distance = 0;
        for (int i = 0; i < N*N; i++) 
		{
            if (field[i] != 0) 
			{
                distance = abs(field[i] - i - 1);
            } 
			else 
			{
                distance = N*N - i;
            }
            sum += distance / N;
            distance %= N;
            if (distance == N-1	) 
			{
                sum += 2;
            } 
			else 
			{
                sum += distance;
            }
        }

        return sum;
    }

    short field[N*N];
	
    short zero_pos;
};

struct Hasher 
{
    size_t operator()(const State& state) const
    {
        size_t hash = 0;
        for (size_t i = 0; i < N*N; i++)
		{
            hash = (hash * 31 + i);
        }

        return hash;
    }
};

struct comparator
{
	bool operator()(const State &a, const State &b) 
	{
        return a.getHeuristic() >= b.getHeuristic();
    }
};

std::string getSolution(const short field[N*N]) 
{
    std::priority_queue<State, std::vector<State>, comparator> queue;
    std::unordered_map<State, std::string, Hasher> visited;
	std::unordered_map<std::string, int> path_lenght;

    State init_state(field);
    visited[init_state];

    queue.push(init_state);

    State solved(SOLVED);

    while (!(queue.top() == solved)) 
	{
        State current_state = queue.top();
        queue.pop();

		#ifdef DEBUG
		auto f = current_state.getField();
		std::cout<<std::endl;
		std::cout<<"Heuristic : "  << current_state.getHeuristic()
			   << " Queue deepth : " << queue.size()
			   << " Zero pos : " << current_state.getZeroPos()/N + 1<< " " <<current_state.getZeroPos()%N + 1 << std::endl;
		for (size_t i = 0; i < N*N; i++)
		{
			if(i%4==0)
				std::cout<<std::endl;
			std::cout<<f[i]<<" ";
		}
		#endif
		
		if (queue.size() > 160) 
		{
            std::priority_queue<State, std::vector<State>, comparator> new_queue;
            for (int i = 0; i < 80; ++i) 
			{
                new_queue.emplace(queue.top());
                queue.pop();
            }

            queue = new_queue;
        }

        if(current_state.getZeroPos() % N < N - 1) 
		{
            State new_state = current_state.moveLeft();
            if(visited.find(new_state) == visited.end()) 
			{
                queue.emplace(new_state);
                visited[new_state] = visited[current_state] + "L";
            }
        }
        if(current_state.getZeroPos() % N > 0) 
		{
            State new_state = current_state.moveRight();
            if(visited.find(new_state) == visited.end()) 
			{
                queue.emplace(new_state);
                visited[new_state] = visited[current_state] + "R";
            }
        }
        if(current_state.getZeroPos() / N < N-1) 
		{
            State new_state = current_state.moveUp();
            if( visited.find(new_state) == visited.end() ) 
			{
                queue.emplace(new_state);
                visited[new_state] = visited[current_state] + "U";
            }
        }
        if(current_state.getZeroPos() / N > 0) 
		{
            State new_state = current_state.moveDown();
            if( visited.find(new_state) == visited.end() ) 
			{
                queue.emplace(new_state);
                visited[new_state] = visited[current_state] + "D";
            }
        }
    }

    return visited[solved];
}


int getInvCount(short arr[])
{
	int inv_count = 0;
	for (int i = 0; i < N * N - 1; i++)
	{
		for (int j = i + 1; j < N * N; j++)
		{
			if (arr[j] != 0
			 && arr[i] != 0
			 && arr[i] > arr[j])
			 {
				inv_count++;
			 }
		}
	}
	return inv_count;
}

int findXPosition(short puzzle[N*N])
{
	for (size_t i = 0; i < N*N; i++)
	{
		if(puzzle[i] == 0)
		{
			return N - (i / N);
		}
	}
}

bool isSolvable(short puzzle[N*N])
{
	int invCount = getInvCount(puzzle);

	if (N & 1)
	{
		return !(invCount & 1);
	}
	else     
	{
		int pos = findXPosition(puzzle);
		if (pos & 1)
			return !(invCount & 1);
		else
			return invCount & 1;
	}
}

int main() 
{
    short field[N*N];

    for (size_t i = 0; i < N*N; i++) 
	{
        std::cin >> field[i];
    }

    if (!isSolvable(field)) 
	{
        std::cout << -1 << std::endl;
        return 0;
    }

    std::string solution = getSolution(field);
    std::cout << solution.size()<< std::endl;
	std::cout << solution << std::endl;

    return 0;
}