
#include <news_reader.h>

void* deal_register_request(void* arg)
{
	//构造请求回复包
	register_req* pRegisterReq = (register_req*)arg;
	register_res* pRegisterRes = (register_res*)malloc(sizeof(register_res));
	pRegisterRes->nType = REGISTER_RES;

	//查询和插入用户信息
	//MYSQL* pSQLHandler = connect_database("localhost", "root", "", "news_reader");
	sql_node_t *pSQLNode  = get_db_connect(pGlobalSQLPool);

	//用户已注册
	if(check_user_info(pSQLNode->pSQLHandler, pRegisterReq->szMail, pRegisterReq->szPasswd) != _user_not_exist)
	{
		pRegisterRes->nResult = _user_exist;
		write(pRegisterReq->nClientFd, pRegisterRes, sizeof(register_res));
	}else{
		insert_user(pSQLNode->pSQLHandler, pRegisterReq->szMail, pRegisterReq->szName, pRegisterReq->szPasswd);
		pRegisterRes->nResult = _register_success;
		write(pRegisterReq->nClientFd, pRegisterRes, sizeof(register_res));
	}

	//内存释放
	release_sql_node(pGlobalSQLPool, pSQLNode);	
	free(pRegisterRes);
	pRegisterRes = NULL;

	return NULL;
}
