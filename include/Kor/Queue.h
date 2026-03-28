// Copyright Jan Kristian Fisera. All Rights Reserved.
// Licensed under the MIT License. See LICENSE in the repository root.

#pragma once

#include "Kor/KorMinimal.h"

#include "Kor/Memory.h"
#include "Kor/QueueAllocator.h"

KOR_NAMESPACE_BEGIN

// TODO: Introduce iterator
// TODO: Consider returning memory and allow changing values!
template<typename ElementT, typename AllocatorT>
class TQueue
{
public:
	// Types
	/////////////////////////////////

	typedef AllocatorT AllocatorType;
	typedef typename AllocatorT::NodeType AllocatorNodeType;
	typedef typename AllocatorT::SizeType SizeType;

	// Constructors
	/////////////////////////////////

	KOR_FORCEINLINE TQueue() = default;
	KOR_FORCEINLINE TQueue(const TQueue& other) { CopyFrom(other); }
	KOR_FORCEINLINE TQueue(TQueue&& other) { MoveFrom(Move(other)); }

	// Destructor
	/////////////////////////////////

	KOR_FORCEINLINE ~TQueue() { EmptyImpl(); }

	// Operators
	/////////////////////////////////
	KOR_FORCEINLINE bool operator==(const TQueue& other) const { return _allocator == other._allocator; }

	KOR_FORCEINLINE bool operator!=(const TQueue& other) const { return _allocator != other._allocator; }

	KOR_FORCEINLINE TQueue& operator=(const TQueue& other) { CopyFrom(other); return *this; }
	KOR_FORCEINLINE TQueue& operator=(TQueue&& other) { MoveFrom(Move(other)); return *this; }

	// Getters
	/////////////////////////////////

	KOR_FORCEINLINE bool IsEmpty() const { return !_allocator.GetHead(); }
	KOR_FORCEINLINE SizeType GetNum() const { return _allocator.GetSize(); }

	// Peek
	/////////////////////////////////

	KOR_FORCEINLINE bool Peek(ElementT& outVal) const { return PeekImpl(outVal); }
	KOR_FORCEINLINE ElementT Peek_GetCopy() const { ElementT result = ElementT(); PeekImpl(result); return result; }

	// Enqueue
	/////////////////////////////////

	KOR_FORCEINLINE void Enqueue(const ElementT& val) { AddImpl(val); }
	KOR_FORCEINLINE void Enqueue(ElementT&& val) { AddImpl(Move(val)); }

	// Dequeue
	/////////////////////////////////

	KOR_FORCEINLINE bool Dequeue() { return RemoveFromHeadImpl(); }
	KOR_FORCEINLINE bool Dequeue(ElementT& outVal) { return RemoveFromHeadImpl(outVal); }
	KOR_FORCEINLINE ElementT Dequeue_GetCopy() { ElementT result = ElementT(); Dequeue(result); return result; }

	// Empty
	/////////////////////////////////

	KOR_FORCEINLINE void Empty() { EmptyImpl(); }
	KOR_FORCEINLINE void Reset() { EmptyImpl(); }

private:

	bool PeekImpl(ElementT& outVal) const
	{
		AllocatorNodeType* node = _allocator.GetHead();
		if(node)
		{
			SMemory::CopyTyped(&outVal, &node->Value);
		}

		return node != nullptr;
	}

	AllocatorNodeType* AddImpl(const ElementT& val)
	{
		AllocatorNodeType* node = _allocator.Allocate(1);
		SMemory::MoveTyped(&node->Value, &val);
		return node;
	}

	AllocatorNodeType* AddImpl(ElementT&& val)
	{
		AllocatorNodeType* node = _allocator.Allocate(1);
		SMemory::MoveTyped(&node->Value, &val);
		return node;
	}

	bool RemoveFromHeadImpl()
	{
		AllocatorNodeType* node = _allocator.GetHead();
		if(!node)
		{
			return false;
		}

		SMemory::Destruct(&node->Value);
		_allocator.Deallocate(node);

		return true;
	}

	bool RemoveFromHeadImpl(ElementT& outVal)
	{
		AllocatorNodeType* node = _allocator.GetHead();
		if(!node)
		{
			return false;
		}

		SMemory::MoveTyped(&outVal, &node->Value);
		_allocator.Deallocate(node);

		return true;
	}

	void EmptyImpl()
	{
		AllocatorNodeType* currentNode = _allocator.GetHead();
		if(currentNode)
		{
			while(currentNode != nullptr)
			{
				SMemory::Destruct(&currentNode->Value);
				currentNode = currentNode->Next;
			}

			_allocator.Release();
		}
	}

	void CopyFrom(const TQueue& other)
	{
		EmptyImpl();

		AllocatorNodeType* currentNode = other._allocator.GetHead();
		while(currentNode != nullptr)
		{
			AllocatorNodeType* newNode = _allocator.Allocate(1);
			SMemory::CopyTyped(&newNode->Value, &currentNode->Value);
		}
	}

	void MoveFrom(TQueue&& other)
	{
		EmptyImpl();

		_allocator.SetHead(other._allocator.GetHead());
		_allocator.SetTail(other._allocator.GetTail());

		other._allocator.SetHead(nullptr);
		other._allocator.SetTail(nullptr);
	}

	AllocatorType _allocator = {};
};

template<typename ElementT, typename AllocatorT>
struct TContainerTypeTraits<TQueue<ElementT, AllocatorT>> : public TContainerTypeTraits<void>
{
	using ElementType = ElementT;
	using AllocatorType = AllocatorT;

	enum
	{
		IsContainer = true,
		IsDynamic = true
	};
};

KOR_NAMESPACE_END