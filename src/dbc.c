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

#include <dbc.h>
#include <endpoint.h>
#include <erst.h>
#include <stdio.h>
#include <string.h>
#include <sys.h>
#include <trb.h>
#include <xhc.h>

#define __cachealign __attribute__((aligned(64)))

static struct dbc g_dbc __cachealign;
static struct dbc_ctx g_ctx __cachealign;
static struct erst_segment g_erst[NR_SEGS] __cachealign;

static struct trb g_ering[TRB_PER_SEG];
static struct trb g_oring[TRB_PER_SEG];
static struct trb g_iring[TRB_PER_SEG];

static inline void* dbc_alloc(unsigned long long size)
{
    return sys_alloc_aligned(64, size);
}

static inline void dbc_free(void *ptr)
{
    sys_free(ptr);
}

void dbc_dump_regs(struct dbc_reg *reg)
{
    printf("DbC registers:\n");

    printf("id: 0x%x\n", reg->id);
    printf("db: 0x%x\n", reg->db);
    printf("erstsz: 0x%x\n", reg->erstsz);
    printf("erstba: 0x%llx\n", reg->erstba);
    printf("erdp: 0x%llx\n", reg->erdp);
    printf("ctrl: 0x%x\n", reg->ctrl);
    printf("st: 0x%x\n", reg->st);
    printf("portsc: 0x%x\n", reg->portsc);
    printf("cp: 0x%llx\n", reg->cp);
    printf("ddi1: 0x%x\n", reg->ddi1);
    printf("ddi2: 0x%x\n", reg->ddi2);
}

/* See section 7.6.4.1 for explanation of the initialization sequence */
int dbc_init(void)
{
    memset(&g_dbc, 0, sizeof(g_dbc));

    /* Registers */
    struct dbc_reg *reg = xhc_find_dbc_base();
    if (!reg) {
        return 0;
    }

    int erstmax = (reg->id & 0x1F0000) >> 16;
    if (NR_SEGS > (1 << erstmax)) {
        return 0;
    }

    g_dbc.regs = reg;

    /* Event ring */
    memset(&g_ering, 0, sizeof(g_ering));
    g_dbc.ering = g_ering;

    /* Event ring segment table */
    memset(&g_erst, 0, sizeof(g_erst));
    unsigned long long base = sys_virt_to_phys(g_ering);
    if (!base) {
        return 0;
    }

    g_erst[0].base = base;
    g_erst[0].nr_trb = TRB_PER_SEG;
    g_dbc.erst = g_erst;

    /* Endpoint context */
    memset(&g_ctx, 0, sizeof(g_ctx));

    unsigned int max_burst = (reg->ctrl & 0xFF0000) >> 16;
    unsigned long long out = sys_virt_to_phys(g_oring);
    unsigned long long in = sys_virt_to_phys(g_iring);

    init_endpoint(g_ctx.ep_out, max_burst, bulk_out, out);
    init_endpoint(g_ctx.ep_in, max_burst, bulk_in, in);

    g_dbc.ctx = &g_ctx;
    g_dbc.oring = g_oring;
    g_dbc.iring = g_iring;

    /* Hardware registers */
    reg->erstsz = NR_SEGS;
    reg->erstba = sys_virt_to_phys(g_erst);
    reg->erdp = base;
    reg->cp = sys_virt_to_phys(&g_ctx);

    /* Geeeet it on */
    //reg->ctrl |= (1UL << 31);

    return 1;
}
