#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "storage.h"

int main(){
    head_stor = load_stor("F:/PureC/delivery_disign/storage.txt");
    head_package = load_package("F:/PureC/delivery_disign/package.txt");
    package* p  =head_package;
    while(p!=NULL){
        place_package_to_stor(p);
        p = p -> next;
    }
    save_package("F:/PureC/delivery_disign/save_package.txt");
    save_stor("F:/PureC/delivery_disign/save_stor.txt");
    return 0;
}