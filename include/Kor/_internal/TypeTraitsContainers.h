// Copyright Alternity Arts. All Rights Reserved

#pragma once

template<typename T>
struct TContainerTypeTraits
{
	// Internal element type
	using ElementType = void;

	// Internal allocator type
	using AllocatorType = void;

	// Flags
	enum
	{
		IsContainer = false,
		IsDynamic = false,
		InlineMemory = false
	};
};

// [Is Container]
// * Checks whether specific type is container type

template<typename T>
struct TIsContainer { enum { Value = TContainerTypeTraits<T>::IsContainer }; };

// [Is Dynamic Container]
// * Checks whether specific type is container and can do dynamic allocations

template<typename T>
struct TIsDynamicContainer
{
private:
	typedef TContainerTypeTraits<T> ContainerT;

public:
	enum { Value = ContainerT::IsContainer && ContainerT::IsDynamic };
};

// [Is Fixed Container]
// * Checks whether specific type is container and cannot do dynamic allocations

template<typename T>
struct TIsFixedContainer
{
private:
	typedef TContainerTypeTraits<T> ContainerT;

public:
	enum { Value = ContainerT::IsContainer && !ContainerT::IsDynamic };
};
