#include <stdio.h>
#if 0
union ss {
    union s1 {
        struct k1 {
            unsigned int q1:1;
            unsigned int q2:2;
            unsigned int q3:29;
        }kk1;
        struct k2 {
            unsigned int q1:2;
            unsigned int q2:1;
            unsigned int q3:29;
        }kk2;
        struct k3 {
            unsigned int q1:3;
            unsigned int q2:3;
            unsigned int q3:26;
        }kk3;
    }ss1;
    union s2 {
        struct k1 {
            unsigned int q1:1;
            unsigned int q2:2;
            unsigned int q3:29;
        }kk1;
        struct k2 {
            unsigned int q1:2;
            unsigned int q2:1;
            unsigned int q3:29;
        }kk2;
        struct k3 {
            unsigned int q1:3;
            unsigned int q2:3;
            unsigned int q3:26;
        }kk3;
    }ss2;
};
#endif
int main()
{
    //union ss test1;
    printf("hello\n");
    //printf("test is %d\n", &(test1.ss1.kk1.q2));

    return 10;
}
