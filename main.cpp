#include <iostream>
#include "logs.h"
#include "explorer.h"
#include "archiver.h"

int
main(int argc, char** argv){
    std::cout << "Hello navtor" << std::endl;
    debug_print("Hello from debugprint");

    uint32_t x = 0;
    auto explorer = new Explorer();
    explorer->GetCurrentLocationContent();
    explorer->PrintCurrentLocationContent();
    std::cin >> x;
    
//    explorer->MoveToLocation(explorer->getLocationItembyId(x));
//    explorer->PrintCurrentLocationContent();

    auto archive = new Archiver("test.tar.gz");
    archive->AddDirectory(explorer->getLocationItembyId(x));
    delete archive;

    delete explorer;

    return 1;
}
