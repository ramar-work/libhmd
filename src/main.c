#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <zhttp.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <mkdio.h>


//app
int app (struct HTTPBody *req, struct HTTPBody *res ) {
	//Define everything
  FILE *mp = NULL; 
  MMIOT * mio = NULL;
	char mbuf[4096] = {0}, buf[4096] = {0}, err[2048] = {0}, bf[1024] = {0};
	char fname[2048] = {0}; //"files/test.md";
	int fd, size;
	struct stat sb;

	//Somewhere, configuration should be allowed that allows one to change which type is evaluated
	//... 

	//Change the end extension
	snprintf( fname, sizeof( fname ), "files/%s.md", req->path );

	if ( stat( fname, &sb ) == -1 ) {
		snprintf( bf, sizeof( bf ), "stat error: %s\n", strerror( errno ) );
		return http_set_error( res, 404, bf ); 
	}

	if ( ( fd = open( fname, O_CREAT | O_RDWR ) ) == -1 ) {
		snprintf( bf, sizeof( bf ), "fd open error: %s\n", strerror( errno ) );
		return http_set_error( res, 500, bf ); 
	} 

	if ( ( size = read( fd, buf, sb.st_size ) ) == -1 ) {
		snprintf( bf, sizeof( bf ), "read error: %s\n", strerror( errno ) );
		return http_set_error( res, 500, bf ); 
	}

	//I feel like I don't really need to cover this either...
	if ( close( fd ) == -1 ) {
		0;	
	}
	
	if ( !( mio = mkd_string( buf, size, 0 )	) ) {
		snprintf( bf, sizeof( bf ), "mkd_string failed." );
		return http_set_error( res, 500, bf ); 
	}

	if ( !( mp = fmemopen( mbuf, sizeof( mbuf ) - 1, "w+" ) ) ) {
		snprintf( bf, sizeof( bf ), "fmemopen error: %s\n", strerror( errno ) );
		return http_set_error( res, 500, bf ); 
	}

	if ( markdown( mio, mp, 0 ) > 0 ) {
		snprintf( bf, sizeof( bf ), "Markdown conversion of %s failed.", fname );
		return http_set_error( res, 500, bf ); 
	}

	//Absolutely hate this interface...
	rewind( mp );
	fseek( mp, 0L, SEEK_END );
	size = ftell( mp ); 

	//Do I really need to cover this?
	if ( fclose( mp ) == -1 ) {
		fprintf( stderr, "failed to close memory stream." );
	}

	//Output it
	mkd_cleanup( mio );
	http_set_status( res, 200 ); 
	http_set_ctype( res, "text/html" );
	http_copy_content( res, mbuf, size ); 
	return 0;
}


#ifdef RUN_MAIN 
int main (int argc, char *argv[]) {
  //Check that the file is even a markdown file
  FILE *mp = NULL; 
  MMIOT * mio = NULL; //= mkd_in() 
	char mbuf[ 4096 ] = {0}, buf[ 4096 ] = {0}, err[2048] = {0};
	const char fname[] = "files/test.md";
	int fd, size;

	//stat and read the file
	struct stat sb;
	if ( stat( fname, &sb ) == -1 ) {
		fprintf( stderr, "Stat failed.\n" ); return 1;
	}

	if ( ( fd = open( fname, O_CREAT | O_RDWR ) ) == -1 ) {
		fprintf( stderr, "open failed.\n" ); return 1;
	} 

	if ( ( size = read( fd, buf, sb.st_size ) ) == -1 ) {
		fprintf( stderr, "read failed.\n" ); return 1;
	}
	
	if ( !( mio = mkd_string( buf, size, 0 )	) ) {
		fprintf( stderr, "mkd_string failed.\n" ); return 1;
	}

	if ( !( mp = fmemopen( mbuf, sizeof( mbuf ) - 1, "w+" ) ) ) {
		fprintf( stderr, "fmemopen failed.\n" ); return 1;
	}


	if ( markdown( mio, mp, 0 ) > 0 ) {
		fprintf( stderr, "markdown failed." );
		return 1;
	}

	rewind( mp );
	fseek( mp, 0L, SEEK_END );
	if ( ( size = ftell( mp ) ) == -1 ) {
		fprintf( stderr, "fseek failed." );
		return 1;
	}

fprintf( stderr,"size of in-memory file: %d\n", size );
	if ( fclose( mp ) == -1 ) {
		fprintf( stderr, "failed to close memory stream." );
	}

	mkd_cleanup( mio );
#if 0
	write( 2, mbuf, size );
#else
	
#endif
	return 0;
}
#endif
