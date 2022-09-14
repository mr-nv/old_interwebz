#pragma once

/* GetRandom returns a random integer between min and max. */
#define _GetRandom( min, max ) ((rand() % (int)(((max) + 1) - (min))) + (min))
#define MakePtr(cast, ptr, addValue) (cast)( (DWORD)(ptr) + (DWORD)(addValue))


#define FORMAT_ARGS( buf )	    \
	va_list argptr;					\
	va_start( argptr, fmt );		\
	vsprintf( buf, fmt, argptr );	\
	va_end( argptr );				\

#define REPEAT_ONCE( action )   \
	static bool f = false;          \
	if( !f )                        \
		{                               \
		action                      \
		f = true;                   \
		}

#define AsciiAnim_Timer 10
#define AsciiBoxLen 47

#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define LIGHTGREY 7
#define DARKGREY 8
#define LIGHTBLUE 9
#define LIGHTGREEN 10
#define LIGHTCYAN 11
#define LIGHTRED 12
#define LIGHTMAGENTA 13
#define YELLOW 14
#define WHITE 15
#define BLINK 128


// getvtable( inst, offset )
// Returns the vtable as an array of void pointers.
inline void**& getvtable( void* inst, size_t offset = 0 )
{
	return *reinterpret_cast<void***>( (size_t)inst + offset );
}
inline const void** getvtable( const void* inst, size_t offset = 0 )
{
	return *reinterpret_cast<const void***>( (size_t)inst + offset );
}

// getvfunc<T>( inst, index, offset )
// Returns the vfunc from the instance casted to a (function) type of your choice.
template< typename Fn >
inline Fn getvfunc( const void* inst, size_t index, size_t offset = 0 )
{
	return reinterpret_cast<Fn>( getvtable( inst, offset )[ index ] );
}  