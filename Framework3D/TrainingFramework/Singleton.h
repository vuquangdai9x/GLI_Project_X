#pragma once
#include<stddef.h>
template<typename T>
class Singleton
{
public:
	Singleton() ;
	~Singleton();
	static T* GetInstance();
	static void DestroyInstance();
	static void CreateInstance();	

protected:
	Singleton(const Singleton &) {};
	Singleton& operator=(const Singleton &) {};
	static  T * ms_pInstance;
};
template<typename T>
typename T* Singleton<T>::ms_pInstance = 0;


template<typename T>
inline Singleton<T>::Singleton()
{
	ms_pInstance = static_cast <T*> (this);
}

template<typename T>
inline Singleton<T>::~Singleton()
{
}
template<typename T>
void Singleton<T>::CreateInstance()
{
	if (ms_pInstance == NULL)
		ms_pInstance = new T();
}
template<typename T>
T* Singleton<T>::GetInstance()
{
	return ms_pInstance;
}
template<typename T>
void Singleton<T>::DestroyInstance()
{
	if (ms_pInstance)
	{
		delete ms_pInstance;
		ms_pInstance = NULL;
	}
}

