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

// 不支持 unique_ptr , 不能直接嵌套形成二重列表
template <typename T>
class SIDataList final : public SIConstVector<T>
{
public:
	bool Contains(const T& 值) const
	{
		return std::find(this->begin(), this->end(), 值) != this->end();
	}

	int IndexOf(const T& 值) const
	{
		typename SIConstVector<T>::const_iterator 迭代器 = std::find(this->begin(), this->end(), 值);
		if (迭代器 != this->end())
		{
			return ((int)(迭代器 - this->begin()));
		}
		return -1;
	}

	T GetItem(size_t 索引)
	{
		//if (this->InvalidIndex(索引))
		//{
		//	SIDebug::Log("向 SIDataList 取出数据时 , 索引指向了范围外 : [%hs]", typeid(T).name());
		//}
		return this->at(索引);
	}

	T TryGetItem(size_t 索引, T 默认值)
	{
		if (this->InvalidIndex(索引))
		{
			return 默认值;
		}
		return this->at(索引);
	}

	bool ValidIndex(int 索引)
	{
		return 索引 > -1 && 索引 < static_cast<int>(this->size());
	}

	bool InvalidIndex(int 索引)
	{
		return 索引 < 0 || 索引 >= static_cast<int>(this->size());
	}

	SIDataList() = delete;
	SIDataList(const SIDataList&) = delete;
	SIDataList(SIDataList&&) = delete;
	SIDataList(noinit_t _) :  SIConstVector<T>(_) {}
};

// 不支持 unique_ptr , 不能直接嵌套形成二重集合
template <typename TKey, typename TValue>
class SIDataMap final
{
public:
	SIConstVector<std::pair<TKey, TValue>> 数据列表;

	// 功能函数
	bool Contains(const TKey& 键) const
	{
		return this->get_iterator(键) != this->数据列表.end();
	}

	TValue TryGet(const TKey& 键, TValue 默认值) const
	{
		typename SIConstVector<std::pair<TKey, TValue>>::const_iterator 迭代器 = this->get_iterator(键);
		if (迭代器 != this->数据列表.end())
		{
			return 迭代器->second;
		}
		return 默认值;
	}

	size_t Count()
	{
		return this->数据列表.size();
	}

	bool Empty() const
	{
		return this->数据列表.empty();
	}

	SIDataMap() = delete;
	SIDataMap(const SIDataMap&) = delete;
	SIDataMap(SIDataMap&&) = delete;
	SIDataMap(noinit_t _) :数据列表(_) {};

private:
	typename SIConstVector<std::pair<TKey, TValue>>::const_iterator get_iterator(const TKey& key) const
	{
		return std::find_if(this->数据列表.begin(), this->数据列表.end(), [&](const SIConstVector<std::pair<TKey, TValue>>::value_type& item)
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
	//真正的Static大头被封装到SIClassManager的接口里面了

	// 基础属性
	FixedString<32> Name;
	// 初始化
	SIEnumerable(const char* 名称)
	{
		this->Name = 名称;
	}
	// 数据处理函数
	virtual ~SIEnumerable() = default;
	virtual void InitializeConstants() {}
	virtual void LoadFromINI(CCINIClass* pINI) {}
	virtual void LoadFromStream(SIStreamReader& Stm) = 0;
	virtual void SaveToStream(SIStreamWriter& Stm) = 0;
	// 功能函数
	const char* GetName()
	{
		return (const char*)(this->Name);
	}
};