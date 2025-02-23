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
	bool ReadStream(ECStreamReader& ��, T& ֵ, bool ע��仯);
	template <typename T>
	bool WriteStream(ECStreamWriter& ��, const T& ֵ);
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
			static bool ReadFromStream(ECStreamReader& ��, T& ֵ, bool ע��仯)
			{
				return read_from_stream(��, ֵ, ע��仯, 0, 0);
			}

			template <typename T>
			static bool WriteToStream(ECStreamWriter& ��, const T& ֵ)
			{
				return write_to_stream(��, ֵ, 0, 0);
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
			static auto read_from_stream(ECStreamReader& ��, T& ֵ, bool ע��仯, int, int) -> decltype(ֵ.Load(��, ע��仯))
			{
				return ֵ.Load(��, ע��仯);
			}

			template <typename T>
			static auto read_from_stream(ECStreamReader& ��, T& ֵ, bool ע��仯, Dummy, int) -> decltype(ֵ.load(��, ע��仯))
			{
				return ֵ.load(��, ע��仯);
			}

			template <typename T>
			static bool read_from_stream(ECStreamReader& ��, T& ֵ, bool ע��仯, Dummy, Dummy)
			{
				StreamObject<T> item;
				return item.ReadFromStream(��, ֵ, ע��仯);
			}

			template <typename T>
			static auto write_to_stream(ECStreamWriter& ��, const T& ֵ, int, int) -> decltype(ֵ.Save(��))
			{
				return ֵ.Save(��);
			}

			template <typename T>
			static auto write_to_stream(ECStreamWriter& ��, const T& ֵ, Dummy, int) -> decltype(ֵ.save(��))
			{
				return ֵ.save(��);
			}

			template <typename T>
			static bool write_to_stream(ECStreamWriter& ��, const T& ֵ, Dummy, Dummy)
			{
				StreamObject<T> item;
				return item.WriteToStream(��, ֵ);
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
		std::unique_ptr<T> operator() (ECStreamReader& ��, bool ע��仯) const
		{
			return load(��, ע��仯, 0);
		}

	private:
		static auto load(ECStreamReader& ��, bool ע��仯, int) -> decltype(T::LoadOne(��, ע��仯))
		{
			return T::LoadOne(��, ע��仯);
		}

		static auto load(ECStreamReader& ��, bool ע��仯, ECHelper_SGDummy)
		{
			return std::make_unique<T>();
		}
	};

	template <typename T>
	struct SaveFactory
	{
		bool operator() (ECStreamWriter& ��, const T* ֵ)
		{
			return save(��, ֵ, 0);
		}

	private:
		static auto save(ECStreamWriter& ��, const T* ֵ, int) -> decltype(T::SaveOne(��, ֵ))
		{
			return T::SaveOne(��, ֵ);
		}

		static auto save(ECStreamWriter& ��, const T* ֵ, ECHelper_SGDummy)
		{
			return true;
		}
	};

	template <typename T>
	bool ReadStream(ECStreamReader& ��, T& ֵ, bool ע��仯 = true)
	{
		return detail::Selector::ReadFromStream(��, ֵ, ע��仯);
	}

	template <typename T>
	bool WriteStream(ECStreamWriter& ��, const T& ֵ)
	{
		return detail::Selector::WriteToStream(��, ֵ);
	}

	template <typename T>
	T* RestoreObject(ECStreamReader& ��, bool ע��仯 = true)
	{
		T* ��ָ�� = nullptr;
		if (!��.Load(��ָ��))
		{
			return nullptr;
		}
		if (��ָ��)
		{
			std::unique_ptr<T> ��ָ�� = LoadFactory<T>()(��, ע��仯);
			if (ECSavegameHelper::ReadStream(��, *��ָ��, ע��仯))
			{
				BaseSwizzle::Instance.RegisterChange(��ָ��, ��ָ��.get());
				return ��ָ��.release();
			}
		}
		return nullptr;
	}

	template <typename T>
	bool PersistObject(ECStreamWriter& ��, const T* ֵ)
	{
		if (!ECSavegameHelper::WriteStream(��, ֵ))
		{
			return false;
		}
		if (ֵ)
		{
			return SaveFactory<T>()(��, ֵ) && ECSavegameHelper::WriteStream(��, *ֵ);
		}
		return true;
	}


	template <typename T>
	struct StreamObject
	{
		bool ReadFromStream(ECStreamReader& ��, T& ֵ, bool ע��仯) const
		{
			bool ������ = ��.Load(ֵ);
			if (ע��仯)
			{
				ECSwizzleHelper swizzle(ֵ);
			}
			return ������;
		}

		bool WriteToStream(ECStreamWriter& ��, const T& ֵ) const
		{
			��.Save(ֵ);
			return true;
		}
	};
};


template <typename T>
struct ECSavegameHelper::StreamObject<VectorClass<T>>
{
	bool ReadFromStream(ECStreamReader& ��, VectorClass<T>& ֵ, bool ע��仯) const
	{
		ֵ.Clear();
		int Capacity = 0;
		if (!��.Load(Capacity))
		{
			return false;
		}
		ֵ.Reserve(Capacity);
		for (auto ix = 0; ix < Capacity; ++ix)
		{
			if (!ECSavegameHelper::ReadStream(��, ֵ.Items[ix], ע��仯))
			{
				return false;
			}
		}
		return true;
	}

	bool WriteToStream(ECStreamWriter& ��, const VectorClass<T>& ֵ) const
	{
		��.Save(ֵ.Capacity);
		for (auto ix = 0; ix < ֵ.Capacity; ++ix)
		{
			if (!ECSavegameHelper::WriteStream(��, ֵ.Items[ix]))
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
	bool ReadFromStream(ECStreamReader& ��, DynamicVectorClass<T>& ֵ, bool ע��仯) const
	{
		ֵ.Clear();
		int Capacity = 0;
		if (!��.Load(Capacity))
		{
			return false;
		}
		ֵ.Reserve(Capacity);
		if (!��.Load(ֵ.Count) || !��.Load(ֵ.CapacityIncrement))
		{
			return false;
		}
		for (auto ix = 0; ix < ֵ.Count; ++ix)
		{
			if (!ECSavegameHelper::ReadStream(��, ֵ.Items[ix], ע��仯))
			{
				return false;
			}
		}
		return true;
	}

	bool WriteToStream(ECStreamWriter& ��, const DynamicVectorClass<T>& ֵ) const
	{
		��.Save(ֵ.Capacity);
		��.Save(ֵ.Count);
		��.Save(ֵ.CapacityIncrement);
		for (auto ix = 0; ix < ֵ.Count; ++ix)
		{
			if (!ECSavegameHelper::WriteStream(��, ֵ.Items[ix]))
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
	bool ReadFromStream(ECStreamReader& ��, TypeList<T>& ֵ, bool ע��仯) const
	{
		if (!ECSavegameHelper::ReadStream<DynamicVectorClass<T>>(��, ֵ, ע��仯))
		{
			return false;
		}
		return ��.Load(ֵ.unknown_18);
	}

	bool WriteToStream(ECStreamWriter& ��, const TypeList<T>& ֵ) const
	{
		if (!ECSavegameHelper::WriteStream<DynamicVectorClass<T>>(��, ֵ))
		{
			return false;
		}
		��.Save(ֵ.unknown_18);
		return true;
	}
};

template <typename T>
struct ECSavegameHelper::StreamObject<Vector3D<T>>
{
	bool ReadFromStream(ECStreamReader& ��, Vector3D<T>& ֵ, bool ע��仯) const
	{
		if (ECSavegameHelper::ReadStream<int>(��, ֵ.X, ע��仯) && ECSavegameHelper::ReadStream<int>(��, ֵ.Y, ע��仯) && ECSavegameHelper::ReadStream<int>(��, ֵ.Z, ע��仯))
		{
			return true;
		}
		return false;
	}

	bool WriteToStream(ECStreamWriter& ��, const Vector3D<T>& ֵ) const
	{
		if (ECSavegameHelper::WriteStream(��, ֵ.X) && ECSavegameHelper::WriteStream(��, ֵ.Y) && ECSavegameHelper::WriteStream(��, ֵ.Z))
		{
			return true;
		}
		return false;
	}
};

template <>
struct ECSavegameHelper::StreamObject<TintStruct>
{
	bool ReadFromStream(ECStreamReader& ��, TintStruct& ֵ, bool ע��仯) const
	{
		if (ECSavegameHelper::ReadStream<int>(��, ֵ.Red, ע��仯) && ECSavegameHelper::ReadStream<int>(��, ֵ.Green, ע��仯) && ECSavegameHelper::ReadStream<int>(��, ֵ.Blue, ע��仯))
		{
			return true;
		}
		return false;
	}

	bool WriteToStream(ECStreamWriter& ��, const TintStruct& ֵ) const
	{
		if (ECSavegameHelper::WriteStream(��, ֵ.Red) && ECSavegameHelper::WriteStream(��, ֵ.Green) && ECSavegameHelper::WriteStream(��, ֵ.Blue))
		{
			return true;
		}
		return false;
	}
};

template <>
struct ECSavegameHelper::StreamObject<CounterClass>
{
	bool ReadFromStream(ECStreamReader& ��, CounterClass& ֵ, bool ע��仯) const
	{
		if (!ECSavegameHelper::ReadStream<VectorClass<int>>(��, ֵ, ע��仯))
		{
			return false;
		}
		return ��.Load(ֵ.Total);
	}

	bool WriteToStream(ECStreamWriter& ��, const CounterClass& ֵ) const
	{
		if (!ECSavegameHelper::WriteStream<VectorClass<int>>(��, ֵ))
		{
			return false;
		}
		��.Save(ֵ.Total);
		return true;
	}
};

template <size_t Size>
struct ECSavegameHelper::StreamObject<std::bitset<Size>>
{
	bool ReadFromStream(ECStreamReader& ��, std::bitset<Size>& ֵ, bool ע��仯) const
	{
		unsigned char value = 0;
		for (auto i = 0u; i < Size; ++i)
		{
			auto pos = i % 8;
			if (pos == 0 && !��.Load(value))
			{
				return false;
			}
			ֵ.set(i, ((value >> pos) & 1) != 0);
		}
		return true;
	}

	bool WriteToStream(ECStreamWriter& ��, const std::bitset<Size>& ֵ) const
	{
		unsigned char value = 0;
		for (auto i = 0u; i < Size; ++i)
		{
			auto pos = i % 8;
			if (ֵ[i])
			{
				value |= 1 << pos;
			}
			if (pos == 7 || i == Size - 1)
			{
				��.Save(value);
				value = 0;
			}
		}
		return true;
	}
};

template <>
struct ECSavegameHelper::StreamObject<std::string>
{
	bool ReadFromStream(ECStreamReader& ��, std::string& ֵ, bool ע��仯) const
	{
		size_t size = 0;
		if (��.Load(size))
		{
			std::vector<char> buffer(size);
			if (!size || ��.Read(reinterpret_cast<unsigned char*>(buffer.data()), size))
			{
				ֵ.assign(buffer.begin(), buffer.end());
				return true;
			}
		}
		return false;
	}

	bool WriteToStream(ECStreamWriter& ��, const std::string& ֵ) const
	{
		��.Save(ֵ.size());
		��.Write(reinterpret_cast<const unsigned char*>(ֵ.c_str()), ֵ.size());
		return true;
	}
};

template <typename T>
struct ECSavegameHelper::StreamObject<std::unique_ptr<T>>
{
	bool ReadFromStream(ECStreamReader& ��, std::unique_ptr<T>& ֵ, bool ע��仯) const
	{
		ֵ.reset(RestoreObject<T>(��, ע��仯));
		return true;
	}

	bool WriteToStream(ECStreamWriter& ��, const std::unique_ptr<T>& ֵ) const
	{
		return PersistObject(��, ֵ.get());
	}
};

template <typename T>
struct ECSavegameHelper::StreamObject<std::shared_ptr<T>>
{
	bool ReadFromStream(ECStreamReader& ��, std::shared_ptr<T>& ֵ, bool ע��仯) const
	{
		ֵ.reset(RestoreObject<T>(��, ע��仯));
		return true;
	}

	bool WriteToStream(ECStreamWriter& ��, const std::shared_ptr<T>& ֵ) const
	{
		return PersistObject(��, ֵ.get());
	}
};

template <typename T>
struct ECSavegameHelper::StreamObject<std::vector<T>>
{
	bool ReadFromStream(ECStreamReader& ��, std::vector<T>& ֵ, bool ע��仯) const
	{
		ֵ.clear();
		size_t Capacity = 0;
		if (!��.Load(Capacity))
		{
			return false;
		}
		ֵ.reserve(Capacity);
		size_t Count = 0;
		if (!��.Load(Count))
		{
			return false;
		}
		ֵ.resize(Count);
		for (auto ix = 0u; ix < Count; ++ix)
		{
			if (!ECSavegameHelper::ReadStream(��, ֵ[ix], ע��仯))
			{
				return false;
			}
		}
		return true;
	}

	bool WriteToStream(ECStreamWriter& ��, const std::vector<T>& ֵ) const
	{
		��.Save(ֵ.capacity());
		��.Save(ֵ.size());
		for (auto ix = 0u; ix < ֵ.size(); ++ix)
		{
			if (!ECSavegameHelper::WriteStream(��, ֵ[ix]))
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
	bool ReadFromStream(ECStreamReader& ��, std::pair<TKey, TValue>& ֵ, bool ע��仯) const
	{
		if (!ECSavegameHelper::ReadStream(��, ֵ.first, ע��仯) || !ECSavegameHelper::ReadStream(��, ֵ.second, ע��仯))
		{
			return false;
		}
		return true;
	}

	bool WriteToStream(ECStreamWriter& ��, const std::pair<TKey, TValue>& ֵ) const
	{
		if (!ECSavegameHelper::WriteStream(��, ֵ.first) || !ECSavegameHelper::WriteStream(��, ֵ.second))
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

	bool ReadFromStream(ECStreamReader& ��, std::set<TKey, _Pr>& ֵ, bool ע��仯) const
	{
		// use pointer in set is unswizzleable
		is_pointer(typename std::is_pointer<TKey>::type());

		ֵ.clear();
		size_t Size = 0;
		if (!��.Load(Size))
		{
			return false;
		}
		for (auto ix = 0u; ix < Size; ++ix)
		{
			TKey buffer = TKey();
			if (!ECSavegameHelper::ReadStream(��, buffer, false))
			{
				return false;
			}
			ֵ.emplace(buffer);
		}
		return true;
	}

	bool WriteToStream(ECStreamWriter& ��, const std::set<TKey, _Pr>& ֵ) const
	{
		// use pointer in set is unswizzleable
		is_pointer(typename std::is_pointer<TKey>::type());

		��.Save(ֵ.size());
		for (const auto& item : ֵ)
		{
			if (!ECSavegameHelper::WriteStream(��, item))
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

	bool ReadFromStream(ECStreamReader& ��, std::map<TKey, TValue>& ֵ, bool ע��仯) const
	{
		// use pointer as key of map is unswizzleable
		is_pointer(typename std::is_pointer<TKey>::type());

		ֵ.clear();
		size_t Count = 0;
		if (!��.Load(Count))
		{
			return false;
		}
		for (auto ix = 0u; ix < Count; ++ix)
		{
			TKey key = TKey();
			if (!ECSavegameHelper::ReadStream(��, key, false))
			{
				return false;
			}
			ֵ.emplace(key, TValue());
			if (!ECSavegameHelper::ReadStream(��, ֵ[key], ע��仯))
			{
				return false;
			}
		}
		return true;
	}

	bool WriteToStream(ECStreamWriter& ��, const std::map<TKey, TValue>& ֵ) const
	{
		// use pointer as key of map is unswizzleable
		is_pointer(typename std::is_pointer<TKey>::type());

		��.Save(ֵ.size());
		for (const auto& item : ֵ)
		{
			if (!ECSavegameHelper::WriteStream(��, item.first) || !ECSavegameHelper::WriteStream(��, item.second))
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
	bool ReadFromStream(ECStreamReader& ��, SHPStruct*& ֵ, bool ע��仯) const
	{
		if (ֵ && !ֵ->IsReference())
		{
			Internal_DebugLog("SHPStruct* to save contains SHP file data. Possible leak.\n");
		}
		ֵ = nullptr;
		bool hasValue = true;
		if (ECSavegameHelper::ReadStream(��, hasValue) && hasValue)
		{
			std::string name;
			if (ECSavegameHelper::ReadStream(��, name))
			{
				if (auto pSHP = FileSystem::LoadSHPFile(name.c_str()))
				{
					ֵ = pSHP;
					return true;
				}
			}
		}
		return !hasValue;
	}

	bool WriteToStream(ECStreamWriter& ��, SHPStruct* const& ֵ) const
	{
		const char* filename = nullptr;
		if (ֵ)
		{
			if (auto pRef = ֵ->AsReference())
			{
				filename = pRef->Filename;
			}
			else
			{
				Internal_DebugLog("Cannot save SHPStruct, because it isn't a reference.\n");
			}
		}
		if (ECSavegameHelper::WriteStream(��, filename != nullptr))
		{
			if (filename)
			{
				std::string file(filename);
				return ECSavegameHelper::WriteStream(��, file);
			}
		}
		return filename == nullptr;
	}
};

template <>
struct ECSavegameHelper::StreamObject<RocketStruct>
{
	bool ReadFromStream(ECStreamReader& ��, RocketStruct& ֵ, bool ע��仯) const
	{
		if (!��.Load(ֵ))
		{
			return false;
		}
		if (ע��仯)
		{
			ECSwizzleHelper swizzle(ֵ.Type);
		}
		return true;
	}

	bool WriteToStream(ECStreamWriter& ��, const RocketStruct& ֵ) const
	{
		��.Save(ֵ);
		return true;
	}
};

template <>
struct ECSavegameHelper::StreamObject<BuildType>
{
	bool ReadFromStream(ECStreamReader& ��, BuildType& ֵ, bool ע��仯) const
	{
		if (!��.Load(ֵ))
		{
			return false;
		}
		if (ע��仯)
		{
			ECSwizzleHelper swizzle(ֵ.CurrentFactory);
		}
		return true;
	}

	bool WriteToStream(ECStreamWriter& ��, const BuildType& ֵ) const
	{
		��.Save(ֵ);
		return true;
	}
};

template <>
struct ECSavegameHelper::StreamObject<WeaponStruct>
{
	bool ReadFromStream(ECStreamReader& ��, WeaponStruct& ֵ, bool ע��仯) const
	{
		return ECSavegameHelper::ReadStream(��, ֵ.WeaponType, ע��仯)
			&& ECSavegameHelper::ReadStream(��, ֵ.FLH, ע��仯)
			&& ECSavegameHelper::ReadStream(��, ֵ.BarrelLength, ע��仯)
			&& ECSavegameHelper::ReadStream(��, ֵ.BarrelThickness, ע��仯)
			&& ECSavegameHelper::ReadStream(��, ֵ.TurretLocked, ע��仯);
	}

	bool WriteToStream(ECStreamWriter& ��, const WeaponStruct& ֵ) const
	{
		return ECSavegameHelper::WriteStream(��, ֵ.WeaponType)
			&& ECSavegameHelper::WriteStream(��, ֵ.FLH)
			&& ECSavegameHelper::WriteStream(��, ֵ.BarrelLength)
			&& ECSavegameHelper::WriteStream(��, ֵ.BarrelThickness)
			&& ECSavegameHelper::WriteStream(��, ֵ.TurretLocked);
	}
};
