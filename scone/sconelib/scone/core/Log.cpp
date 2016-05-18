#include "Log.h"

#include <boost/thread/mutex.hpp>
#include <boost/thread/lock_guard.hpp>
#include <stdarg.h>

#ifdef WIN32
#	pragma warning( disable: 4996 ) // we don't need to push/pop because it's not a header
#endif

#define LOG_MESSAGE_F( LEVEL, FORMAT ) \
if ( LEVEL >= g_LogLevel ) \
{ \
	va_list args; va_start( args, FORMAT ); \
		char _buf_[ g_MaxLogMessageSize ]; \
		vsnprintf( _buf_, sizeof( _buf_ ), FORMAT, args ); \
		LogMessageNoCheck( LEVEL, _buf_ ); \
		va_end( args ); \
}

namespace scone
{
	namespace log
	{
		// TODO: replace g_LogLevel with atomic variable
		// Even though simple read / write operations are atomic on WIN32
		// (see https://msdn.microsoft.com/en-us/library/windows/desktop/ms684122(v=vs.85).aspx)
		Level g_LogLevel = InfoLevel;
		boost::mutex g_LogMutex;
		const int g_MaxLogMessageSize = 1000;

		std::ostream& LogStream() { return std::cout; }

		void LogMessageNoCheck( Level level, const char* message )
		{
			boost::lock_guard< boost::mutex > lock( g_LogMutex );
			LogStream() << message << std::endl;
		}

		void LogMessageCheck( Level level, const char* message )
		{
			if ( level >= g_LogLevel )
				LogMessageNoCheck( level, message );
		}

		void SCONE_API LogMessage( Level level, const String& msg )
		{
			LogMessageCheck( level, msg.c_str() );
		}

		void SCONE_API SetLevel( Level level )
		{
			g_LogLevel = level;	
		}

		Level SCONE_API GetLevel()
		{
			return g_LogLevel;
		}

		void SCONE_API Trace( const String& msg )
		{
			LogMessageCheck( TraceLevel, msg.c_str() );
		}

		void SCONE_API Debug( const String& msg )
		{
			LogMessageCheck( DebugLevel, msg.c_str() );
		}

		void SCONE_API Info( const String& msg )
		{
			LogMessageCheck( InfoLevel, msg.c_str() );
		}

		void SCONE_API Warning( const String& msg )
		{
			LogMessageCheck( WarningLevel, msg.c_str() );
		}

		void SCONE_API Error( const String& msg )
		{
			LogMessageCheck( ErrorLevel, msg.c_str() );
		}

		void SCONE_API Critical( const String& msg )
		{
			LogMessageCheck( CriticalLevel, msg.c_str() );
		}

		void SCONE_API WarningF( const char* msg, ... )
		{
			LOG_MESSAGE_F( WarningLevel, msg );
		}

		void SCONE_API PeriodicTraceF( int period, const char* msg, ... )
		{
			static int counter = 0;
			if ( counter++ % period == 0 ) {
				LOG_MESSAGE_F( TraceLevel, msg );
			}
		}

		void SCONE_API TraceF( const char* msg, ... )
		{
			LOG_MESSAGE_F( TraceLevel, msg );
		}

		void SCONE_API DebugF( const char* msg, ... )
		{
			LOG_MESSAGE_F( DebugLevel, msg );
		}

		void SCONE_API InfoF( const char* msg, ... )
		{
			LOG_MESSAGE_F( InfoLevel, msg );
		}

		void SCONE_API ErrorF( const char* msg, ... )
		{
			LOG_MESSAGE_F( ErrorLevel, msg );
		}

		void SCONE_API CriticalF( const char* msg, ... )
		{
			LOG_MESSAGE_F( CriticalLevel, msg );
		}
	}
}