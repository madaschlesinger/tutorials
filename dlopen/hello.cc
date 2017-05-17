#include <iostream>

// a few ways to do this - read that this is the most portable....
// but not confirmed - init/fini where used on some platforms w/o the
// attribute delcarations

static void init() __attribute__((constructor));
static void fini() __attribute__((destructor));

void init() {
    printf("I'm a constructor - libary loader\n");
}


extern "C" void hello() {
    std::cout << "hello" << '\n';
}


void fini() {
    printf("I'm a lib destrtor - libary unloader\n");
}
