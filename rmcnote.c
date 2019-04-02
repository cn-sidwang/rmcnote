
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define  CH_LR		(0x0A)
#define  MARK_LEFT	"/*"
#define  MARK_RIGHT	"*/"
#define  MARK_LINE	"//"
#define  STR_SYM	'\"'

void _move_text( char *buf, int start, int end )
{
	while( ( buf[start++] = buf[end++] ) );
}

void _rmmark( char *buf, size_t size )
{
	int  index = 0;
	for( int i=0; i<size; i++ )
	{
		if( buf[i]==(char)(CH_LR) )
		{
			index = i;
			while( buf[++index]==(char)(CH_LR) );
			if( (index-i)>2 )
				_move_text( buf, i, index );
		}

		if( buf[i]==STR_SYM )
		{
			while( buf[i]!='\0' && buf[i++]!=STR_SYM );
		}

		if( strncmp( buf+i, MARK_LEFT, strlen(MARK_LEFT) )==0 )
		{
			index = i;
			while( buf[index]!='\0' 
				&& strncmp( buf+index, MARK_RIGHT, strlen(MARK_RIGHT) )!=0
			    	&& index++ );
			if( strncmp( buf+index, MARK_RIGHT, strlen(MARK_RIGHT) )==0 )
			{
				_move_text( buf, i, index+strlen(MARK_RIGHT) );
			}
		}

		if( strncmp( buf+i, MARK_LINE, strlen(MARK_LINE) )==0 )
		{
			index = i;
			while( buf[index]!='\0' && buf[index]!=(char)(CH_LR) && index++ );
			_move_text( buf, i, index );
		}
	}
}

char* rmmark( const char *file )
{
	char 	*ret = NULL;
	FILE 	*fp = NULL;
	size_t  file_size = 0;

	fp = fopen( file, "r" );
	if( fp!=NULL )
	{
		fseek( fp, 0, SEEK_END );
		file_size = ftell(fp);
		rewind( fp );
		ret = (char*)calloc(sizeof(char), file_size + 2);
		ret[0] = CH_LR;
		fread( ret+1, file_size, sizeof(char), fp );
		fclose(fp);
		_rmmark( ret, file_size );
	}
	else
	{
		perror("Error");
	}
	return ret;
}

int main(int argc, char **argv)
{
	char *ret = NULL;
	ret = rmmark( argv[1] );
	if( ret!=NULL )
	{
		printf("%s", ret );
		free( ret );
	}
	printf("\n");
	return 0;
}

