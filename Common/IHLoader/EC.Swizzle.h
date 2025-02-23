#pragma once
#include <type_traits>
#include <SwizzleManagerClass.h>

template<typename T>
struct is_swizzlable : public std::is_pointer<T>::type {};

class BaseSwizzle
{
public:
	static BaseSwizzle Instance;

	BaseSwizzle()
	{}

	~BaseSwizzle()
	{}

	// 把需要改变地址的指针放在这里 , 在调用函数后 *指针 的值会变成 nullptr
	HRESULT RegisterForChange(void** 指针)
	{
		return SwizzleManagerClass::Instance().Swizzle(指针);
	}

	// 原版游戏中的所有对象都会保存它的 this 指针的值到输出流中 , 然后通过调用这个函数来记录保存的 this 指针的值和新的指针的值
	HRESULT RegisterChange(void* 旧的指针, void* 新的指针)
	{
		return SwizzleManagerClass::Instance().Here_I_Am((long)旧的指针, 新的指针);
	}

	template<typename T>
	void RegisterPointerForChange(T*& 指针)
	{
		auto 转换后的指针 = const_cast<std::remove_cv_t<T>**>(&指针);
		this->RegisterForChange(reinterpret_cast<void**>(转换后的指针));
	}
};

struct ECSwizzleHelper
{
	template <typename T>
	ECSwizzleHelper(T& 指针)
	{
		swizzle(指针, typename is_swizzlable<T>::type());
	}

private:
	template <typename TSwizzle>
	void swizzle(TSwizzle& 指针, std::true_type)
	{
		BaseSwizzle::Instance.RegisterPointerForChange(指针);
	}

	template <typename TSwizzle>
	void swizzle(TSwizzle& 指针, std::false_type)
	{
		// not swizzlable
	}
};