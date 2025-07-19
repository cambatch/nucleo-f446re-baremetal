
void spin(volatile long count)
{
    while(count--) (void)0;
}

int main(void)
{
    const long gpioa = 0x40020000;
    const long rcc = 0x40023800;

    const long rcc_ahb1enr = rcc + 0x30;
    const long gpioa_moder = gpioa + 0x00;
    const long gpioa_odr = gpioa + 0x14;

    // Enable gpioa
    volatile long *reg = (long*)rcc_ahb1enr;
    *reg |= (1 << 0);

    // set gpioa PA5 mode to output
    reg = (long*)gpioa_moder;
    *reg &= ~(3 << (2 * 5));
    *reg |= (1 << (2 * 5));

    reg = (long*)gpioa_odr;

    while(1)
    {
        *reg |= (1 << 5);
        spin(999999);
        *reg &= ~(1 << 5);
        spin(999999);
    }
}
