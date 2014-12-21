//
//  main.cpp
//  pkg-remove
//
//  Created by Prakhar Shukla on 20/12/14.
//  Copyright (c) 2014 Prakhar Shukla. All rights reserved.
//

#include <iostream>
#include <string>
#include <stdio.h>

const char* REMOVE ="remove";
const char* LIST = "list";
const char* INFO = "info";
const char* HELP = "Try:\n\"pkg-remove list\" to list installed packages.\n ";

bool execute(const char* cmd) {
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return false;
    char buffer[256];
    while(!feof(pipe)) {
        if(fgets(buffer, 255, pipe) != NULL)
            std::cout<<buffer;
    }
    pclose(pipe);
    return true;
}

void showAll() {
    std::cout<<"\nList of packages :\n";
    if(execute("pkgutil --pkgs")) {
        std::cout<<"\nList complete.\n";
    } else {
        std::cout<<"\nInternal Error. Report to owner";
    }
}

void removePackage(const char* pkgname) {
    std::cout<<"\nRemoving package "<<pkgname;
    char command [1024] = "cd /; pkgutil --only-files --files ";
    strcat(command, pkgname);
    strcat(command, " | tr '\\n' '\\0' | xargs -n 1 -0 sudo rm -i");
    bool status = execute(command);
    strcpy(command, "cd /; pkgutil --only-dirs --files ");
    strcat(command, pkgname);
    strcat(command, " | tr '\\n' '\\0' | xargs -n 1 -0 sudo rm -i");
    if(status) status = execute(command);
    strcpy(command, "pkgutil --forget ");
    strcat(command, pkgname);
    if(status) status=execute(command);
    if(status) {
        std::cout<<"\nPackage Removed Successfully.\n";
    } else {
        std::cout<<"\nInternal Error. Report to owner";
    }
}

void showPackageInfo(const char* pkgname) {
    char command [1024]= "pkgutil --pkg-info ";
    strcat(command, pkgname);
    bool status = execute(command);
    if(!status) {
        std::cout<<"\nInternal Error. R\eport to owner";
    }
}

int main(int argc, const char * argv[]) {
    if(strcmp(argv[1],LIST)==0) {
        //print existing installed packages
        showAll();
    } else if(strcmp(argv[1], REMOVE)==0) {
        //code to uninstall packages
        removePackage(argv[2]);
    } else if(strcmp(argv[1], INFO)==0) {
        //code to package information
        showPackageInfo(argv[2]);
    } else {
        std::cout<<HELP;
    }
    return 0;
}
