# include <cstdio>
# include <cstring>
#include <iostream>
#include <ostream>
#include <strings.h>
# include <unordered_map>

int main() {
    int total = 0;
    while (true) {
        char ip1[100], ip2[100], ip3[100];
        scanf("%s\n%s\n%s", ip1, ip2, ip3);
        std::unordered_map<char, int> m;
        if (ip1[0] == 'e' && ip1[1] == '\0') {
            break;
        }

        for (int i = 0; ip1[i] != '\0'; i++) {
            m[ip1[i]] = 1;
        }

        for (int i = 0; ip2[i] != '\0'; i++) {
            if (m[ip2[i]] == 1) {
                m[ip2[i]] = 2;
            }
        }

        int priority = 0;
        for (int i = 0; ip3[i] != '\0'; i++) {
            if (m[ip3[i]] == 2) {
                // found char
                if (ip3[i] >= 'a') {
                    priority = ip3[i] - 'a' + 1;
                } else {
                    priority = ip3[i] - 'A' + 27;
                }
                total += priority;

                // std::cout << std::endl << priority << "  " << ip[i] << std::endl;

                break;
            }
        }

        /* int priority = 0;
        for (int i = len/2; i < len; i++) {
            if (m.find(ip[i]) != m.end()) {
                // found char
                if (ip[i] >= 'a') {
                    priority = ip[i] - 'a' + 1;
                } else {
                    priority = ip[i] - 'A' + 27;
                }
                total += priority;

                // std::cout << std::endl << priority << "  " << ip[i] << std::endl;

                break;
            }
        }*/ 
    }

    std::cout << total << std::endl;
}