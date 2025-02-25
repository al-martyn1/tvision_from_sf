/*
 *      Turbo Vision - Version 2.0
 *
 *      Copyright (c) 1994 by Borland International
 *      All Rights Reserved.
 *
 *
 * Modified by Robert H�hne to be used for RHIDE.
 * Modified by Vadim Beloborodov to be used on WIN32 console
 *
 * SET: Moved the standard headers here because according to DJ
 * they can inconditionally declare symbols like NULL.
 * Added members for platform indepent writes. Started with a JASC
 * modification. Added: write8, write16, write32, write64, writeShort,
 * writeInt and writeLong.
 *
 */
#include <stdio.h> // fprintf in writeData
#define Uses_string // Needed for Win32
#define Uses_PubStreamBuf
#define Uses_TStreamable
#define Uses_TStreamableClass
#define Uses_TPWrittenObjects
#define Uses_TStreamableTypes
#define Uses_pstream
#define Uses_opstream
#define Uses_TPWrittenObjects
#include <tv.h>

opstream::opstream()
{
    objs = new TPWrittenObjects();
}

opstream::opstream( streambuf * sb )
{
    objs = new TPWrittenObjects();
    pstream::init( sb );
}

opstream::~opstream()
{
    destroy(objs);
}

opstream& opstream::seekp( streampos pos )
{
    objs->freeAll();
    bp->PubSeekOff( pos, ios::beg );
    return *this;
}

opstream& opstream::seekp( streamoff pos, ios::seek_dir dir )
{
    objs->freeAll();
    bp->PubSeekOff( pos, dir );
    return *this;
}

streampos opstream::tellp()
{
    return bp->PubSeekOff( 0, ios::cur, ios::out );
}

opstream& opstream::flush()
{
    bp->PubSync();
    return *this;
}

void opstream::writeByte( uchar ch )
{
    bp->sputc( ch );
}

void opstream::writeBytes( const void *data, size_t sz )
{
    bp->sputn( (char *)data, sz );
}

/*
 *  writeShort, writeInt and writeLong:
 *  These are platform dependent, writes the size and byte order of the native
 *  platform.
 *  Created by SET to be compatible with original code and V. Bugrov ideas.
 */

#define DefineWriteDep(name,type)\
void opstream::write##name(type val)\
{\
 bp->sputn((char *)&val,sizeof(type));\
}
DefineWriteDep(Short,ushort);
DefineWriteDep(Int,uint);
DefineWriteDep(Long,ulong);

/*
 *  write16, write32 and write64:
 *  These are platform independent, writes a fixed size in little endian order
 *  and if the platform is big endian swaps bytes.
 *  Created by SET to be compatible with original code and JASC + V. Bugrov
 *  ideas.
 */

#ifdef TV_BIG_ENDIAN
#define Swap(a,b) t=v[a]; v[a]=v[b]; v[b]=t

static inline
void Swap16(char *v)
{
 char t;
 Swap(0,1);
}

static inline
void Swap32(char *v)
{
 char t;
 Swap(0,3);
 Swap(1,2);
}

static inline
void Swap64(char *v)
{
 char t;
 Swap(0,7);
 Swap(1,6);
 Swap(2,5);
 Swap(3,4);
}
#else
static inline
void Swap16(char *) {}
static inline
void Swap32(char *) {}
static inline
void Swap64(char *) {}
#endif

#define DefineWrite(name,type)\
void opstream::write##name(type val)\
{\
 Swap##name((char *)&val);\
 bp->sputn((char *)&val,sizeof(type));\
}
DefineWrite(16,uint16);
DefineWrite(32,uint32);
DefineWrite(64,uint64);

void opstream::writeString( const char *str )
{
    if( str == 0 )
        {
        writeByte( 0xFF );
        return;
        }
    int len = strlen( str );
    if (len > 0xfd)
    {
      writeByte( 0xfe );
      write32(len);
    }
    else
    {
      writeByte( (uchar)len );
    }
    writeBytes( str, len );
}

/* Operators moved to headers by JASC */

opstream& operator << ( opstream& ps, TStreamable& t )
{
    ps.writePrefix( t );
    ps.writeData( t );
    ps.writeSuffix( t );
    return ps;
}

opstream& operator << ( opstream& ps, TStreamable *t )
{
    P_id_type index;
    if( t == 0 )
        ps.writeByte( pstream::ptNull );
    else if( (index = ps.find( t )) != P_id_notFound )
        {
        ps.writeByte( pstream::ptIndexed );
        ps.writeWord( index );
        }
    else
        {
        ps.writeByte( pstream::ptObject );
        ps << *t;
        }
    return ps;
}

void opstream::writePrefix( const TStreamable& t )
{
    writeByte( '[' );
    writeString( t.streamableName() );
}


void opstream::writeData( TStreamable& t )
{
    if( types->lookup( t.streamableName() ) == 0 )
        {
        fprintf(stderr,_("type not registered: %s\n"),t.streamableName());
        error( peNotRegistered, t );
        }
    else
        {
        registerObject( &t );
        t.write( *this );
        }
}

void opstream::writeSuffix( const TStreamable& )
{
    writeByte( ']' );
}

P_id_type opstream::find( const void *adr )
{
    return objs->find( adr );
}

void opstream::registerObject( const void *adr )
{
    objs->registerObject( adr );
}

