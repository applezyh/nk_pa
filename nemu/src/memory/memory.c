#include "nemu.h"
#include "device/mmio.h"
#include "memory/mmu.h"
#define PMEM_SIZE (256 * 1024 * 1024)

#define PTE_ADDR(pte) ((uint32_t(pte)) & ~0xfff)

#define PDX(va) (((uint32_t)(va) >> 22) & 0x3ff)
#define PTX(va) (((uint32_t)(va) >> 12) & 0x3ff)
#define OFF(va) ((uint32_t(va)) & 0xfff)

#define pmem_rw(addr, type) *(type *)({\
    Assert(addr < PMEM_SIZE, "physical address(0x%08x) is out of bound at %x", addr, cpu.eip); \
    guest_to_host(addr); \
    })

uint8_t pmem[PMEM_SIZE];

/* Memory accessing interfaces */

uint32_t paddr_read(paddr_t addr, int len) {
  int NO = is_mmio(addr);
  return NO==-1?pmem_rw(addr, uint32_t) & (~0u >> ((4 - len) << 3)):mmio_read(addr,len,NO);
}

void paddr_write(paddr_t addr, int len, uint32_t data) {
  int NO = is_mmio(addr);
  if(NO==-1) memcpy(guest_to_host(addr), &data, len);
  else mmio_write(addr,len,data,NO);
}

uint32_t page_translate(vaddr_t addr, bool iswrite) {
  PDE pde, *pgdir;
  PTE pte, *pgtab;
  paddr_t paddr = addr;
  CR0 cr0 = *((CR0*)&cpu.CR0);
  CR3 cr3 = *((CR3*)&cpu.CR3);
  if (cr0.protect_enable && cr0.paging) {
    pgdir = (PDE *)(intptr_t)(cr3.page_directory_base << 12);
    pde.val = paddr_read((intptr_t)&pgdir[(addr >> 22) & 0x3ff], 4);
    assert(pde.present);
    pde.accessed = 1;

    pgtab = (PTE *)(intptr_t)(pde.page_frame << 12);
    pte.val = paddr_read((intptr_t)&pgtab[(addr >> 12) & 0x3ff], 4);
    assert(pte.present);
    pte.accessed = 1;
    pte.dirty = iswrite ? 1 : pte.dirty;

    paddr = (pte.page_frame << 12) | (addr & PAGE_MASK);
  }

  return paddr;
}

uint32_t vaddr_read(vaddr_t addr, int len) {
	if (((addr & 0xfff) + len) > 0x1000) {
		int point;
		paddr_t paddr, low, high;
		// calculate the split point
		point = (int)((addr & 0xfff) + len - 0x1000);
		// get the low address
		paddr = page_translate(addr, false);
		low = paddr_read(paddr, len - point);
		// get the low address
		paddr = page_translate(addr + len - point, false);
		high = paddr_read(paddr, point);
		paddr = (high << ((len - point) << 3)) + low;
		return paddr;
	}
	else {
		
		paddr_t paddr = page_translate(addr, false);
		uint32_t instr = paddr_read(paddr, len);
		return instr;
	}
}

void vaddr_write(vaddr_t addr, int len, uint32_t data) {
	//Log("addr = %x, len = %d", addr, len);
	if (((addr & 0xfff) + len) > 0x1000) {
		int point;
		uint32_t low, high;
		paddr_t paddr;
		// calculate the split point 
		point = (int)((addr & 0xfff) + len - 0x1000);
		// split the date into the high and low
		low = (data << (point << 3)) >> (point << 3);
		high = data >> ((len - point) << 3);
	
		//Log("addr = %x, high = %x, low = %x, point = %d", addr, high, low, point);
		// store the low data
		paddr = page_translate(addr, true);
		paddr_write(paddr, len - point, low);
		// store the high data
		paddr = page_translate(addr + len - point, true);
		paddr_write(paddr, point, high);
	}
	else {
		paddr_t paddr = page_translate(addr, true);
		paddr_write(paddr, len, data);
	}
}
