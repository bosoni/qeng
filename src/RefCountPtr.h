// https://www.codeproject.com/Articles/64111/Building-a-Quick-and-Handy-Reference-Counting-Clas

/**
* A reference counting-managed pointer for classes derived from RCBase which can
* be used as C pointer
*/
template < class T >
class RefCountPtr
{
public:
	//Construct using a C pointer
	//e.g. RCPtr< T > x = new T();
	RCPtr(T* ptr = NULL)
		: mPtr(ptr)
	{
		if(ptr != NULL) {ptr->grab();}
	}

	//Copy constructor
	RCPtr(const RCPtr &ptr)
		: mPtr(ptr.mPtr)
	{
		if(mPtr != NULL) {mPtr->grab();}
	}

	~RCPtr()
	{
		if(mPtr != NULL) {mPtr->release();}
	}

	//Assign a pointer
	//e.g. x = new T();
	RCPtr &operator=(T* ptr)
	{
		//The following grab and release operations have to be performed
		//in that order to handle the case where ptr == mPtr
		//(See comment below by David Garlisch)
		if(ptr != NULL) {ptr->grab();}
		if(mPtr != NULL) {mPtr->release();}
		mPtr = ptr;
		return (*this);
	}

	//Assign another RCPtr
	RCPtr &operator=(const RCPtr &ptr)
	{
		return (*this) = ptr.mPtr;
	}

	//Retrieve actual pointer
	T* get() const
	{
		return mPtr;
	}

	//Some overloaded operators to facilitate dealing with an RCPtr 
         //as a conventional C pointer.
	//Without these operators, one can still use the less transparent 
         //get() method to access the pointer.
	T* operator->() const {return mPtr;}		//x->member
	T &operator*() const {return *mPtr;}		//*x, (*x).member
	operator T*() const {return mPtr;}		//T* y = x;
	operator bool() const {return mPtr != NULL;}	//if(x) {/*x is not NULL*/}
	bool operator==(const RCPtr &ptr) {return mPtr == ptr.mPtr;}
	bool operator==(const T *ptr) {return mPtr == ptr;}

private:
	T *mPtr;	//Actual pointer
};

