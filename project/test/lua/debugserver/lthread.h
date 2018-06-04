#ifndef __LTHREAD_H__
#define __LTHREAD_H__
#include <thread>
#include <functional>
#include <map>

namespace lth 
{

enum class lthstate {NONE, RUN, STOP};
/*
template<typename T, typename... Ts>
std::unique_ptr<T> make_unique(Ts&&... params)
{
	return std::unique_ptr<T>(new T(std::forward<Ts>(params)...));
}
*/
template <typename T>
class lthread
{

public: 
	lthread(): id(0)
	{

	}

	~lthread()
	{

	}

public:
	bool CreateThread(T foo)
	{
		lthstate state = lthstate::NONE;
		auto f = [&state, &foo]()->int {
			do
			{
				if (state == lthstate::STOP)
					break;

				if (state == lthstate::NONE)
					continue;

				foo();
			} while (true);
			return 1;
		};
		return true;
	}

private:
	int id;
	std::map<int, std::function<void()> > m_mapfn;
};

};

#endif