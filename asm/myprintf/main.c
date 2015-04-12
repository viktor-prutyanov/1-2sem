extern void myprintf(const char *, ...);

int main()
{
    myprintf("Hello %s %crom %s! %b is 0o%o%% of %d. %s 0x%x! I %c %x!",
        "world", 'f', "V.P.", 1337L, 100L, 1337L, "So much", 3802L, 3, 3802);
    return 0;
}