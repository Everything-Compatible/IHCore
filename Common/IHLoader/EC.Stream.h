#pragma once
#include <SwizzleManagerClass.h>
#include "EC.Swizzle.h"
#include "IH.Loader.h"
#include <bitset>
#include <set>
#include <map>

class ECStreamWriter;
class ECStreamReader;
namespace ECSavegameHelper
{
	template <typename T>
	bool ReadStream(ECStreamReader& 流, T& 值, bool 注册变化);
	template <typename T>
	bool WriteStream(ECStreamWriter& 流, const T& 值);
};

class ECStreamByte
{
public:
	using data_t = unsigned char;

protected:
	std::vector<data_t> Data;
	size_t CurrentOffset;

public:
	ECStreamByte(size_t Reserve = 0x1000) : Data(), CurrentOffset(0)
	{
		this->Data.reserve(Reserve);
	}

	~ECStreamByte() = default;

	size_t Size() const
	{
		return this->Data.size();
	}

	size_t Offset() const
	{
		return this->CurrentOffset;
	}

	/**
	* reads {Length} bytes from {pStm} into its storage
	*/
	bool ReadFromStream(IStream* pStm, const size_t Length)
	{
		auto size = this->Data.size();
		this->Data.resize(size + Length);
		auto pv = reinterpret_cast<void*>(this->Data.data());
		ULONG out = 0;
		auto success = pStm->Read(pv, Length, &out);
		bool result(SUCCEEDED(success) && out == Length);
		if (!result)
			this->Data.resize(size);
		return result;
	}

	/**
	* writes all internal storage to {pStm}
	*/
	bool WriteToStream(IStream* pStm) const
	{
		const size_t Length(this->Data.size());
		auto pcv = reinterpret_cast<const void*>(this->Data.data());
		ULONG out = 0;
		auto success = pStm->Write(pcv, Length, &out);
		return SUCCEEDED(success) && out == Length;
	}

	/**
	* reads the next block of bytes from {pStm} into its storage,
	* the block size is prepended to the block
	*/
	size_t ReadBlockFromStream(IStream* pStm)
	{
		ULONG out = 0;
		size_t Length = 0;
		if (SUCCEEDED(pStm->Read(&Length, sizeof(Length), &out)))
		{
			if (this->ReadFromStream(pStm, Length))
				return Length;
		}
		return 0;
	}

	/**
	* writes all internal storage to {pStm}, prefixed with its length
	*/
	bool WriteBlockToStream(IStream* pStm) const
	{
		ULONG out = 0;
		const size_t Length = this->Data.size();
		if (SUCCEEDED(pStm->Write(&Length, sizeof(Length), &out)))
			return this->WriteToStream(pStm);
		return false;
	}

	// primitive save/load - should not be specialized

	/**
	* if it has {Size} bytes left, assigns the first {Size} unread bytes to {Value}
	* moves the internal position forward
	*/
	bool Read(data_t* Value, size_t Size)
	{
		bool ret = false;
		if (this->Data.size() >= this->CurrentOffset + Size)
		{
			auto Position = &this->Data[this->CurrentOffset];
			std::memcpy(Value, Position, Size);
			ret = true;
		}
		this->CurrentOffset += Size;
		return ret;
	}

	/**
	* ensures there are at least {Size} bytes left in the internal storage, and assigns {Value} casted to byte to that buffer
	* moves the internal position forward
	*/
	void Write(const data_t* Value, size_t Size)
	{
		this->Data.insert(this->Data.end(), Value, Value + Size);
	}

	/**
	* attempts to read the data from internal storage into {Value}
	*/
	template<typename T>
	bool Load(T& Value)
	{
		// get address regardless of overloaded & operator
		auto Bytes = &reinterpret_cast<data_t&>(Value);
		return this->Read(Bytes, sizeof(T));
	}

	/**
	* writes the data from {Value} into internal storage
	*/
	template<typename T>
	void Save(const T& Value)
	{
		// get address regardless of overloaded & operator
		auto Bytes = &reinterpret_cast<const data_t&>(Value);
		this->Write(Bytes, sizeof(T));
	};
};

class ECStreamBase
{
public:
	explicit ECStreamBase(ECStreamByte& Stream) : stream(&Stream), success(true)
	{}

	ECStreamBase(const ECStreamBase&) = delete;

	ECStreamBase& operator = (const ECStreamBase&) = delete;

	bool Success() const
	{
		return this->success;
	}

protected:

	ECStreamByte* stream;
	bool success;
};

void RaiseEmitLoadWarning();

class ECStreamWriter : public ECStreamBase
{
public:
	explicit ECStreamWriter(ECStreamByte& Stream) : ECStreamBase(Stream)
	{}

	ECStreamWriter(const ECStreamWriter&) = delete;

	ECStreamWriter& operator = (const ECStreamWriter&) = delete;

	template <typename T>
	ECStreamWriter& Process(T& value, bool RegisterForChange = true)
	{
		this->success &= ECSavegameHelper::WriteStream(*this, value);
		return *this;
	}

	template <typename T>
	void Save(const T& buffer)
	{
		this->stream->Save(buffer);
	}

	void Write(const ECStreamByte::data_t* Value, size_t Size)
	{
		this->stream->Write(Value, Size);
	}
	bool Expect(unsigned int value)
	{
		this->Save(value);
		return true;
	}
	bool RegisterChange(const void* oldPtr)
	{
		this->Save(oldPtr);
		return true;
	}
};

class ECStreamReader : public ECStreamBase
{
public:
	explicit ECStreamReader(ECStreamByte& Stream) : ECStreamBase(Stream)
	{}

	ECStreamReader(const ECStreamReader&) = delete;

	ECStreamReader& operator = (const ECStreamReader&) = delete;

	template <typename T>
	ECStreamReader& Process(T& value, bool RegisterForChange = true)
	{
		this->success &= ECSavegameHelper::ReadStream(*this, value, RegisterForChange);
		return *this;
	}

	template <typename T>
	bool Load(T& buffer)
	{
		if (!this->stream->Load(buffer))
		{
			RaiseEmitLoadWarning();
			this->success = false;
			return false;
		}
		return true;
	}

	bool ExpectEndOfBlock() const;
	bool Read(ECStreamByte::data_t* Value, size_t Size);
	bool Expect(unsigned int value);
	bool RegisterChange(void* pNew);

private:
	void EmitExpectEndOfBlockWarning() const;
	void EmitLoadWarning(size_t size) const;
	void EmitExpectWarning(unsigned int found, unsigned int expect) const;
	void EmitSwizzleWarning(long id, void* pointer) const;
};




namespace ECSavegameHelper
{
	template <typename T>
	struct StreamObject;

	namespace detail
	{
		struct Selector
		{
			template <typename T>
			static bool ReadFromStream(ECStreamReader& 流, T& 值, bool 注册变化)
			{
				return read_from_stream(流, 值, 注册变化, 0, 0);
			}

			template <typename T>
			static bool WriteToStream(ECStreamWriter& 流, const T& 值)
			{
				return write_to_stream(流, 值, 0, 0);
			}

		private:
			// support for upper-case Load and lowercase load member functions.
			// this is more complex than needed, but allows for more consistency
			// in function naming.
			struct Dummy
			{
				Dummy(int a) {};
			};

			template <typename T>
			static auto read_from_stream(ECStreamReader& 流, T& 值, bool 注册变化, int, int) -> decltype(值.Load(流, 注册变化))
			{
				return 值.Load(流, 注册变化);
			}

			template <typename T>
			static auto read_from_stream(ECStreamReader& 流, T& 值, bool 注册变化, Dummy, int) -> decltype(值.load(流, 注册变化))
			{
				return 值.load(流, 注册变化);
			}

			template <typename T>
			static bool read_from_stream(ECStreamReader& 流, T& 值, bool 注册变化, Dummy, Dummy)
			{
				StreamObject<T> item;
				return item.ReadFromStream(流, 值, 注册变化);
			}

			template <typename T>
			static auto write_to_stream(ECStreamWriter& 流, const T& 值, int, int) -> decltype(值.Save(流))
			{
				return 值.Save(流);
			}

			template <typename T>
			static auto write_to_stream(ECStreamWriter& 流, const T& 值, Dummy, int) -> decltype(值.save(流))
			{
				return 值.save(流);
			}

			template <typename T>
			static bool write_to_stream(ECStreamWriter& 流, const T& 值, Dummy, Dummy)
			{
				StreamObject<T> item;
				return item.WriteToStream(流, 值);
			}
		};
	}

	struct ECHelper_SGDummy
	{
		ECHelper_SGDummy(int a) {};
	};

	template <typename T>
	struct LoadFactory
	{
		std::unique_ptr<T> operator() (ECStreamReader& 流, bool 注册变化) const
		{
			return load(流, 注册变化, 0);
		}

	private:
		static auto load(ECStreamReader& 流, bool 注册变化, int) -> decltype(T::LoadOne(流, 注册变化))
		{
			return T::LoadOne(流, 注册变化);
		}

		static auto load(ECStreamReader& 流, bool 注册变化, ECHelper_SGDummy)
		{
			return std::make_unique<T>();
		}
	};

	template <typename T>
	struct SaveFactory
	{
		bool operator() (ECStreamWriter& 流, const T* 值)
		{
			return save(流, 值, 0);
		}

	private:
		static auto save(ECStreamWriter& 流, const T* 值, int) -> decltype(T::SaveOne(流, 值))
		{
			return T::SaveOne(流, 值);
		}

		static auto save(ECStreamWriter& 流, const T* 值, ECHelper_SGDummy)
		{
			return true;
		}
	};

	template <typename T>
	bool ReadStream(ECStreamReader& 流, T& 值, bool 注册变化 = true)
	{
		return detail::Selector::ReadFromStream(流, 值, 注册变化);
	}

	template <typename T>
	bool WriteStream(ECStreamWriter& 流, const T& 值)
	{
		return detail::Selector::WriteToStream(流, 值);
	}

	template <typename T>
	T* RestoreObject(ECStreamReader& 流, bool 注册变化 = true)
	{
		T* 旧指针 = nullptr;
		if (!流.Load(旧指针))
		{
			return nullptr;
		}
		if (旧指针)
		{
			std::unique_ptr<T> 新指针 = LoadFactory<T>()(流, 注册变化);
			if (ECSavegameHelper::ReadStream(流, *新指针, 注册变化))
			{
				BaseSwizzle::Instance.RegisterChange(旧指针, 新指针.get());
				return 新指针.release();
			}
		}
		return nullptr;
	}

	template <typename T>
	bool PersistObject(ECStreamWriter& 流, const T* 值)
	{
		if (!ECSavegameHelper::WriteStream(流, 值))
		{
			return false;
		}
		if (值)
		{
			return SaveFactory<T>()(流, 值) && ECSavegameHelper::WriteStream(流, *值);
		}
		return true;
	}


	template <typename T>
	struct StreamObject
	{
		bool ReadFromStream(ECStreamReader& 流, T& 值, bool 注册变化) const
		{
			bool 载入结果 = 流.Load(值);
			if (注册变化)
			{
				ECSwizzleHelper swizzle(值);
			}
			return 载入结果;
		}

		bool WriteToStream(ECStreamWriter& 流, const T& 值) const
		{
			流.Save(值);
			return true;
		}
	};
};


template <typename T>
struct ECSavegameHelper::StreamObject<VectorClass<T>>
{
	bool ReadFromStream(ECStreamReader& 流, VectorClass<T>& 值, bool 注册变化) const
	{
		值.Clear();
		int Capacity = 0;
		if (!流.Load(Capacity))
		{
			return false;
		}
		值.Reserve(Capacity);
		for (auto ix = 0; ix < Capacity; ++ix)
		{
			if (!ECSavegameHelper::ReadStream(流, 值.Items[ix], 注册变化))
			{
				return false;
			}
		}
		return true;
	}

	bool WriteToStream(ECStreamWriter& 流, const VectorClass<T>& 值) const
	{
		流.Save(值.Capacity);
		for (auto ix = 0; ix < 值.Capacity; ++ix)
		{
			if (!ECSavegameHelper::WriteStream(流, 值.Items[ix]))
			{
				return false;
			}
		}
		return true;
	}
};

template <typename T>
struct ECSavegameHelper::StreamObject<DynamicVectorClass<T>>
{
	bool ReadFromStream(ECStreamReader& 流, DynamicVectorClass<T>& 值, bool 注册变化) const
	{
		值.Clear();
		int Capacity = 0;
		if (!流.Load(Capacity))
		{
			return false;
		}
		值.Reserve(Capacity);
		if (!流.Load(值.Count) || !流.Load(值.CapacityIncrement))
		{
			return false;
		}
		for (auto ix = 0; ix < 值.Count; ++ix)
		{
			if (!ECSavegameHelper::ReadStream(流, 值.Items[ix], 注册变化))
			{
				return false;
			}
		}
		return true;
	}

	bool WriteToStream(ECStreamWriter& 流, const DynamicVectorClass<T>& 值) const
	{
		流.Save(值.Capacity);
		流.Save(值.Count);
		流.Save(值.CapacityIncrement);
		for (auto ix = 0; ix < 值.Count; ++ix)
		{
			if (!ECSavegameHelper::WriteStream(流, 值.Items[ix]))
			{
				return false;
			}
		}
		return true;
	}
};

template <typename T>
struct ECSavegameHelper::StreamObject<TypeList<T>>
{
	bool ReadFromStream(ECStreamReader& 流, TypeList<T>& 值, bool 注册变化) const
	{
		if (!ECSavegameHelper::ReadStream<DynamicVectorClass<T>>(流, 值, 注册变化))
		{
			return false;
		}
		return 流.Load(值.unknown_18);
	}

	bool WriteToStream(ECStreamWriter& 流, const TypeList<T>& 值) const
	{
		if (!ECSavegameHelper::WriteStream<DynamicVectorClass<T>>(流, 值))
		{
			return false;
		}
		流.Save(值.unknown_18);
		return true;
	}
};

template <typename T>
struct ECSavegameHelper::StreamObject<Vector3D<T>>
{
	bool ReadFromStream(ECStreamReader& 流, Vector3D<T>& 值, bool 注册变化) const
	{
		if (ECSavegameHelper::ReadStream<int>(流, 值.X, 注册变化) && ECSavegameHelper::ReadStream<int>(流, 值.Y, 注册变化) && ECSavegameHelper::ReadStream<int>(流, 值.Z, 注册变化))
		{
			return true;
		}
		return false;
	}

	bool WriteToStream(ECStreamWriter& 流, const Vector3D<T>& 值) const
	{
		if (ECSavegameHelper::WriteStream(流, 值.X) && ECSavegameHelper::WriteStream(流, 值.Y) && ECSavegameHelper::WriteStream(流, 值.Z))
		{
			return true;
		}
		return false;
	}
};

template <>
struct ECSavegameHelper::StreamObject<TintStruct>
{
	bool ReadFromStream(ECStreamReader& 流, TintStruct& 值, bool 注册变化) const
	{
		if (ECSavegameHelper::ReadStream<int>(流, 值.Red, 注册变化) && ECSavegameHelper::ReadStream<int>(流, 值.Green, 注册变化) && ECSavegameHelper::ReadStream<int>(流, 值.Blue, 注册变化))
		{
			return true;
		}
		return false;
	}

	bool WriteToStream(ECStreamWriter& 流, const TintStruct& 值) const
	{
		if (ECSavegameHelper::WriteStream(流, 值.Red) && ECSavegameHelper::WriteStream(流, 值.Green) && ECSavegameHelper::WriteStream(流, 值.Blue))
		{
			return true;
		}
		return false;
	}
};

template <>
struct ECSavegameHelper::StreamObject<CounterClass>
{
	bool ReadFromStream(ECStreamReader& 流, CounterClass& 值, bool 注册变化) const
	{
		if (!ECSavegameHelper::ReadStream<VectorClass<int>>(流, 值, 注册变化))
		{
			return false;
		}
		return 流.Load(值.Total);
	}

	bool WriteToStream(ECStreamWriter& 流, const CounterClass& 值) const
	{
		if (!ECSavegameHelper::WriteStream<VectorClass<int>>(流, 值))
		{
			return false;
		}
		流.Save(值.Total);
		return true;
	}
};

template <size_t Size>
struct ECSavegameHelper::StreamObject<std::bitset<Size>>
{
	bool ReadFromStream(ECStreamReader& 流, std::bitset<Size>& 值, bool 注册变化) const
	{
		unsigned char value = 0;
		for (auto i = 0u; i < Size; ++i)
		{
			auto pos = i % 8;
			if (pos == 0 && !流.Load(value))
			{
				return false;
			}
			值.set(i, ((value >> pos) & 1) != 0);
		}
		return true;
	}

	bool WriteToStream(ECStreamWriter& 流, const std::bitset<Size>& 值) const
	{
		unsigned char value = 0;
		for (auto i = 0u; i < Size; ++i)
		{
			auto pos = i % 8;
			if (值[i])
			{
				value |= 1 << pos;
			}
			if (pos == 7 || i == Size - 1)
			{
				流.Save(value);
				value = 0;
			}
		}
		return true;
	}
};

template <>
struct ECSavegameHelper::StreamObject<std::string>
{
	bool ReadFromStream(ECStreamReader& 流, std::string& 值, bool 注册变化) const
	{
		size_t size = 0;
		if (流.Load(size))
		{
			std::vector<char> buffer(size);
			if (!size || 流.Read(reinterpret_cast<unsigned char*>(buffer.data()), size))
			{
				值.assign(buffer.begin(), buffer.end());
				return true;
			}
		}
		return false;
	}

	bool WriteToStream(ECStreamWriter& 流, const std::string& 值) const
	{
		流.Save(值.size());
		流.Write(reinterpret_cast<const unsigned char*>(值.c_str()), 值.size());
		return true;
	}
};

template <typename T>
struct ECSavegameHelper::StreamObject<std::unique_ptr<T>>
{
	bool ReadFromStream(ECStreamReader& 流, std::unique_ptr<T>& 值, bool 注册变化) const
	{
		值.reset(RestoreObject<T>(流, 注册变化));
		return true;
	}

	bool WriteToStream(ECStreamWriter& 流, const std::unique_ptr<T>& 值) const
	{
		return PersistObject(流, 值.get());
	}
};

template <typename T>
struct ECSavegameHelper::StreamObject<std::shared_ptr<T>>
{
	bool ReadFromStream(ECStreamReader& 流, std::shared_ptr<T>& 值, bool 注册变化) const
	{
		值.reset(RestoreObject<T>(流, 注册变化));
		return true;
	}

	bool WriteToStream(ECStreamWriter& 流, const std::shared_ptr<T>& 值) const
	{
		return PersistObject(流, 值.get());
	}
};

template <typename T>
struct ECSavegameHelper::StreamObject<std::vector<T>>
{
	bool ReadFromStream(ECStreamReader& 流, std::vector<T>& 值, bool 注册变化) const
	{
		值.clear();
		size_t Capacity = 0;
		if (!流.Load(Capacity))
		{
			return false;
		}
		值.reserve(Capacity);
		size_t Count = 0;
		if (!流.Load(Count))
		{
			return false;
		}
		值.resize(Count);
		for (auto ix = 0u; ix < Count; ++ix)
		{
			if (!ECSavegameHelper::ReadStream(流, 值[ix], 注册变化))
			{
				return false;
			}
		}
		return true;
	}

	bool WriteToStream(ECStreamWriter& 流, const std::vector<T>& 值) const
	{
		流.Save(值.capacity());
		流.Save(值.size());
		for (auto ix = 0u; ix < 值.size(); ++ix)
		{
			if (!ECSavegameHelper::WriteStream(流, 值[ix]))
			{
				return false;
			}
		}
		return true;
	}
};

template <typename TKey, typename TValue>
struct ECSavegameHelper::StreamObject<std::pair<TKey, TValue>>
{
	bool ReadFromStream(ECStreamReader& 流, std::pair<TKey, TValue>& 值, bool 注册变化) const
	{
		if (!ECSavegameHelper::ReadStream(流, 值.first, 注册变化) || !ECSavegameHelper::ReadStream(流, 值.second, 注册变化))
		{
			return false;
		}
		return true;
	}

	bool WriteToStream(ECStreamWriter& 流, const std::pair<TKey, TValue>& 值) const
	{
		if (!ECSavegameHelper::WriteStream(流, 值.first) || !ECSavegameHelper::WriteStream(流, 值.second))
		{
			return false;
		}
		return true;
	}
};

template <typename TKey, typename _Pr>
struct ECSavegameHelper::StreamObject<std::set<TKey, _Pr>>
{
	static void is_pointer(std::true_type) = delete;
	static void is_pointer(std::false_type) {}

	bool ReadFromStream(ECStreamReader& 流, std::set<TKey, _Pr>& 值, bool 注册变化) const
	{
		// use pointer in set is unswizzleable
		is_pointer(typename std::is_pointer<TKey>::type());

		值.clear();
		size_t Size = 0;
		if (!流.Load(Size))
		{
			return false;
		}
		for (auto ix = 0u; ix < Size; ++ix)
		{
			TKey buffer = TKey();
			if (!ECSavegameHelper::ReadStream(流, buffer, false))
			{
				return false;
			}
			值.emplace(buffer);
		}
		return true;
	}

	bool WriteToStream(ECStreamWriter& 流, const std::set<TKey, _Pr>& 值) const
	{
		// use pointer in set is unswizzleable
		is_pointer(typename std::is_pointer<TKey>::type());

		流.Save(值.size());
		for (const auto& item : 值)
		{
			if (!ECSavegameHelper::WriteStream(流, item))
			{
				return false;
			}
		}
		return true;
	}
};

template <typename TKey, typename TValue>
struct ECSavegameHelper::StreamObject<std::map<TKey, TValue>>
{
	inline static void is_pointer(std::true_type) = delete;
	inline static void is_pointer(std::false_type) {}

	bool ReadFromStream(ECStreamReader& 流, std::map<TKey, TValue>& 值, bool 注册变化) const
	{
		// use pointer as key of map is unswizzleable
		is_pointer(typename std::is_pointer<TKey>::type());

		值.clear();
		size_t Count = 0;
		if (!流.Load(Count))
		{
			return false;
		}
		for (auto ix = 0u; ix < Count; ++ix)
		{
			TKey key = TKey();
			if (!ECSavegameHelper::ReadStream(流, key, false))
			{
				return false;
			}
			值.emplace(key, TValue());
			if (!ECSavegameHelper::ReadStream(流, 值[key], 注册变化))
			{
				return false;
			}
		}
		return true;
	}

	bool WriteToStream(ECStreamWriter& 流, const std::map<TKey, TValue>& 值) const
	{
		// use pointer as key of map is unswizzleable
		is_pointer(typename std::is_pointer<TKey>::type());

		流.Save(值.size());
		for (const auto& item : 值)
		{
			if (!ECSavegameHelper::WriteStream(流, item.first) || !ECSavegameHelper::WriteStream(流, item.second))
			{
				return false;
			}
		}
		return true;
	}
};

void Internal_DebugLog(const char* pFormat, ...);
template <>
struct ECSavegameHelper::StreamObject<SHPStruct*>
{
	bool ReadFromStream(ECStreamReader& 流, SHPStruct*& 值, bool 注册变化) const
	{
		if (值 && !值->IsReference())
		{
			Internal_DebugLog("SHPStruct* to save contains SHP file data. Possible leak.\n");
		}
		值 = nullptr;
		bool hasValue = true;
		if (ECSavegameHelper::ReadStream(流, hasValue) && hasValue)
		{
			std::string name;
			if (ECSavegameHelper::ReadStream(流, name))
			{
				if (auto pSHP = FileSystem::LoadSHPFile(name.c_str()))
				{
					值 = pSHP;
					return true;
				}
			}
		}
		return !hasValue;
	}

	bool WriteToStream(ECStreamWriter& 流, SHPStruct* const& 值) const
	{
		const char* filename = nullptr;
		if (值)
		{
			if (auto pRef = 值->AsReference())
			{
				filename = pRef->Filename;
			}
			else
			{
				Internal_DebugLog("Cannot save SHPStruct, because it isn't a reference.\n");
			}
		}
		if (ECSavegameHelper::WriteStream(流, filename != nullptr))
		{
			if (filename)
			{
				std::string file(filename);
				return ECSavegameHelper::WriteStream(流, file);
			}
		}
		return filename == nullptr;
	}
};

template <>
struct ECSavegameHelper::StreamObject<RocketStruct>
{
	bool ReadFromStream(ECStreamReader& 流, RocketStruct& 值, bool 注册变化) const
	{
		if (!流.Load(值))
		{
			return false;
		}
		if (注册变化)
		{
			ECSwizzleHelper swizzle(值.Type);
		}
		return true;
	}

	bool WriteToStream(ECStreamWriter& 流, const RocketStruct& 值) const
	{
		流.Save(值);
		return true;
	}
};

template <>
struct ECSavegameHelper::StreamObject<BuildType>
{
	bool ReadFromStream(ECStreamReader& 流, BuildType& 值, bool 注册变化) const
	{
		if (!流.Load(值))
		{
			return false;
		}
		if (注册变化)
		{
			ECSwizzleHelper swizzle(值.CurrentFactory);
		}
		return true;
	}

	bool WriteToStream(ECStreamWriter& 流, const BuildType& 值) const
	{
		流.Save(值);
		return true;
	}
};

template <>
struct ECSavegameHelper::StreamObject<WeaponStruct>
{
	bool ReadFromStream(ECStreamReader& 流, WeaponStruct& 值, bool 注册变化) const
	{
		return ECSavegameHelper::ReadStream(流, 值.WeaponType, 注册变化)
			&& ECSavegameHelper::ReadStream(流, 值.FLH, 注册变化)
			&& ECSavegameHelper::ReadStream(流, 值.BarrelLength, 注册变化)
			&& ECSavegameHelper::ReadStream(流, 值.BarrelThickness, 注册变化)
			&& ECSavegameHelper::ReadStream(流, 值.TurretLocked, 注册变化);
	}

	bool WriteToStream(ECStreamWriter& 流, const WeaponStruct& 值) const
	{
		return ECSavegameHelper::WriteStream(流, 值.WeaponType)
			&& ECSavegameHelper::WriteStream(流, 值.FLH)
			&& ECSavegameHelper::WriteStream(流, 值.BarrelLength)
			&& ECSavegameHelper::WriteStream(流, 值.BarrelThickness)
			&& ECSavegameHelper::WriteStream(流, 值.TurretLocked);
	}
};
