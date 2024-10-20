#pragma once
#include <functional>
#include <utility>
#include <YRMathVector.h>
#include <YRPP.h>

template<typename T>
class SIConstVector
{
protected:
	const T* Begin;
	const T* End;
	const T* CapacityEnd;
public:
	using iterator = const T*;
	using value_type = T;
	using reference = const T&;
	using const_iterator = const T*;
	using const_reference = const T&;

	inline const T* begin() const { return Begin; }
	inline const T* end() const { return End; }
	inline const T& front() const { return *Begin;}
	inline const T& back() const { return *(End - 1); }
	inline const T& at(size_t sz) const { return Begin[sz]; }
	inline const T& operator[] (size_t sz) const  { return Begin[sz]; }
	inline const size_t size() const { return ((size_t)(End - Begin)) / sizeof(T); }
	inline const size_t capacity() const { return ((size_t)(CapacityEnd - Begin)) / sizeof(T); }
	inline const bool Empty() const { return Begin == End; }

	SIConstVector() = delete;
	SIConstVector(const SIConstVector&) = delete;
	SIConstVector(SIConstVector&&) = delete;
	SIConstVector(noinit_t) {};
};

// ��֧�� unique_ptr , ����ֱ��Ƕ���γɶ����б�
template <typename T>
class SIDataList final : public SIConstVector<T>
{
public:
	bool Contains(const T& ֵ) const
	{
		return std::find(this->begin(), this->end(), ֵ) != this->end();
	}

	int IndexOf(const T& ֵ) const
	{
		typename SIConstVector<T>::const_iterator ������ = std::find(this->begin(), this->end(), ֵ);
		if (������ != this->end())
		{
			return ((int)(������ - this->begin()));
		}
		return -1;
	}

	T GetItem(size_t ����)
	{
		//if (this->InvalidIndex(����))
		//{
		//	SIDebug::Log("�� SIDataList ȡ������ʱ , ����ָ���˷�Χ�� : [%hs]", typeid(T).name());
		//}
		return this->at(����);
	}

	T TryGetItem(size_t ����, T Ĭ��ֵ)
	{
		if (this->InvalidIndex(����))
		{
			return Ĭ��ֵ;
		}
		return this->at(����);
	}

	bool ValidIndex(int ����)
	{
		return ���� > -1 && ���� < static_cast<int>(this->size());
	}

	bool InvalidIndex(int ����)
	{
		return ���� < 0 || ���� >= static_cast<int>(this->size());
	}

	SIDataList() = delete;
	SIDataList(const SIDataList&) = delete;
	SIDataList(SIDataList&&) = delete;
	SIDataList(noinit_t _) :  SIConstVector<T>(_) {}
};

// ��֧�� unique_ptr , ����ֱ��Ƕ���γɶ��ؼ���
template <typename TKey, typename TValue>
class SIDataMap final
{
public:
	SIConstVector<std::pair<TKey, TValue>> �����б�;

	// ���ܺ���
	bool Contains(const TKey& ��) const
	{
		return this->get_iterator(��) != this->�����б�.end();
	}

	TValue TryGet(const TKey& ��, TValue Ĭ��ֵ) const
	{
		typename SIConstVector<std::pair<TKey, TValue>>::const_iterator ������ = this->get_iterator(��);
		if (������ != this->�����б�.end())
		{
			return ������->second;
		}
		return Ĭ��ֵ;
	}

	size_t Count()
	{
		return this->�����б�.size();
	}

	bool Empty() const
	{
		return this->�����б�.empty();
	}

	SIDataMap() = delete;
	SIDataMap(const SIDataMap&) = delete;
	SIDataMap(SIDataMap&&) = delete;
	SIDataMap(noinit_t _) :�����б�(_) {};

private:
	typename SIConstVector<std::pair<TKey, TValue>>::const_iterator get_iterator(const TKey& key) const
	{
		return std::find_if(this->�����б�.begin(), this->�����б�.end(), [&](const SIConstVector<std::pair<TKey, TValue>>::value_type& item)
			{
				return item.first == key;
			});
	}
};

template<typename T>
class SIVector3D : public Vector3D<T>
{
public:
	size_t SIValueCount;
};

template <typename T>
class SIEnumerable
{
public:
	//������Static��ͷ����װ��SIClassManager�Ľӿ�������

	// ��������
	FixedString<32> Name;
	// ��ʼ��
	SIEnumerable(const char* ����)
	{
		this->Name = ����;
	}
	// ���ݴ�����
	virtual ~SIEnumerable() = default;
	virtual void InitializeConstants() {}
	virtual void LoadFromINI(CCINIClass* pINI) {}
	virtual void LoadFromStream(SIStreamReader& Stm) = 0;
	virtual void SaveToStream(SIStreamWriter& Stm) = 0;
	// ���ܺ���
	const char* GetName()
	{
		return (const char*)(this->Name);
	}
};