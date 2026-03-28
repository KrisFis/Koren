// Copyright Alternity Arts. All Rights Reserved

#pragma once

#include "ASTDMinimal.h"

#include "ASTD/Memory.h"

template<typename ElementT>
class TQueueAllocator
{
public:

	// Types
	/////////////////////////////////

	typedef ElementT ElementType;
	typedef uint32 SizeType;

	struct SNode
	{
		ElementType Value;

		SNode* Next;
		SNode* Previous;
	};

	typedef SNode NodeType;

	// Constructor
	/////////////////////////////////

	FORCEINLINE TQueueAllocator() = default;

	// Operators
	/////////////////////////////////

	FORCEINLINE bool operator==(const TQueueAllocator& other) const { return _head == other._head && _tail == other._tail && _size == other._size; }
	FORCEINLINE bool operator!=(const TQueueAllocator& other) const { return !operator==(other); }

	// Getters
	/////////////////////////////////

	FORCEINLINE NodeType* GetHead() const { return _head; }
	FORCEINLINE void SetHead(NodeType* node) { _head = node; }

	FORCEINLINE NodeType* GetTail() const { return _tail; }
	FORCEINLINE void SetTail(NodeType* node) { _tail = node; }

	FORCEINLINE SizeType GetSize() const { return _size; }
	FORCEINLINE void SetSize(SizeType size) { _size = size; }

	// Methods
	/////////////////////////////////

	NodeType* Allocate(SizeType num)
	{
		NodeType* firstNode = nullptr;
		NodeType* prevNode = _tail;
		for(SizeType i = 0; i < num; ++i)
		{
			NodeType* newNode = SMemory::MallocTyped<NodeType>();
			newNode->Previous = prevNode;
			newNode->Next = nullptr;

			if(prevNode)
				prevNode->Next = newNode;

			if(!firstNode)
				firstNode = newNode;

			prevNode = newNode;
		}

		_tail = prevNode;
		if(!_head) { _head = firstNode; }
		_size += num;

		return firstNode;
	}

	void Deallocate(NodeType* node)
	{
		// Link previous node with next and vice versa
		{
			if(node != _head)
			{
				node->Previous->Next = node->Next;
			}
			else
			{
				_head = node->Next;
			}

			if(node != _tail)
			{
				node->Next->Previous = node->Previous;
			}
			else
			{
				_tail = node->Previous;
			}
		}

		SMemory::Free(node);
		--_size;
	}

	void Release()
	{
		NodeType* currentNode = _head;
		while(currentNode != nullptr)
		{
			NodeType* nextNode = currentNode->Next;
			SMemory::Free(currentNode);
			currentNode = nextNode;
		}

		_head = nullptr;
		_tail = nullptr;
		_size = 0;
	}

private:

	NodeType* _head = nullptr;
	NodeType* _tail = nullptr;
	SizeType _size = 0;
};
