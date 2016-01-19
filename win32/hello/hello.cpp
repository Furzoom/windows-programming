#include <stdio.h>
#include <windows.h>

int main(int argc, char* argv[])
{
    int nSelect = ::MessageBox(NULL, TEXT("Hello, Windows"), TEXT("Hello"), MB_OKCANCEL);
    if (nSelect == IDOK)
    {
        printf("Click OK!\n");
    }
    else
    {
        printf("Click Cancel!\n");
    }

    getchar();
    return 0;
}
