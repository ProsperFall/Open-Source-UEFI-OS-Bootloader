int AcpiTableSignIs(char* ptr1,const char* Sign)
{
    for(int n =0;n<4;n++,ptr1++,Sign++)
    {
        if(*ptr1 != *Sign)    return 0;
    }
    return 1;
}
