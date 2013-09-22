#include <system.h>

struct regs last_interrupt_regs;

uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void outb(uint16_t port, uint8_t val)
{
    asm volatile("outb %1, %0" : : "dN"(port), "a"(val));
}

void io_wait(void)
{
    asm volatile("jmp 1f\n\t" "1:jmp 2f\n\t" "2:");
}

void cpuid(int code, uint32_t *a, uint32_t *d)
{
    asm volatile("cpuid" : "=a"(*a), "=d"(*d) : "0"(code) : "ebx", "ecx");
}

void wrmsr(uint32_t msr_id, uint64_t msr_value)
{
    asm volatile("wrmsr" : : "c" (msr_id), "A" (msr_value));
}

void *memset(void *s, int c, size_t n)
{
    uint8_t *p = s;
    while(n--)
        *p++ = (uint8_t) c;
    return s;
}

void *memcpy(void *dest, const void *src, size_t count) {
    char* dst8 = (char*) dest;
    char* src8 = (char*) src;

    while (count--) {
        *dst8++ = *src8++;
    }
    return dest;
}

int strlen(char *c)
{
    int res;
    while(c[res++]);
    return res;
}

char *convert(int num, unsigned int base)
{
    // assert(base > 2 && base <= 16);

    static char buf[33];
    char *ptr;

    ptr = &buf[sizeof(buf) - 1];
    *ptr = '\0';
    do
    {
        *--ptr= "0123456789abcdef"[num%base];
        num /= base;
    } while(num != 0);
    
    return(ptr);
}

char *convert_u(uint64_t num, unsigned int base)
{
    // assert(base > 2 && base <= 16);

    static char buf[33];
    char *ptr;

    ptr = &buf[sizeof(buf) - 1];
    *ptr = '\0';
    do
    {
        *--ptr= "0123456789abcdef"[num%base];
        num /= base;
    } while(num != 0);
    
    return(ptr);
}

/* ABSOLUTE FAILURE
char *convert64(int64_t num)
{
    static char buf[66];
    char *i;
    unsigned int str_index, lo, hi;
    lo = (unsigned int) num & 0xFFFFFFFF;
    hi = (unsigned int) num >> 32;
    

    for(i = &buf[sizeof(buf) - 1]; i > &buf; --i)
    {
        *i = ((lo > 0 ? lo : hi) % 10) + 0x30;

        if(lo <= 0 && hi <= 0)
            break;

        num /= 10;
    }

    return i;
}
*/

void kprintf(char *format, ...)
{
    char *p;
    int i;
    uint64_t i64;
    unsigned u;
    char *s;
    va_list argp;

    va_start(argp, format);

    p = format;
    for(p = format; *p != '\0'; p++)
    {
        if(*p != '%')
        {
            putch(*p);
            continue;
        }
        p++;

        switch(*p)
        {
            case 'c':
                i = va_arg(argp, int);
                putch(i);
                break;
            case 'd': case 'i':
                i = va_arg(argp, int);

                if(i < 0)
                {
                    i = -i;
                    putch('-');
                }

                puts(convert(i, 10));
                break;
            case 'l':
                i64 = va_arg(argp, uint64_t);
                puts(convert_u(i64, 10));
                break;
            case 'o':
                i = va_arg(argp, int);
                puts(convert(i, 8));
                break;
            case 's': 
                s = va_arg(argp, char*);
                puts(s);
                break;
            case 'u': 
                u = va_arg(argp, unsigned int);
                puts(convert_u((uint64_t) u, 10));
                break;
            case 'x':
                i64 = va_arg(argp, uint64_t);
                puts(convert_u(i64, 16));
                break;
            case 'b':
                u = va_arg(argp, unsigned int);
                puts(convert(u, 2));
                break;
            case '%':
                putch('%');
                break;
        }
    }

    va_end(argp);
}

void ksprintf(char *buffer, char *format, ...)
{
    char *p;
    int i, cur_buf_index = 0;
    uint64_t i64;
    unsigned u;
    char *s;
    va_list argp;

    va_start(argp, format);

    p = format;
    for(p = format; *p != '\0'; p++)
    {
        if(*p != '%')
        {
            buffer[cur_buf_index++] = *p;
            continue;
        }
        p++;

        switch(*p)
        {
            case 'c':
            {
                i = va_arg(argp, int);
                buffer[cur_buf_index++] = i;
                break;
            }
            case 'd': case 'i':
            {
                i = va_arg(argp, int);

                if(i < 0)
                    buffer[cur_buf_index++] = '-';

                char *num = convert(i, 10);

                int i;
                for(i = 0; i < strlen(num); ++i)
                    buffer[cur_buf_index++] = num[i];

                break;
            }
            case 'l':
            {
                i64 = va_arg(argp, uint64_t);
                char *num = convert_u(i64, 10);

                int i;
                for(i = 0; i < strlen(num); ++i)
                    buffer[cur_buf_index++] = num[i];

                break;
            }
            case 'o':
            {
                i = va_arg(argp, int);
                char *num = convert(i, 8);

                int i;
                for(i = 0; i < strlen(num); ++i)
                    buffer[cur_buf_index++] = num[i];

                break;
            }
            case 's':
            {
                s = va_arg(argp, char*);
                
                int i;
                for(i = 0; i < strlen(s); ++i)
                    buffer[cur_buf_index++] = s[i];

                break;
            }
            case 'u':
            {
                u = va_arg(argp, unsigned int);
                char *num = convert_u((uint64_t) u, 10);

                int i;
                for(i = 0; i < strlen(num); ++i)
                    buffer[cur_buf_index++] = num[i];
                break;
            }
            case 'x':
            {
                u = va_arg(argp, unsigned int);
                char *num = convert_u((uint64_t) u, 16);

                int i;
                for(i = 0; i < strlen(num); ++i)
                    buffer[cur_buf_index++] = num[i];

                break;
            }
            case 'b':
            {
                u = va_arg(argp, unsigned int);
                char *num = convert_u((uint64_t) u, 2);

                int i;
                for(i = 0; i < strlen(num); ++i)
                    buffer[cur_buf_index++] = num[i];

                break;
            }
            case '%':
            {
                buffer[cur_buf_index++] = '%';
                break;
            }
        }
    }

    va_end(argp);
}

extern uint32_t _read_cr0();
extern uint32_t _read_cr1();
extern uint32_t _read_cr2();
extern uint32_t _read_cr3();

void dump_regs(struct regs *r)
{
    /*
    kprintf("eax: %u, ebx: %u\necx: %u, edx: %u\nesi: %u, edi: %u\ncr0: %u, cr1: %u, cr2: %u, cr3: %u\n",
            (uint64_t) r->eax, (uint64_t) r->ebx, (uint64_t) r->ecx, (uint64_t) r->edx,
            (uint64_t) r->esi, (uint64_t) r->edi,
            (uint64_t) _read_cr0(), (uint64_t) _read_cr1(), 
            (uint64_t) _read_cr2(), (uint64_t) _read_cr3()); /**/

    kprintf("eax: %u, ebx: %u\necx: %u, edx: %u\nesi: %u, edi: %u\n",
            (uint64_t) r->eax, (uint64_t) r->ebx, (uint64_t) r->ecx, (uint64_t) r->edx,
            (uint64_t) r->esi, (uint64_t) r->edi);
}