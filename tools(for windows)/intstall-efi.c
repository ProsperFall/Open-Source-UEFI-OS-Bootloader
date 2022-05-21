#include <stdio.h>

int main(int argc,char*args[])
{
    system("disk-attach.bat");
    printf("Install right now? Yes or No (y/n)\n");
    int cha;
Agin:
    cha = getchar();
    if( cha == 'y' || cha == 'Y')
    {
        printf("Installing\n");
        system("install.bat");
    }
    else if( cha == 'n' || cha == 'N')
    {
        printf("Operate has been Canceled\n");
    }
    else
    {
        printf("Unknow key,Press Again!");
        getchar();
        getchar();
        getchar();
        getchar();//fk windows stdin
        goto Agin;
    }
    getchar();//fk windows stdin
    if( argc>1 && args[1][0]=='p')
    {
        printf("\nPause.\n");//
        cha = getchar();
    }
    system("disk-detach.bat");
    return 0;
}