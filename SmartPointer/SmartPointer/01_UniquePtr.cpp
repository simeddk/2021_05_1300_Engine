#include <iostream>
#include <Windows.h>
#include <vector>
using namespace std;

class A
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

private:
	int* data;
};


void Create()
{
	unique_ptr<A> obj1(new A());
	
	unique_ptr<A> obj2 = move(obj1);
	obj2->Func();

	cout << obj1.get() << endl;
	cout << obj2.get() << endl;
}

void SetZeroIndex(A* inA, int inValue)
{
	inA->SetZeroIndex(inValue);
}

void GetZeroIndex(A* inA)
{
	inA->GetZeroIndex();
}

int main()
{
	auto obj = make_unique<A>();
	obj->SetZeroIndex(300);
	obj->GetZeroIndex();

	vector<unique_ptr<A>> vec;
	vec.emplace_back(new A());
	vec.emplace_back(new A());
	vec.emplace_back(new A());
	vec.back()->Func();

	system("pause");
	return 0;
}