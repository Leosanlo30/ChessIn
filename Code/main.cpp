#include <bits/stdc++.h>

using namespace std;

 string execStockfish(const char* cmd) {
    array<char, 128> buffer;
    string result;
    
    // Abre una "tubería" (pipe) para leer la salida del comando
    unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);

    if (!pipe) {
        throw runtime_error("falla al abrir pipe");
    }

    // Lee lo que el motor responde línea por línea
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}