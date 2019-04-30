//
// Copyright (C) 2019 Assured Information Security, Inc.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <stdlib.h>

/**
 * bf_map_hpa
 *
 * Map in memory at the given physical address
 *
 * @param hpa the physical address to map
 * @param size the number of bytes to map
 * @param flags the memory type flags (see XUE_MEM_* values in xue.h)
 * @return the virtual address used to access phys
 */
void *bf_map_hpa(unsigned long long hpa, unsigned int size, int flags);

/**
 * bf_virt_to_phys
 *
 * Translate the given virtual address to its corresponding physical address.
 *
 * @param virt the virtual address to translate
 * @return the resulting physical address
 */
unsigned long long bf_virt_to_phys(unsigned long long virt);

void *sys_map_hpa(unsigned long long hpa, unsigned int size, int flags)
{
    return bf_map_hpa(hpa, size, flags);
}

unsigned long long sys_virt_to_phys(unsigned long long virt)
{
    return bf_virt_to_phys(virt);
}

void *sys_alloc_aligned(unsigned long long align, unsigned long long size)
{
    return aligned_alloc(align, size);
}

void sys_free(void *ptr)
{
    free(ptr);
}
