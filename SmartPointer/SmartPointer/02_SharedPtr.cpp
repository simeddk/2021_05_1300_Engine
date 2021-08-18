#include <iostream>
#include <Windows.h>
using namespace std;

class A : public enable_shared_from_this<A>
{
public:
	A()
	{
		data = new int[100];
		cout << "Resource Acquisition" << endl;
	}

	~A()
	{
		delete[] data;
		cout << "Resource Destroyed" << endl;
	}

public:
	FORCEINLINE void Func() { cout << "나는 메소드다!!" << endl; }

	FORCEINLINE int GetZeroIndex() { return data[0]; }
	FORCEINLINE void SetZeroIndex(int InValue)
	{
		data[0] = InValue;
		cout << "data[0] : " << data[0] << endl;
	}

	shared_ptr<A> GetSharedPtr() { return  shared_from_this(); }

	FORCEINLINE void SetOther(shared_ptr<A> InOther) { other = InOther; }

private:
	int* data;
	shared_ptr<A> other;
};

int main()
{
	auto obj1 = make_shared<A>();
	auto obj2 = make_shared<A>();

	obj1->SetOther(obj2);
	obj2->SetOther(obj1);

	cout << obj1.use_count() << endl;
	cout << obj2.use_count() << endl;

	system("pause");
	return 0;
}