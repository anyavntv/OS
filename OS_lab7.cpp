#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

int f(int x)
{
    if (x == 1)
        while(1){
            sleep(8);
    }
    return -x - 1;
}

int g(int x)
{
    return x - 2;
    

}

int main()
{
    int fd1[2];
    int fd2[2];
    if (pipe(fd1) == -1)
        return 1;
    if (pipe(fd2) == -1)
        return 2;
    int id1 = -2, id2 = -2;
    if ((id1 = fork()) < 0)
        return 3;
    if (id1 != 0 && (id2 = fork()) < 0)
    {
        kill(id1, SIGKILL);
        return 4;
    }

    if (id1 == 0 && id2 == -2)
    { // child1
        usleep(30000);
        int x;
        read(fd1[0], &x, sizeof(int));
        close(fd1[0]);
        int resultF = f(x);
        write(fd1[1], &resultF, sizeof(resultF));
        close(fd1[1]);
    }
    else if (id1 != 0 && id2 != 0)
    { // parent
        kill(id1, SIGSTOP);
        kill(id2, SIGSTOP);
        int x;
        std::cout << "Input a number: ";
        std::cin >> x;
        write(fd1[1], &x, sizeof(x));
        write(fd2[1], &x, sizeof(x));
        close(fd1[1]);
        close(fd2[1]);
        kill(id1, SIGCONT);
        kill(id2, SIGCONT);
        int sec = 0;
        bool finished[2] = {0, 0};
        int fx, gx;
        while (1)
        {
            if (waitpid(id1, NULL, WNOHANG) == id1)
            {
                finished[0] = 1;
                read(fd1[0], &fx, sizeof(fx));
                close(fd1[0]);
                if (fx == 0)
                {
                    kill(id1, SIGKILL);
                    kill(id2, SIGKILL);
                    close(fd2[0]);
                    std::cout << "first process returned, so f(x) * g(x) = 0\n";
                    return 0;
                }
            }
            if (waitpid(id2, NULL, WNOHANG) == id2)
            {
                finished[1] = 1;
                read(fd2[0], &gx, sizeof(gx));
                close(fd2[0]);
                if (gx == 0)
                {
                    kill(id1, SIGKILL);
                    kill(id2, SIGKILL);
                    close(fd1[0]);
                    std::cout << "second process returned 0, so f(x) * g(x) = 0\n";
                    return 0;
                }
            }
            if (finished[0] == 1 && finished[1] == 1)
                break;
            sleep(1);
            ++sec;
            if (sec % 10 == 0)
            {
                do
                {
                    char temp;
                    std::cout << "Should the child processes be killed? Type y or n\n";
                    std::cin >> temp;
                    if (temp == 'y' || temp == 'Y')
                    {
                        kill(id1, SIGKILL);
                        kill(id2, SIGKILL);
                        return 21;
                    }
                    else if (temp == 'n' || temp == 'N')
                        break;
                } while (1);
            }
        }

        std::cout << "f(x) * g(x) = " << fx << " * " << gx << " = " << (fx * gx) << '\n';
    }
    else if (id2 == 0)
    { // child2
        usleep(30000);
        int x;
        read(fd2[0], &x, sizeof(int));
        close(fd2[0]);
        int resultG = g(x);
        write(fd2[1], &resultG, sizeof(resultG));
        close(fd2[1]);
    }
}