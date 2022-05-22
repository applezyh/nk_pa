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
  CR0 cr0 = *((CR0*)&cpu.CR0);
	if (cr0.paging && cr0.protect_enable) {
		paddr_t pde_base = cpu.CR3;
		paddr_t pde_address = pde_base + ((addr >> 22) << 2);
		paddr_t pde = paddr_read(pde_address, 4);
		if (!(pde & 0x1)) {
      Log("CR0: %x", cpu.CR0);
			Log("addr = 0x%x, iswrite = %d", addr, iswrite);
			Log("pde = 0x%x, pde_base = 0x%x, pde_address = 0x%x", pde, pde_base, pde_address);
			assert(0);
		}

		paddr_t pte_base = pde & 0xfffff000;
		//paddr_t pte_address = pte_base + (((addr & 0x003ff000) >> 12) << 2);
		paddr_t pte_address = pte_base + ((addr & 0x003ff000) >> 10);
		paddr_t pte = paddr_read(pte_address, 4);
		if (!(pte & 0x1)) {
			Log("addr = 0x%x, iswrite = %d", addr, iswrite);
			Log("pte = 0x%x", pte);
			assert(0);
		}
		paddr_t page_address = (pte & 0xfffff000) + (addr & 0xfff);
		
		// set the access and dirty
		pde = pde | 0x20;
		pte = pte | 0x20;
		if (iswrite) {
			pde = pde | 0x40;
			pte = pte | 0x40;
		}
		paddr_write(pde_address, 4, pde);
		paddr_write(pte_address, 4, pte);
	
		return page_address;
	}
	else {
	  printf("%x %x\n", addr, cpu.eip);
	  return addr;
	}
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
		return paddr_read(paddr, len);
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
