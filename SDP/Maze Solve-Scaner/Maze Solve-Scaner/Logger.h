
#ifndef LOGGER_HEADER_INLCUDED
#define LOGGER_HEADER_INCLUDED
//
//
// needs more work
//
//
#ifdef LOG

#include <iostream>
#include <ctime>

using std::clog;
//using namespace np;

#define LOGGER clog
#define LOG_TEXT(T) LOGGER << T;
#define LOG_ENDL LOGGER << std::endl;

#define LOG_COLOR(C) LOGGER << std::hex << C << std::dec;

#define LOG_POINT_COORD(Pt) LOGGER << "( " << Pt.x << " , " << " )" ;
#define LOG_POINT_COLOR(Pt, Map) LOGGER << "color " << std::hex << Map.getPtColor(Pt.x, Pt.y) << std::dec;
#define LOG_POINT_DIST_FROM_START(Pt, Map) LOGGER << "dist from start " << Map.getPtDistFrStart(Pt.x, Pt.y);
#define LOG_POINT(Pt, Map)	LOG_POINT_COORD(Pt); \
							LOG_POINT_COLOR(Pt, Map); \
							LOG_POINT_DIST_FROM_START(Pt, Map); 
#define LOG_POINT_MARK_VISIT(Pt) LOGGER << " Marking pt (" << Pt.x << " , " << Pt.y << ") as visited";
#define LOG_POINT_CHANGE_SOURCE(Pt, OSource, NSource)	LOGGER << " Changing Pt ";	\
														LOG_POINT_COORD(Pt);		\
														LOGGER << "source from ";	\
														LOG_POINT_COORD(OSource);	\
														LOGGER << " to ";			\
														LOG_POINT_COORD(NSource);


#define LOG_MAX_HEAP_TOP(T, Map)	LOG_POINT(T.first, Map); \
									LOGGER << "	dist to Objective " << T.second << std::endl;
#define LOG_A_STAR_END_CONDITION(T , Pt ,Map)	LOGGER	<< T.second << " == 0 || ";	\
												LOG_POINT_COLOR(T.first, Map);		\
												LOGGER << " == " ;					\
												LOG_POINT_COLOR(Pt, Map);			\
												LOG_ENDL;

#define LOG_POINT_ARRAY(Arr, Size, Map)	for(size_t i = 0; i < Size; ++i) \
											{LOG_POINT(Arr[i], Map); };
#define LOG_POINT_VECTOR(Vect, Map) LOG_POINT_ARRAY(Vect, Vect.size(), Map);
#define LOG_KEYS_ARRAY(Keys, Map)	LOGGER << " Keys Array::";	\
									LOG_ENDL;					\
									LOG_POINT_VECTOR(Keys, Map);

#define LOG_POINT_CHANGE_SOURCE_FAIL_BETTER_SOURCE(Pt, Source, Map) LOGGER	<< " Failed :: Point has a better source "		\
																			<< map.getPtDistFrStart(pt.x, pt.y) << " < "	\
																			<< map.getPtDistFrStart(source.x, source.y) + 1 << ")";
#define LOG_POINT_OUT_OF_BOUNDS(Source) {LOGGER << " Failed :: Point out of bounds ";\
										 LOG_POINT_COORD(Source);}


#else

#define LOGGER
#define LOG_TEXT(T)
#define LOG_ENDL
#define LOG_COLOR(C)
#define LOG_POINT_COORD(Pt) 
#define LOG_POINT_COLOR(Pt, Map) 
#define LOG_POINT_DIST_FROM_START(Pt, Map) 
#define LOG_POINT(Pt, Map)
#define LOG_POINT_MARK_VISIT(Pt) 
#define LOG_POINT_CHANGE_SOURCE(Pt, OSource, NSource)
#define LOG_MAX_HEAP_TOP(T, Map)
#define LOG_A_STAR_END_CONDITION(T , Pt ,Map)
#define LOG_POINT_ARRAY(Arr, Size, Map)	
#define LOG_POINT_VECTOR(Vect, Map)
#define LOG_KEYS_ARRAY(Keys, Map)
#define LOG_POINT_CHANGE_SOURCE_FAIL_BETTER_SOURCE(Pt, Source, Map)
#define LOG_POINT_OUT_OF_BOUNDS(Source)

#endif

#endif


