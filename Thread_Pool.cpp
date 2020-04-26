#include <iostream>
#include<future>
#include<vector>
#include<mutex>

using namespace std;

class Thread_Pool
{
public:
	Thread_Pool() 
	{
		this->max_thread_number = 0;
	}

	~Thread_Pool() 
	{
		stop();
	}

	void set_thread_number(const int& number=0);//+

	void run();//+

	template<class T, class R = result_of_t<T & ()>>
	future<R> add(T&& t)
	{
		packaged_task<R()> PT(forward<T>(t));
		auto result = PT.get_future();
		{
			unique_lock<mutex> LOCK(mtx);
			pool.emplace_back(move(PT));
		}
		_cvar.notify_one();
		return result;
	}

	void cancel();

	void stop();
	
	//vector<future<void>>& get_results();
private:

	mutex mtx;

	condition_variable _cvar;

	vector<future<void>>results;

	vector<packaged_task<void()>>pool;

	unsigned int max_thread_number;

	void async_task();
	
};

void Thread_Pool::set_thread_number(const int& number)
{
	int a = thread::hardware_concurrency();
	if (number >= a||number==0)
	{
		cout << "WARNING! ALL " << a << " CPU`S WILL BE USED! DO YOU WANT TO CONTINUE?(Y/N)\n";
		if (cin.get() == 'Y')
			this->max_thread_number = a;
	}
	else
		this->max_thread_number = number;
	cout << "SET COMPLETED\n";
}

void Thread_Pool::run()
{
		if (this->max_thread_number == 0)
		{
			cout << "THREAD NUMBER IS UNSET!\n";
			return;
		}
		for (int i = 0; i < this->max_thread_number; i++)
		{
			this->results.push_back(async(launch::async, [this] { async_task(); }));
		}
}

void Thread_Pool::cancel()
{
	{
		unique_lock<mutex> LOCK(mtx);
		pool.clear();
	}
	stop();
	this->results.clear();
}

void Thread_Pool::async_task()
{
	packaged_task<void()>task;
	while (true)
	{
		packaged_task<void()> function;
		{
			unique_lock<mutex> LOCK(mtx);
			if (pool.empty())
			{
				_cvar.wait(LOCK, [&] {return !pool.empty(); });
			}
			function = move(pool.front());
			pool.erase(pool.begin());
		}
		if (!function.valid())
			break;
		else
			function();
	}
}

void Thread_Pool::stop()
{
	{
		unique_lock<mutex> LOCK(mtx);
		for (auto&& unused : this->results)
		{
			pool.push_back({});
		}
	}
	_cvar.notify_all();
}

/*vector<future<void>>& Thread_Pool::get_results()
{
	return ref(this->results);
}*/

int f1(int a)
{
	cout << "F1\n";
	return a;
}
int f2(int a)
{
	cout << " F2\n";
	return a;
}

int main()
{
	Thread_Pool tp;
	tp.add([]() {return f1(10); });
	return 0;
}