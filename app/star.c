char star(int n)
{
    char ret;

    switch (n)
    {
        case 0: ret = '|'; break;
        case 1: ret = '\\'; break;
        case 2: ret = '-'; break;
        case 3: ret = '/'; break;
    }

    return ret;
}
