// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "ASTDMinimal.h"

#include "ASTD/Archive.h"

#include "ASTD/Array.h"
#include "ASTD/CString.h"

struct SString
{
	// Types
	/////////////////////////////////

	typedef tchar CharType;
	typedef TArray<CharType> DataType;
	typedef typename DataType::SizeType SizeType;

	typedef CharType* StringIteratorType;
	typedef const CharType* ConstStringIteratorType;

	// Constructors
	/////////////////////////////////

	FORCEINLINE SString() { InitToEmpty(); }

	FORCEINLINE SString(const SString& other) { AppendStringImpl(other); }
	FORCEINLINE SString(SString&& other) noexcept { AppendStringImpl(Move(other)); }

	FORCEINLINE SString(const CharType* text) { AppendCharsImpl(text); }
	FORCEINLINE SString(const CharType* text, SizeType length) { AppendCharsImpl(text, length); }

	// fill constructor
	FORCEINLINE SString(SizeType length, CharType val = CHAR_TERM) { InitToFill(length, val); }

	FORCEINLINE explicit SString(const DataType& data) { AppendDataImpl(data); }
	FORCEINLINE explicit SString(DataType&& data) noexcept { AppendDataImpl(Move(data)); }

	// Gets the empty string as a non-mutable reference
	static const SString& GetEmpty()
	{
		static SString emptyString = SString();
		return emptyString;
	}

	// Compare operators
	/////////////////////////////////

	FORCEINLINE bool operator==(const SString& other) const { return _data == other._data; }
	FORCEINLINE bool operator!=(const SString& other) const { return !operator==(other); }

	// Assign operators
	/////////////////////////////////

	FORCEINLINE SString& operator=(const SString& other) { InitToEmpty(); AppendStringImpl(other); return *this; }
	FORCEINLINE SString& operator=(SString&& other) noexcept { InitToEmpty(); AppendStringImpl(Move(other)); return *this; }

	// Arithmetic operators
	/////////////////////////////////

	FORCEINLINE SString operator+(const SString& other) const { SString tmpStr(*this); tmpStr.AppendStringImpl(other); return *this; }
	FORCEINLINE SString operator+(SString&& other) const { SString tmpStr(*this); tmpStr.AppendStringImpl(Move(other)); return *this; }

	FORCEINLINE SString& operator+=(const SString& other) { AppendStringImpl(other); return *this; }
	FORCEINLINE SString& operator+=(SString&& other) { AppendStringImpl(Move(other)); return *this; }

	// Get operators
	/////////////////////////////////

	FORCEINLINE const CharType* operator*() const { return _data.GetData(); }
	FORCEINLINE CharType* operator*() { return _data.GetData(); }

	FORCEINLINE CharType operator[](SizeType idx) const { return _data[idx]; }

	// Path operators
	/////////////////////////////////

	FORCEINLINE SString operator/(const SString& other) const { SString tmpStr(*this); tmpStr.operator/=(other); return tmpStr; }
	FORCEINLINE SString operator/=(const SString& other) { AppendCharsImpl(TEXT("/")); AppendStringImpl(other); return *this; }

	// Property getters
	/////////////////////////////////

	FORCEINLINE const DataType& GetData() const { return _data; }

	FORCEINLINE const CharType* GetChars() const { return _data.GetData(); }
	FORCEINLINE CharType* GetChars() { return _data.GetData(); }

	// Gets length of the string WITHOUT terminating character
	FORCEINLINE SizeType GetLength() const { return _data.GetNum() > 1 ? _data.GetNum() - 1 : 0; }

	// Checks whether specific index within the string can be safely dereferenced 
	FORCEINLINE bool IsValidIndex(SizeType idx) const { return idx >= 0 && idx < GetLastCharIndex(); }

	// Checks whether string is empty
	FORCEINLINE bool IsEmpty() const { return GetLength() == 0; }

	// Construction
	/////////////////////////////////

	template<
		typename StringT,
		typename... VarTypes>
	static SString Printf(StringT&& fmt, VarTypes&&... args)
	{
		static_assert(sizeof...(VarTypes) > 0, "No arguments provided. Use construction from fmt directly instead");
		static_assert(TIsSame<typename TPure<StringT>::Type, CharType*>::Value || TIsSame<typename TPure<StringT>::Type, SString>::Value, "Format variable has to be string type");
		thread_local CharType buffer[SCString::LARGE_BUFFER_SIZE];

		// TODO: Replace with custom implementation
		if constexpr (TIsSame<CharType, wchar>::Value)
		{
			swprintf(buffer, SCString::LARGE_BUFFER_SIZE, fmt, Forward<VarTypes>(args)...);
		}
		else
		{
			snprintf(buffer, SCString::LARGE_BUFFER_SIZE, fmt, Forward<VarTypes>(args)...);
		}

		return SString(buffer);
	}

	// Conversions
	/////////////////////////////////

	// Converts string to int32 equivalent
	// * "10" => 10
	FORCEINLINE int32 ToInt32() const { return SCString::ToInt32(_data.GetData()); }

	// Converts string to int64 equivalent
	// * "10" => 10
	FORCEINLINE int64 ToInt64() const { return SCString::ToInt64(_data.GetData()); }

	// Converts string to int64 equivalent
	// * "10.1" => 10.1
	FORCEINLINE double ToDouble() const { return SCString::ToDouble(_data.GetData()); }

	// Constructs new string from int32
	// * 10 => "10"
	static SString FromInt32(int32 val)
	{
		thread_local CharType buffer[SCString::MAX_BUFFER_SIZE_INT32];
		return SString(SCString::FromInt32(val, buffer, SCString::MAX_BUFFER_SIZE_INT32));
	}

	// Constructs new string from int64
	// * 10 => "10"
	static SString FromInt64(int64 val)
	{
		thread_local CharType buffer[SCString::MAX_BUFFER_SIZE_INT64];
		return SString(SCString::FromInt64(val, buffer, SCString::MAX_BUFFER_SIZE_INT64));
	}

	// Constructs new string from double, providing number of digits to expect
	// * 10.1 => "10.1"
	static SString FromDouble(double val, uint8 digits)
	{
		thread_local CharType buffer[SCString::MAX_BUFFER_SIZE_DOUBLE];
		return SString(SCString::FromDouble(val, digits, buffer, SCString::MAX_BUFFER_SIZE_DOUBLE));
	}

	// Iterations
	/////////////////////////////////

	FORCEINLINE StringIteratorType begin() { return _data.GetNum() > 1 ? &_data[0] : nullptr; }
	FORCEINLINE ConstStringIteratorType begin() const { return _data.GetNum() > 1 ? &_data[0] : nullptr; }

	FORCEINLINE StringIteratorType end() { return _data.GetNum() > 1 ? &_data[GetLastCharIndex()] : nullptr; }
	FORCEINLINE ConstStringIteratorType end() const { return _data.GetNum() > 1 ? &_data[GetLastCharIndex()] : nullptr; }

	// Compares
	/////////////////////////////////

	// Compares this string against the provided one
	// * returns 0 if equal, -1 if this string is "bigger" and 1 if provided string is "bigger"
	FORCEINLINE int32 Compare(const SString& other, bool caseSensitive = true) const { return SCString::Compare(GetChars(), other.GetChars(), caseSensitive); }

	// Checks whether this string is same as the provided one
	// * Is same as Compare == 0
	FORCEINLINE bool Equals(const SString& other, bool caseSensitive = true) const { return Compare(other, caseSensitive) == 0; }

	// Checks
	/////////////////////////////////

	// Checks whether this string contains ONLY whitespaces
	FORCEINLINE bool IsWhitespace() const
	{
		if(GetLength() > 0)
		{
			const CharType* data = _data.GetData();
			while(*data != CHAR_TERM)
			{
				if(!SCString::IsWhitespaceChar(*data))
					return false;

				++data;
			}
		}

		return true;
	}

	// Checks whether this string contains provided string from the beginning
	FORCEINLINE bool StartsWith(const SString& val, bool caseSensitive = true) const
	{
		return ContainsAtIndexImpl(*this, val, 0, caseSensitive);
	}

	// Checks whether this string contains provided string from the end
	FORCEINLINE bool EndsWith(const SString& val, bool caseSensitive = true) const
	{
		return ContainsAtIndexImpl(*this, val, GetLastCharIndex() - val.GetLastCharIndex(), caseSensitive);
	}

	// Checks whether this string contains provided string in any place
	FORCEINLINE bool Contains(const SString& val, bool caseSensitive = true, bool fromStart = true) const
	{
		return !!SCString::Find(GetChars(), val.GetChars(), caseSensitive, fromStart);
	}

	// Checks whether this string contains provided string in provided index
	FORCEINLINE bool ContainsAt(const SString& val, SizeType index, bool caseSensitive = true)
	{
		return ContainsAtIndexImpl(*this, val, index, caseSensitive);
	}

	// Gets index from which this string contains provided string
	FORCEINLINE SizeType Find(const SString& val, bool caseSensitive = true, bool fromStart = true) const
	{
		return SCString::FindIndex(GetChars(), val.GetChars(), caseSensitive, fromStart);
	}

	// Append
	/////////////////////////////////

	// Appends this string with other string
	FORCEINLINE void Append(const SString& other) { AppendStringImpl(other); }
	FORCEINLINE void Append(SString&& other) { AppendStringImpl(Move(other)); }
	FORCEINLINE void Append(const CharType* other, SizeType num = INDEX_NONE) { AppendCharsImpl(other, num); }

	// Appends this string via "printf"
	template<typename StringT, typename... ArgTypes>
	FORCEINLINE void AppendPrintf(StringT&& fmt, ArgTypes&&... args)
	{
		AppendStringImpl(
			Move(
				SString::Printf(
					Forward<StringT>(fmt),
					Forward<ArgTypes>(args)...
				)
			)
		);
	}

	// Const manipulation
	/////////////////////////////////

	bool Split(const SString& val, SString* outLeft, SString* outRight, bool caseSensitive = true, bool fromStart = true) const
	{
		const SizeType foundIdx = SCString::FindIndex(GetChars(), val.GetChars(), caseSensitive, fromStart);
		if (foundIdx == INDEX_NONE)
			return false;

		if(outLeft)
		{
			outLeft->_data = DataType(_data.GetData(), foundIdx + 1);
			outLeft->_data[foundIdx] = CHAR_TERM;
		}

		if(outRight)
		{
			outRight->_data = DataType(GetChars() + foundIdx + 1, _data.GetNum() - foundIdx);
		}

		return true;
	}

	TArray<SString> SplitToArray(const SString& delimiter, bool discardEmpty = true, SizeType num = INDEX_NONE, bool caseSensitive = true) const
	{
		TArray<SString> result;

		SplitBySubstringPrivate(*this, delimiter, discardEmpty, caseSensitive, _data.GetNum(),
			[&result, &num](const CharType* ptr, SizeType count) -> bool
			{
				SString& newStr = result.AddUninitialized_GetRef();
				newStr._data = DataType(ptr, count);
				newStr._data.Add(CHAR_TERM);
				return (--num == 0);
			}
		);

		return result;
	}

	// Manipulation
	/////////////////////////////////

	SString Replace(const SString& from, const SString& to, SizeType num = INDEX_NONE, bool caseSensitive = true) const
	{
		SString newString(*this);
		newString.ReplaceInline(from, to, num, caseSensitive);
		return newString;
	}

	// -1 = All
	void ReplaceInline(const SString& from, const SString& to, SizeType num = INDEX_NONE, bool caseSensitive = true)
	{
		DataType newData(_data.GetNum(), true);
		SplitBySubstringPrivate(*this, from, false, caseSensitive, (num == -1) ? _data.GetNum() : num,
			[&newData, &to, &num](const CharType* ptr, SizeType count) -> bool
			{
				const bool isLast = (*(ptr + count + 1) == CHAR_TERM);

				if(count > 0)
				{
					newData.Append(ptr, count);
				}

				if(!isLast)
				{
					if(to._data.GetNum() > 1)
					{
						newData.Append(to._data.GetData(), to._data.GetNum() - 1);
					}
				}

				if(isLast)
				{
					newData.Add(CHAR_TERM);

					// is redundant, but if implementation changes this might save a day
					return true;
				}

				return false;
			}
		);

		if(newData.GetNum() > 0)
		{
			_data.Replace(newData);
		}
	}

	SString ToUpper() const
	{
		SString newString(*this);
		newString.ToUpperInline();
		return newString;
	}

	FORCEINLINE void ToUpperInline() { SCString::ToUpper(_data.GetData()); }

	SString ToLower() const
	{
		SString newString(*this);
		newString.ToLowerInline();
		return newString;
	}

	FORCEINLINE void ToLowerInline() { SCString::ToLower(_data.GetData()); }

	// Removes all characters from the index position to the end of the string
	// * Does NOT modify the source string
	// * ChopRight at index 1 for "ABC" returns "A"
	SString ChopRight(SizeType idx) const
	{
		SString newString(*this);
		newString.ChopRightInline(idx);
		return newString;
	}

	// Removes all characters from the index position to the end of the string
	// * Modifies the source string
	// * ChopRightInline at index 1 for "ABC" returns "A"
	void ChopRightInline(SizeType idx)
	{
		if(!IsValidIndex(idx))
		{
			return;
		}

		_data.Resize(idx + 1);
		_data[idx] = CHAR_TERM;
	}

	// Removes all characters from the start of the string to the index position
	// * Does NOT modify the source string
	// * ChopLeft at index 1 for "ABC" returns "C"
	SString ChopLeft(SizeType idx) const
	{
		SString newString(*this);
		newString.ChopLeftInline(idx);
		return newString;
	}

	// Removes all characters from the start of the string to the index position
	// * Modifies the source string
	// * ChopLeftInline at index 1 for "ABC" returns "C"
	void ChopLeftInline(SizeType idx)
	{
		if(!IsValidIndex(idx))
		{
			return;
		}

		DataType newData(_data.GetNum() - idx);
		for(SizeType i = 0; i < newData.GetNum(); ++i)
		{
			newData[i] = _data[idx + i];
		}

		_data = Move(newData);
	}

	SString ChopRange(SizeType firstIdx, SizeType secondIdx) const
	{
		SString newString(*this);
		newString.ChopRangeInline(firstIdx, secondIdx);
		return newString;
	}

	void ChopRangeInline(SizeType firstIdx, SizeType secondIdx)
	{
		if( !IsValidIndex(firstIdx) || !IsValidIndex(secondIdx))
		{
			return;
		}

		SizeType* biggerVal, *smallerVal;
		if(firstIdx > secondIdx)
		{
			biggerVal = &firstIdx;
			smallerVal = &secondIdx;
		}
		else
		{
			biggerVal = &secondIdx;
			smallerVal = &firstIdx;
		}

		ChopRightInline(*biggerVal);
		ChopLeftInline(*smallerVal);
	}

	// Reset
	/////////////////////////////////

	FORCEINLINE void Reset() { EmptyImpl(true); }
	FORCEINLINE void Empty(bool releaseResources = true) { EmptyImpl(releaseResources); }

	// Other
	/////////////////////////////////

	FORCEINLINE void Fill(SizeType length, CharType val = CHAR_TERM) { InitToFill(length, val); }
	FORCEINLINE void Reserve(SizeType num) { _data.Reserve(num + 1); } // termination character
	FORCEINLINE void ShrinkToFit() { _data.ShrinkToFit(); }

private:
	FORCEINLINE void InitToEmpty()
	{
		_data.Empty(1);
		AddTermChecked(_data);
	}

	FORCEINLINE void InitToFill(SizeType length, CharType val)
	{
		_data.Resize(length);
		SMemory::FillTyped(_data.GetData(), val, length);
	}
	
	template<
		typename DataT,
		typename TEnableIf<TIsSame<typename TDecay<DataT>::Type, DataType>::Value>::Type* = nullptr>
	void AppendDataImpl(DataT&& data)
	{
		RemoveTerm(_data);
		_data.Append(MoveIfPossible(data));
		AddTerm(_data);
	}

	template<
		typename StringT,
		typename TEnableIf<TIsSame<typename TDecay<StringT>::Type, SString>::Value>::Type* = nullptr>
	void AppendStringImpl(StringT&& str)
	{
		RemoveTerm(_data);
		_data.Append(MoveIfPossible(str._data));
		AddTerm(_data);
	}

	void AppendCharsImpl(const CharType* text, SizeType textLen = INDEX_NONE)
	{
		RemoveTerm(_data);
		if (text)
		{
			_data.Append(text, textLen > 0 ? textLen : SCString::GetLength(text) + 1);
		}
		AddTerm(_data);
	}

	FORCEINLINE void EmptyImpl(bool keepResources) { _data.Empty(keepResources); }
	FORCEINLINE SizeType GetLastCharIndex() const { return _data.GetNum() - 2; }

	FORCEINLINE_DEBUGGABLE static bool HasTerm(const DataType& data) { return !data.IsEmpty() && *data.GetLast() == CHAR_TERM; }
	FORCEINLINE static void AddTermChecked(DataType& data) { data.Add(CHAR_TERM); }
	FORCEINLINE static void AddTerm(DataType& data) { if (!HasTerm(data)) { AddTermChecked(data); }}
	FORCEINLINE static void RemoveTermChecked(DataType& data) { data.RemoveAt(data.GetNum() - 1); }
	FORCEINLINE static void RemoveTerm(DataType& data) { if (HasTerm(data)) { RemoveTermChecked(data); }}

	static bool ContainsAtIndexImpl(const SString& str, const SString& val, SizeType idx, bool caseSensitive)
	{
		if(idx < 0 || str.GetLength() < idx + val.GetLength())
			return false;

		for(SizeType i = idx; i < idx + val.GetLastCharIndex(); ++i)
		{
			CharType lhs = str._data[i];
			CharType rhs = val._data[i];

			if(!caseSensitive)
			{
				lhs = SCString::ToLowerChar(lhs);
				rhs = SCString::ToLowerChar(rhs);
			}

			if(lhs != rhs)
			{
				return false;
			}
		}

		return true;
	}

	template<typename FuncType>
	static void SplitBySubstringPrivate(
		const SString& str, const SString& substr,
		bool ignoreEmpty, bool caseSensitive,
		SizeType maxSplits,
		FuncType&& functor)
	{
		const SizeType mainLen = str.GetLength();
		const SizeType subLen = substr.GetLength();

		if(mainLen > 0 && mainLen > subLen)
		{
			const TArray<CharType> mainStr = caseSensitive ? str._data : str.ToLower()._data;
			const TArray<CharType> subStr = caseSensitive ? substr._data : substr.ToLower()._data;

			const CharType* init = mainStr.GetData();
			const CharType* subChars = subStr.GetData();

			while(const CharType* current = SCString::Find(init, subChars, caseSensitive))
			{
				if (!ignoreEmpty || current-init)
				{
					const SizeType currIdx = PTR_DIFF_TYPED(SizeType, init, mainStr.GetData());
					const SizeType count = PTR_DIFF_TYPED(SizeType, current, init);

					if(functor(mainStr.GetData() + currIdx, count))
						return;
				}

				init = current + subLen;

				if(--maxSplits == 0)
				{
					break;
				}
			}

			if (!ignoreEmpty || *init != CHAR_TERM)
			{
				const SizeType currIdx = PTR_DIFF_TYPED(SizeType, init, mainStr.GetData());
				const SizeType count = mainStr.GetNum() - currIdx;

				functor(mainStr.GetData() + currIdx, count);
			}
		}
	}

	DataType _data = {};
};

template<>
struct TContainerTypeTraits<SString> : public TContainerTypeTraits<void>
{
	using ElementType = SString::CharType;
	using AllocatorType = SString::DataType::AllocatorType;

	enum
	{
		IsContainer = true,
		IsDynamic = true,
		InlineMemory = true
	};
};

// Archive operator<< && operator>>
////////////////////////////////////////////

FORCEINLINE_DEBUGGABLE static SArchive& operator<<(SArchive& ar, const SString& str)
{
	ar.Write(str.GetChars(), str.GetLength());
	return ar;
}

FORCEINLINE_DEBUGGABLE static SArchive& operator>>(SArchive& ar, SString& str)
{
	SString::DataType newData;
	ar >> newData;
	str = SString(Move(newData));
	return ar;
}
