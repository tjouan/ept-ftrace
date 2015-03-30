#ifndef   _REG_H_
#define   _REG_H_

struct _reg {
  int     r_eax;
  int     r_ecx;
  int     r_edx;
  int     r_ebx;
  int     r_esp;
  int     r_ebp;
  int     r_esi;
  int     r_edi;
  int     r_eip;
  int     r_eflags;
  int     r_cs;
  int     r_ss;
  int     r_ds;
  int     r_es;
  int     r_fs;
  int     r_gs;
};

#endif
