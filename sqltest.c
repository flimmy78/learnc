#include <stdio.h>
#include <sqlite3.h>
int rows;
int LoadMyInfo( void * para,  int cols,  char ** column_value,  char ** column_name )
{
	int i;
    rows++;
	printf("记录包含 %d 个字段\n", cols);
	for( i = 0 ; i < cols; i ++ )
	{
		printf("字段名:%s  ?> 字段值:%s\n",  column_name[i], column_value[i] );
	}
	printf("------------------\n"); 
	return 0;
}

int main( int argc, char ** argv)
{
	sqlite3 * db;
	int result;
	char *errmsg =NULL;
    rows = 0;
	result =sqlite3_open("database.db", &db);
	if( result !=SQLITE_OK)
	{
		return -1;  
	}

	result = sqlite3_exec( db,"create table MyTable_1( ID integer primary key autoincrement, name nvarchar(32) )", NULL, NULL, &errmsg );
	if(result != SQLITE_OK)
	{
		 printf("创建表失败，错误码:%d，错误原因:%s\n", result, errmsg);
	}

	result = sqlite3_exec( db, "insert into MyTable_1( name ) values ('走路')", 0, 0, &errmsg);
	if(result != SQLITE_OK)
	{
		 printf("插入记录失败，错误码:%d，错误原因:%s\n", result, errmsg);
	}
	result = sqlite3_exec( db, "insert into MyTable_1( name ) values ('骑单车')", 0, 0, &errmsg );
	if(result != SQLITE_OK)
	{
	     printf("插入记录失败，错误码:%d，错误原因:%s\n", result, errmsg );
	}
	result = sqlite3_exec( db, "insert into MyTable_1( name ) values ('坐汽车')", 0, 0, &errmsg );
	if(result != SQLITE_OK)
	{
		 printf("插入记录失败，错误码:%d，错误原因:%s\n", result, errmsg );
	}

	result = sqlite3_exec( db, "select * from MyTable_1", LoadMyInfo, NULL, &errmsg );//call callback() each row
    printf("total %d rows\n", rows);
	sqlite3_close( db );
	return 0;
}
