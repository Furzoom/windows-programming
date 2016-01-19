#include <stdio.h>

int g_nNum;

int main(int argc, char* argv[])
{
    int i = 198;
    g_nNum = 1003;
    while (1)
    {
        printf("i = %d, addr = %08lx;\tg_nNum = %d, addr = %08lx\n",++i, &i, --g_nNum, &g_nNum);
        getchar();
    }
    return 0;
}