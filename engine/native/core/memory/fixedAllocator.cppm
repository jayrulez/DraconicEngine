module;

#include <bit>
#include <cassert>
#include <cstddef>
#include <cstdint>

export module core.memory.fixedAllocator;
export import core.memory.allocator;
export import core.memory.slice;

export namespace draco::memory
{
	namespace fixed
	{
		struct FixedAllocator
		{
			uint8_t *buffer;
			size_t size;
			bool allocated;
		};

		void init(FixedAllocator *alloc, Slice block)
		{
			alloc->buffer = (uint8_t *)block.data;
			alloc->size = block.size;
			alloc->allocated = false;
		}

		Error alloc(
			Allocator alloc,
			Slice *dst,
			size_t size,
			size_t align
		)
		{
			FixedAllocator *allocData = (FixedAllocator*)alloc.allocatorData;
			size_t alignMask = align - 1;
			size_t alignedSize = allocData->size - (
				(align - (((uintptr_t)allocData->buffer) & alignMask))
				& alignMask
			);
			assert(std::popcount(align) == 1);
			if (allocData->allocated | (alignedSize < size))
			{
				return Error::OutOfMemory;
			}
			dst->data = (void *)(
				((uintptr_t)&(allocData->buffer[alignMask])) & ~alignMask
			);
			dst->size = alignedSize;
			allocData->allocated = true;
			return Error::Okay;
		}

		Error freeAll(Allocator alloc)
		{
			FixedAllocator *allocData = (FixedAllocator*)alloc.allocatorData;
			allocData->allocated = false;
			return Error::Okay;
		}

		AllocatorVTbl fixedAllocatorVtbl = {
			.alloc = alloc,
			.free = nilFree,
			.freeAll = freeAll,
		};

		inline void asAllocator(Allocator *dst, FixedAllocator *alloc)
		{
			asAllocatorVoid(dst, (void*)alloc, &fixedAllocatorVtbl);
		}
	}
}
