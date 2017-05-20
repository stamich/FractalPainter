//
// Created by michal on 22.04.17.
//

//#ifndef FRACTALS_GENERATORS_H
//#define FRACTALS_GENERATORS_H


#ifndef uint
typedef unsigned int uint;
#endif

#ifndef __GENRATORS
#define __GENRATORS

/*
	Kernel for each Generator-class
*/

class Kernel
{
public:
    Kernel();
    Kernel(int);
    Kernel(const Kernel&);

    Kernel& operator = (const Kernel&);
    uint& operator [] (int);
    int Dim(void) const;

    ~Kernel();

private:
    uint *_kernel;
    int _dim;
};

/*
	Base class for generators
*/

class Generator
{
public:
    virtual void Init(Kernel&)=0;
    virtual double Value(void)=0;
    virtual const char* Name(void)=0;
    virtual Kernel InitKernel(void);
    virtual ~Generator() {}
protected:
    Kernel _init;
};


class UGenerator : public Generator
{
public:
    UGenerator();

    void Init(Kernel&);
    double Value(void);
    Kernel InitKernel(void);
    const char* Name(void);

    ~UGenerator();

private:
    double uu[97];
    int ip, jp;
    double cc, cd, cm;
};

class TGenerator : public Generator
{
public:
    TGenerator();

    void Init(Kernel&);
    double Value(void);
    Kernel InitKernel(void);
    const char* Name(void);

    ~TGenerator();

private:
    Kernel _work;
};

class LocalGenerator : public Generator
{
public:
    LocalGenerator();

    void Init(Kernel&);
    double Value(void);
    Kernel InitKernel(void);
    const char* Name(void);

    ~LocalGenerator();

private:
    uint _old;
};

class R3Generator : public Generator
{
public:
    R3Generator();

    void Init(Kernel&);
    double Value(void);
    Kernel InitKernel(void);
    const char* Name(void);

    ~R3Generator();

private:
    int inext,inextp;
    long ma[56];
    int iff;
    int _pre0;
    int *idum;
    int MBIG;
    double MSEED;
    double MZ;
    double FAC;
};

#endif

