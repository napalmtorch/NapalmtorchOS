
int main(void* arg)
{
    asm volatile("int $0x80");
    return 0x32;
}