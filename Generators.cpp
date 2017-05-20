//
// Created by michal on 22.04.17.
//

#include "Generators.h"

#ifndef rand
#include <stdlib.h>
#endif

#ifndef memcpy
#include <string.h>
#endif

#ifndef fmod
#include <cmath>
#endif

/*
	Kernel class members
*/

Kernel::Kernel()
        : _dim(0), _kernel(0)
{
}

Kernel::Kernel(int dim)
        : _dim(dim), _kernel(new uint[dim])
{
}

Kernel::Kernel(const Kernel& other)
        : _dim(other.Dim()), _kernel(new uint[other.Dim()])
{
    memcpy(_kernel, other._kernel, sizeof(int)*_dim);
}

Kernel& Kernel::operator = (const Kernel& other)
{
    if (_kernel!=0)
        delete []_kernel;
    _dim=other.Dim();
    _kernel=new uint[_dim];
    memcpy(_kernel, other._kernel, sizeof(uint)*_dim);
    return(*this);
}

uint& Kernel::operator [] (int i)
{
    return(_kernel[i]);
}

int Kernel::Dim(void) const
{
    return(_dim);
}

Kernel::~Kernel()
{
    if (_kernel!=0)
        delete []_kernel;
}


/*
	Members of Generator class
*/

// ---------------------------------------------------------------------------

Kernel Generator::InitKernel(void)
{
    return(_init);
}

// ===========================================================================

/*
	Members of UGenerator class
*/

//  --------------------------------------------------------------------------

UGenerator::UGenerator()
{
}

void UGenerator::Init(Kernel& init)
{
    int wi=init[0], wj=init[1], wk=init[2], wl=init[3];
    int ii, jj, m;
    double s, t;
    _init=init;
    ip=97;
    jp=33;
    cc=362436.0/16777216.0;
    cd=7654321.0/16777216.0;
    cm=16777213.0/16777216.0;
    for (ii=0; ii<97; ii++)
    {
        s=0.0;
        t=0.5;
        for (jj=1; jj<=24; jj++)
        {
            m=(((wi*wj)%179)*wk)%179;
            wi=wj;
            wj=wk;
            wk=m;
            wl=(53*wl+1)%169;
            if ((wl*m)%64>=32)
                s+=t;
            t*=0.5;
        }
        uu[ii]=s;
    }
}

double UGenerator::Value(void)
{
    double pom;
    pom=uu[ip-1]-uu[jp-1];
    if (pom<0.0)
        pom+=1.0;
    uu[ip-1]=pom;
    ip--;
    if (ip==0)
        ip=97;
    jp--;
    if (jp==0)
        jp=97;
    cc-=cd;
    if (cc<0.0)
        cc+=cm;
    pom-=cc;
    if (pom<0.0)
        pom+=1.0;
    return(pom);
}

Kernel UGenerator::InitKernel(void)
{
    return(_init);
}


const char* UGenerator::Name(void)
{
    return("Universal generator");
}

UGenerator::~UGenerator()
{
}

// ===========================================================================

/*
	Members of TGenerator
	(Tezuka generator)
*/

TGenerator::TGenerator()
{}

void TGenerator::Init(Kernel& init)
{
    uint b=0, c=0;
    _init=init;
    _work=init;
    b=((_work[0]<<9)^_work[0])<<4;
    c=(_work[0]<<4)^(b>>28);
    _work[0]=c;

    b=((_work[1]<<2)^_work[1])<<3;
    c=(_work[1]<<3)^(b>>29);
    _work[1]=c;

    b=((_work[2]<<6)^_work[2])<<1;
    c=(_work[2]<<1)^(b>>31);
    _work[2]=c;
}

double TGenerator::Value(void)
{
    uint b=0, c=0;
    b=((_work[0]<<9)^_work[0])<<4;		c=(_work[0]<<13)^(b>>19);	_work[0]=c;
    b=((_work[1]<<2)^_work[1])<<3;		c=(_work[1]<<20)^(b>>12);	_work[1]=c;
    b=((_work[2]<<6)^_work[2])<<1;		c=(_work[2]<<17)^(b>>15);	_work[2]=c;
    return((_work[0]^_work[1]^_work[2])*2.3283064365e-10);
}

Kernel TGenerator::InitKernel(void)
{
    return(_init);
}

const char* TGenerator::Name(void)
{
    return("Tezuka's generator");
}

TGenerator::~TGenerator()
{}

// ===========================================================================

/*
	Local used by compiler random number generator
	Here are members of class, that represents one
*/

LocalGenerator::LocalGenerator()
{}

void LocalGenerator::Init(Kernel& init)
{
    _init=init;
    _old=RAND_MAX;
    srand(init[0]);
}

double LocalGenerator::Value(void)
{
    return(((double)rand())/RAND_MAX);
}

Kernel LocalGenerator::InitKernel(void)
{
    return(_init);
}

const char* LocalGenerator::Name(void)
{
    return("Compiler built-in generator");
}

LocalGenerator::~LocalGenerator()
{
}

/*
	R3 Generator members

*/

R3Generator::R3Generator()
{
    MBIG=1000000000;
    MSEED=161803398;
    MZ=0;
    FAC=(1.0/MBIG);
}

void R3Generator::Init(Kernel& init)
{
    iff=0;
    _pre0=init[0];
    if (init[1])
        _pre0*=0;
    idum=&_pre0;
}

#define MBIG 1000000000
#define MSEED 161803398
#define MZ 0
#define FAC (1.0/MBIG)

double R3Generator::Value(void)
{
    long mj,mk;
    int i,ii,k;

    if (*idum < 0 || iff == 0) {
        iff=1;
        mj=MSEED-(*idum < 0 ? -*idum : *idum);
        mj %= MBIG;
        ma[55]=mj;
        mk=1;
        for (i=1;i<=54;i++) {
            ii=(21*i) % 55;
            ma[ii]=mk;
            mk=mj-mk;
            if (mk < MZ) mk += MBIG;
            mj=ma[ii];
        }
        for (k=1;k<=4;k++)
            for (i=1;i<=55;i++) {
                ma[i] -= ma[1+(i+30) % 55];
                if (ma[i] < MZ) ma[i] += MBIG;
            }
        inext=0;
        inextp=31;
        *idum=1;
    }
    if (++inext == 56) inext=1;
    if (++inextp == 56) inextp=1;
    mj=ma[inext]-ma[inextp];
    if (mj < MZ) mj += MBIG;
    ma[inext]=mj;
    return mj*FAC;
}

Kernel R3Generator::InitKernel(void)
{
    return(_init);
}

const char* R3Generator::Name(void)
{
    return("ran3 generator");
}

R3Generator::~R3Generator()
{}