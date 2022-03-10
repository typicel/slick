
void INThandler(int);

void signalManager(){
    signal(SIGINT, INThandler);
}

//Handler for interupts
void INThandler(int sig){
    char c;
    signal(sig, SIG_IGN); //ignore signal first
    printf("\nAre you sure you want to quit? [y/n]: ");
    c = getchar();

    if(c == 'y' || c == 'Y')
        exit(0); 
    else {
        signal(SIGINT, INThandler); //reinstall handler
        
    }

}
