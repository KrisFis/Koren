// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "ASTDMinimal.h"

#include "ASTD/Memory.h"
#include "ASTD/QueueAllocator.h"

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

	FORCEINLINE TQueue() = default;
	FORCEINLINE TQueue(const TQueue& other) { CopyFrom(other); }
	FORCEINLINE TQueue(TQueue&& other) { MoveFrom(Move(other)); }

	// Destructor
	/////////////////////////////////

	FORCEINLINE ~TQueue() { EmptyImpl(); }

	// Operators
	/////////////////////////////////
	FORCEINLINE bool operator==(const TQueue& other) const { return _allocator == other._allocator; }

	FORCEINLINE bool operator!=(const TQueue& other) const { return _allocator != other._allocator; }

	FORCEINLINE TQueue& operator=(const TQueue& other) { CopyFrom(other); return *this; }
	FORCEINLINE TQueue& operator=(TQueue&& other) { MoveFrom(Move(other)); return *this; }

	// Getters
	/////////////////////////////////

	FORCEINLINE bool IsEmpty() const { return !_allocator.GetHead(); }
	FORCEINLINE SizeType GetNum() const { return _allocator.GetSize(); }

	// Peek
	/////////////////////////////////

	FORCEINLINE bool Peek(ElementT& outVal) const { return PeekImpl(outVal); }
	FORCEINLINE ElementT Peek_GetCopy() const { ElementT result = ElementT(); PeekImpl(result); return result; }

	// Enqueue
	/////////////////////////////////

	FORCEINLINE void Enqueue(const ElementT& val) { AddImpl(val); }
	FORCEINLINE void Enqueue(ElementT&& val) { AddImpl(Move(val)); }

	// Dequeue
	/////////////////////////////////

	FORCEINLINE bool Dequeue() { return RemoveFromHeadImpl(); }
	FORCEINLINE bool Dequeue(ElementT& outVal) { return RemoveFromHeadImpl(outVal); }
	FORCEINLINE ElementT Dequeue_GetCopy() { ElementT result = ElementT(); Dequeue(result); return result; }

	// Empty
	/////////////////////////////////

	FORCEINLINE void Empty() { EmptyImpl(); }
	FORCEINLINE void Reset() { EmptyImpl(); }

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
