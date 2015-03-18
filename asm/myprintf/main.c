extern void _myprintf(const char *, ...);

int main()
{
    _myprintf("Hello %s %crom %s! %b is 0o%o%% of %d. %s 0x%x!", "world", 'f', "V.P.", 1337L, 100L, 1337L, "So much", 3802L);
    return 0;
}