#ifndef CONST_DATA_H_
#define CONST_DATA_H_

// K������������Ϣ
#define UM__NOTIFY_REQ_KLINE (WM_USER+1)
#define UM_NOTIFY_REQ_COMPLETE (WM_USER+2)
#define UM_NOTIFY_REQ_LOADING (WM_USER+3)

// ��Ʊ����������Ϣ
#define UM_NOTIFY_REQ_DOWNLOAD_STOCK_CODE (WM_USER+4)

// k����������״̬
enum EN_KLINE
{
	en_no_request,
	en_requesting,
	en_request_complete,
	en_request_failed
};

enum WIND_ERROR_CODE
{
	WQERR_OK						= 0,		// �����ɹ�
	WQERR_BASE						= -40520000,// һ���Դ���
	WQERR_UNKNOWN					= -40520001,// δ֪����
	WQERR_INTERNAL_ERROR			= -40520002,// �ڲ�����
	WQERR_SYSTEM_REASON				= -40520003,// ����ϵͳԭ��
	WQERR_LOGON_FAILED				= -40520004,// ��½ʧ��
	WQERR_LOGON_NOAUTH				= -40520005,// �޵�½Ȩ��
	WQERR_USER_CANCEL				= -40520006,// �û�ȡ��
	WQERR_NO_DATA_AVAILABLE			= -40520007,// û�п�������
	WQERR_TIMEOUT					= -40520008,// ����ʱ
	WQERR_LOST_WBOX					= -40520009,// Wbox ����
	WQERR_ITEM_NOT_FOUND			= -40520010,// δ�ҵ��������
	WQERR_SERVICE_NOT_FOUND			= -40520011,// δ�ҵ���ط���
	WQERR_ID_NOT_FOUND				= -40520012,// δ�ҵ���� ID
	WQERR_LOGON_CONFLICT			= -40520013,// ���ڱ���ʹ�������˺ŵ�¼��������������޷�ʹ��ָ���˺ŵ�¼
	WQERR_LOGON_NO_WIM				= -40520014,// δ��¼ʹ�� WIM ���ߣ����޷���¼
	WQERR_TOO_MANY_LOGON_FAILURE	= -40520015,// ������¼ʧ�ܴ�������
	WQERR_IOERROR_CLASS				= -40521000,// �������ݴ�ȡ����
	WQERR_IO_ERROR					= -40521001,// IO ��������
	WQERR_SERVICE_NOT_AVAL			= -40521002,// ��̨������������
	WQERR_CONNECT_FAILED			= -40521003,// ��������ʧ��
	WQERR_SEND_FAILED				= -40521004,// ������ʧ��
	WQERR_RECEIVE_FAILED			= -40521005,// ���ݽ���ʧ��
	WQERR_NETWORK_ERROR				= -40521006,// �������
	WQERR_SERVER_REFUSED			= -40521007,// �������ܾ�����
	WQERR_SVR_BAD_RESPONSE			= -40521008,// �����Ӧ��
	WQERR_DECODE_FAILED				= -40521009,// ���ݽ���ʧ��
	WQERR_INTERNET_TIMEOUT			= -40521010,// ���糬ʱ
	WQERR_ACCESS_FREQUENTLY			= -40521011,// Ƶ������
	WQERR_SERVER_INTERNAL_ERROR		= -40521012,// �������ڲ�����
	WQERR_INVALID_CLASS				= -40522000,// �����������
	WQERR_ILLEGAL_SESSION			= -40522001,// �޺Ϸ��Ự
	WQERR_ILLEGAL_SERVICE			= -40522002,// �Ƿ����ݷ���
	WQERR_ILLEGAL_REQUEST			= -40522003,// �Ƿ�����
	WQERR_WINDCODE_SYNTAX_ERR		= -40522004,// ��ô����﷨����
	WQERR_ILLEGAL_WINDCODE			= -40522005,// ��֧�ֵ���ô���
	WQERR_INDICATOR_SYNTAX_ERR		= -40522006,// ָ���﷨����
	WQERR_ILLEGAL_INDICATOR			= -40522007,// ��֧�ֵ�ָ��
	WQERR_OPTION_SYNTAX_ERR			= -40522008,// ָ������﷨����
	WQERR_ILLEGAL_OPTION			= -40522009,// ��֧�ֵ�ָ�����
	WQERR_DATE_TIME_SYNTAX_ERR		= -40522010,// ������ʱ���﷨����
	WQERR_INVALID_DATE_TIME			= -40522011,// ��֧�ֵ�������ʱ��
	WQERR_ILLEGAL_ARG				= -40522012,// ��֧�ֵ��������
	WQERR_INDEX_OUT_OF_RANGE		= -40522013,// �����±�Խ��
	WQERR_DUPLICATE_WQID			= -40522014,// �ظ��� WQID
	WQERR_UNSUPPORTED_NOAUTH		= -40522015,// ��������ӦȨ��
	WQERR_UNSUPPORTED_DATA_TYPE		= -40522016,// ��֧�ֵ���������
	WQERR_DATA_QUOTA_EXCEED			= -40522017,// ������ȡ������
	WQERR_ILLEGAL_ARG_COMBINATION	= -40522018//  ��֧�ֵ��������
};


#endif
