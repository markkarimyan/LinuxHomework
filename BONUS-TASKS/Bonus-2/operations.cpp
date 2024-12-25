#include <iostream>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <sstream>

struct OperationData {
    int index;
    int num1;
    int num2;
    char op;
};

void* performOperation(void* arg) {
    OperationData* data = (OperationData*)arg;
    int result = 0;
    const char* operationStr;

    switch (data->op) {
        case 's':  
            result = data->num1 + data->num2;
            operationStr = "s";
            break;
        case 'm':  
            result = data->num1 * data->num2;
            operationStr = "m";
            break;
        case 'x':  
            result = data->num1 * data->num1 + data->num2 * data->num2;
            operationStr = "ss";
            break;
        default:
            std::cerr << "Invalid operation" << std::endl;
            return NULL;
    }

    char filename[16];
    snprintf(filename, sizeof(filename), "out_%d", data->index);

    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return NULL;
    }

    char resultStr[128];
    snprintf(resultStr, sizeof(resultStr), "%d %s %d = %d\n", data->num1, operationStr, data->num2, result);

    write(fd, resultStr, strlen(resultStr));
    close(fd);

    return NULL;
}

int main() {
    int N;
    std::cin >> N;
    std::cin.ignore();

    pthread_t threads[N];
    OperationData data[N];

    for (int i = 0; i < N; ++i) {
        std::string line;
        std::getline(std::cin, line);
        std::stringstream ss(line);
        ss >> data[i].num1 >> data[i].num2 >> data[i].op;
        data[i].index = i + 1;

        if (pthread_create(&threads[i], NULL, performOperation, (void*)&data[i]) != 0) {
            std::cerr << "Error creating thread " << i + 1 << std::endl;
            return 1;
        }
    }

    for (int i = 0; i < N; ++i) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
