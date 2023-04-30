#include <iostream>
#include <unistd.h>
#include <pthread.h>

constexpr int N = 4;
int x = 0;
int y = 0;
pthread_mutex_t mutex;

void* wocritical(void*) {
    for (int i = 0; i < 1000000000; ++i)
    x++;
    return NULL;
}

void* wcritical(void*){
    
    for (int i = 0; i < 1000000000; ++i) {
    
    y++;
    
    }
    
    return NULL;

}
int main()
{
    pthread_mutex_init(&mutex, NULL);
    pthread_t t[N];

    for (int i = 0; i < N; ++i)
    if(i % 2 == 0)
    pthread_create(&t[i], NULL, &wocritical, NULL);
    else
    pthread_create(&t[i], NULL, &wcritical, NULL);
    
    for (int i = 0; i < N; ++i)
    pthread_join(t[i], NULL);
    pthread_mutex_destroy(&mutex);
    std::cout << "(Without a critical section) x = " << x;
    std::cout << std::endl;
    std::cout << "(With a critical section) y = " << y;
    return 0;
}