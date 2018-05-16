
#pragma comment (lib, "Util_d.lib")

#include <Collections\BufferQueue.h>

struct Test
{
	int x;
};
int main()
{
	Util::Collections::BufferQueue<Test> q;
	Test t; 
	t.x = 10;
	auto tt = new Test[10];
	for (int i = 0; i < 10; i++)
		tt[i].x = i;
	try
	{
		q.Append(t).Append(tt, 10);
	}
	catch(...)
	{

	}
	
	auto count = q.Count();
	return 0;
}