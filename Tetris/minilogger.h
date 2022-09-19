

#ifndef MINILOGGER_H
#define MINILOGGER_H

#include <cstdio>


class MiniLogger
{

private:

    FILE *file;


public:

    MiniLogger(const char *filename)
    {
        file = fopen(filename, "w");

    }

    virtual ~MiniLogger()
    {
        fclose(file);
    }


    void write(const char *data)
    {
        fprintf(file, "%s\n", data);
    }

};






#endif // MINILOGGER_H
