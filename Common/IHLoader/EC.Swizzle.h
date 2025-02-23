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

	// ����Ҫ�ı��ַ��ָ��������� , �ڵ��ú����� *ָ�� ��ֵ���� nullptr
	HRESULT RegisterForChange(void** ָ��)
	{
		return SwizzleManagerClass::Instance().Swizzle(ָ��);
	}

	// ԭ����Ϸ�е����ж��󶼻ᱣ������ this ָ���ֵ��������� , Ȼ��ͨ�����������������¼����� this ָ���ֵ���µ�ָ���ֵ
	HRESULT RegisterChange(void* �ɵ�ָ��, void* �µ�ָ��)
	{
		return SwizzleManagerClass::Instance().Here_I_Am((long)�ɵ�ָ��, �µ�ָ��);
	}

	template<typename T>
	void RegisterPointerForChange(T*& ָ��)
	{
		auto ת�����ָ�� = const_cast<std::remove_cv_t<T>**>(&ָ��);
		this->RegisterForChange(reinterpret_cast<void**>(ת�����ָ��));
	}
};

struct ECSwizzleHelper
{
	template <typename T>
	ECSwizzleHelper(T& ָ��)
	{
		swizzle(ָ��, typename is_swizzlable<T>::type());
	}

private:
	template <typename TSwizzle>
	void swizzle(TSwizzle& ָ��, std::true_type)
	{
		BaseSwizzle::Instance.RegisterPointerForChange(ָ��);
	}

	template <typename TSwizzle>
	void swizzle(TSwizzle& ָ��, std::false_type)
	{
		// not swizzlable
	}
};