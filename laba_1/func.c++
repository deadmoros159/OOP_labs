#include "header_test.h"


string censored(const string &str){
    string rez;
    const string glasnie = "aeiouyAEIOUYаеёиоуыэюяАЕЁИОУЫЭЮЯ";
    for (char c : str)
        if (glasnie.find(c) == -1) rez += c;
    return rez;
}
