#ifndef STATUS_H
#define STATUS_H

enum Status{
    Success = 0,
    CriticalError,
    CannotOpenFile,
    AccessFileFailed,
    WriteFailed,
    TooManyArgs,
    UserExit,
};

#endif