#if !defined (hash_V100_H_INCLUDE)
#define hash_V100_H_INCLUDE

/*!
    this structure hold an linear dynamic hash table.
    the key type is an unsigned integer, value type is an void *
    hash table can hold variant value types , using void *.
*/
namespace ZPHashTable{
typedef void * CDYNHASHTB;
typedef void * CDYNHASHIt;
/*!
   �½�һ����ϣ�� spFacMax �Ǵ������ѵ�����(����ȡ>=2), spFacMin�Ǵ����ϲ������޿���ȡ>=1��
   ����һ����������еĲ��������������������
*/
CDYNHASHTB hash_init(float spFacMax, float spFacMin);

///��������޸�һ��ֵ��
void hash_set(CDYNHASHTB table, unsigned int key, void * value);

///ɾ�� key
void hash_del(CDYNHASHTB table, unsigned int key);

///���ұ��е�ֵ������һ�����ҽ���洢����bfound����bfound==0ʱ����ʾû�ҵ�
void * hash_get(CDYNHASHTB table, unsigned int key, int * bfound);

///���������Ƿ��м� key, ����-1��ʾ���У�0��ʾľ��
int hash_contains(CDYNHASHTB table, unsigned int key);

///������ʹ�ã���stdin������ϣ��
void hash_dump(CDYNHASHTB table);

///�������е�����Ԫ�أ���ʼһ�α���
CDYNHASHIt hash_iterate_begin(CDYNHASHTB table);

///���õ�����
void hash_iterate_reset(CDYNHASHIt iter);

///�������е�����Ԫ�أ���������,���к󣬷���ֵΪ��0��ʾ�ɹ�ȡ����һ�����ݣ�Ϊ0��ʾ��ͷ�ˡ�
int hash_iterate_next(CDYNHASHIt iter, unsigned int * pkey, void ** pvalue);

///�������е�����Ԫ�أ��رձ��������ͷ��ڴ�
void hash_iterate_end(CDYNHASHIt iter);

///ɾ�������������ڴ�
void hash_fini(CDYNHASHTB table);
}
#endif
