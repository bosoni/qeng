// https://stackoverflow.com/questions/12606538/c-fastest-check-array-bounds/50572905
#pragma once

template<typename T>
class Array
{
	size_t size;
	T* array;

public:
	~Array() { Free(); }

	void Alloc(int size)
	{
#if DEBUG
		printf("  +alloc %d bytes\n", size);
#endif
		this->size = size;
		array = new T[size];
	}
	void Free()
	{
#if DEBUG
		printf("  -free %d bytes\n", size);
#endif
		size = 0;
		delete[] array;
	}

	T& operator[](int index)
	{
#if DEBUG
		if (index >= size)
		{
			printf("\n  ERROR! Out of range! %d/%d \n", index, size);
			throw "Out of range!";
		}
#endif
		return array[index];
	}
};

