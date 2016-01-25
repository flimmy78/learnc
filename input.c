#include <stdio.h>
int main(void)
{
   int sum = 0, i = 0;
   int temp = 0;
   char input[5];
   while (1) {
      scanf("%s", input);
      for (i = 0, temp = 0; input[i] != '\0'; i++) {
          if (input[i]<'0' || input[i]>'9')
              continue;
    	 temp = temp * 10 + input[i] - '0';
      }
      sum += temp;
      printf("input=%d\n", sum);
   }
   return 0;
}
