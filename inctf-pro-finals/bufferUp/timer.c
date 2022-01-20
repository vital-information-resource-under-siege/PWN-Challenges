#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
int main()
{
  unsigned long wwe = time((time_t * )(0x0));	
  srand((unsigned int)((wwe >> 8 & 0xffffff) << 8));
  unsigned int uVar1;
  unsigned long uVar2;
  double dVar3;
  double local_38;
  int local_1c;
  int local_18;
  int local_14;
  unsigned long local_10;
  
  local_14 = 0;
  local_18 = 0;
  uVar1 = rand();
  uVar2 = (unsigned long)(int)uVar1;
  for (local_10 = uVar2; local_1c = local_18, local_10 != 0; local_10 = local_10 / 10) {
    local_18 = local_18 + 1;
  }
  while (local_1c = local_1c + -1, -1 < local_1c) {
    if ((long)uVar2 < 0) {
      local_38 = (double)(uVar2 >> 1 | (unsigned long)(uVar1 & 1));
      local_38 = local_38 + local_38;
    }
    else {
      local_38 = (double)uVar2;
    }
    dVar3 = pow(10.0,(double)local_1c);
    local_14 = local_14 + (int)(local_38 / dVar3) % 10;
  }
  printf("%d\n",local_14);
  local_14 = 0;
  local_18 = 0;
  uVar1 = rand();
  uVar2 = (unsigned long)(int)uVar1;
  for (local_10 = uVar2; local_1c = local_18, local_10 != 0; local_10 = local_10 / 10) {
    local_18 = local_18 + 1;
  }
  while (local_1c = local_1c + -1, -1 < local_1c) {
    if ((long)uVar2 < 0) {
      local_38 = (double)(uVar2 >> 1 | (unsigned long)(uVar1 & 1));
      local_38 = local_38 + local_38;
    }
    else {
      local_38 = (double)uVar2;
    }
    dVar3 = pow(10.0,(double)local_1c);
    local_14 = local_14 + (int)(local_38 / dVar3) % 10;
  }
  printf("%d\n",local_14);
}
